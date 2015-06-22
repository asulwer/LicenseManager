#include "Stdafx.h"
#include "WinAES.h"

using std::ostringstream;

WinAES::WinAES(const wchar_t* lpszContainer, int nFlags) : m_hProvider(NULL),m_hAesKey(NULL),m_bHaveIv(false),m_nFlags(nFlags),m_wszContainer(NULL),m_nIndex(INVALID_INDEX)
{
	std::string key = "PaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRd";
	std::string iv = rsg.Generate(BLOCKSIZE);
	std::copy(key.begin(), key.end(), KEY);
	std::copy(iv.begin(), iv.end(), IV);
	
	if(lpszContainer == NULL)
	{
        lpszContainer = L"Temporary - OK to Delete";
        m_nFlags |= DELETE_CONTAINER;
    }

    size_t len = wcslen(lpszContainer) + 1;
	
    if(len > 0)
	{
        m_wszContainer = new wchar_t[len];
		errno_t err = 0;
        err = wcscpy_s(m_wszContainer, len, lpszContainer);
		
		if(err != 0)
			throw WinAESException("WinAES: Copy Error");
    }
	else
	{
		throw WinAESException("WinAES: Container size <= 0");
	}

    if(!AcquireContext(m_wszContainer))
		throw WinAESException("WinAES: AcquireContext failed");
}

WinAES::WinAES(HCRYPTPROV hProvider) : m_hProvider(hProvider),m_hAesKey(NULL),m_bHaveIv(false),m_nFlags(DEFAULT_FLAGS),m_wszContainer(NULL),m_nIndex(INVALID_INDEX)
{
	std::string key = "PaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRd";
	std::string iv = rsg.Generate(BLOCKSIZE);
	std::copy(key.begin(), key.end(), KEY);
	std::copy(iv.begin(), iv.end(), IV);
}

WinAES::~WinAES()
{
    // Destroy the key
    if(m_hAesKey != NULL)
	{
        CryptDestroyKey(m_hAesKey);
        m_hAesKey = NULL;
    }

    // Destroy the provider
    if(m_hProvider != NULL)
	{
        CryptReleaseContext(m_hProvider, 0);
        m_hProvider = NULL;
    }

    // Destroy the conatiner
    if(DELETE_CONTAINER & m_nFlags)
    {
        if(INVALID_INDEX != m_nIndex)
        {       
            // MSDN: When CRYPT_DELETEKEYSET flag is set, the value returned in phProv
            // is undefined, and thus, the CryptReleaseContext function need not be
            // called afterward.
            CryptAcquireContext(&m_hProvider, m_wszContainer, AesProviders[m_nIndex].params.lpwsz, AesProviders[m_nIndex].params.dwType, CRYPT_DELETEKEYSET);
        }
        m_hProvider = NULL;
    }
    m_nIndex = INVALID_INDEX;

    // Free
    if(m_wszContainer != NULL)
	{
        delete[] m_wszContainer;
        m_wszContainer = NULL;
    }
}

bool WinAES::GenerateRandom( unsigned char* buffer, int size )
{
	if(buffer == NULL)
		throw WinAESException("GenerateRandom: buffer empty");
    
	if(size < 0)
		throw WinAESException("GenerateRandom: size < 0");
		
#ifdef _DEBUG
    // Test the pointer (it is purported good). Not safe for Release builds
    if(IsBadWritePtr(buffer, size) == TRUE)
		throw WinAESException("GenerateRandom: IsBadWritePtr = True");
#endif

	if(m_hProvider == NULL)
		throw WinAESException("GenerateRandom: m_hProvider = NULL");

    // Convert big BOOL to little bool
    return !!CryptGenRandom(m_hProvider, size, buffer);
}

bool WinAES::AcquireContext( const wchar_t* lpszContainer )
{
    // We need to record the index of the provider which we were able to 
    // acquire in case the use wants to delete the imported key. The
    // class destructor will use the index to delete the proper container.

	int iAesProviders = std::extent<decltype(AesProviders)>::value;
	for(int i=0; i<iAesProviders; i++)
    {
        // Only create a new container if requested
        if((AesProviders[i].params.dwFlags & CRYPT_NEWKEYSET) && !(m_nFlags & CREATE_CONTAINER))
		{
			continue;
		}

        if(CryptAcquireContext(&m_hProvider, lpszContainer, AesProviders[i].params.lpwsz, AesProviders[i].params.dwType, AesProviders[i].params.dwFlags))
		{
            m_nIndex = i;
            break;
        }
    }

    // m_nIndex != INVALID_INDEX indicates success. No need for an extra variable to track success from CryptAcquireContext.
    if((m_nIndex == INVALID_INDEX) && (THROW_EXCEPTION & m_nFlags))
	    throw WinAESException("AcquireContext: CryptAcquireContext failed");
    
    return m_nIndex != INVALID_INDEX;
}

HCRYPTPROV WinAES::DuplicateContext()
{
    if(m_hProvider == NULL)
	{
		return NULL;
	}

    if(CryptContextAddRef(m_hProvider, NULL, 0))
	{
		return m_hProvider;
	}

    return NULL;
}

bool WinAES::GenerateDefaults()
{
	bool result = true;

	try
	{
		result = SetKey(KEY, sizeof(KEY));
        if(!result)
		    throw WinAESException("SetKeyWithIv: SetKey failed");
        
        result = SetIv(IV, sizeof(IV));
        if(!result)
		    throw WinAESException("SetKeyWithIv: SetIv failed");
    }
	catch(const WinAESException& /*e*/)
    {
        if(THROW_EXCEPTION & m_nFlags)
	        throw;
    
        result = false;
    }

	return result;
}

bool WinAES::SetKeyWithIv(const unsigned char* key, int ksize, const unsigned char* iv, int vsize)
{
#ifdef _DEBUG
	if(IsBadReadPtr(key, ksize) == TRUE)
		throw WinAESException("SetKeyWithIv: IsBadWritePtr = True");

	if(IsBadReadPtr(iv, vsize) == TRUE)
		throw WinAESException("SetKeyWithIv: IsBadWritePtr = True");
#endif

    // Returned to caller (if non-throwing)
    bool result = false;

    // re-keying?, we need to release the old key here...
    if(m_hAesKey != NULL)
	{
        // Non-fatal
        CryptDestroyKey(m_hAesKey);
        m_hAesKey  = NULL;
        m_bHaveIv = false;
    }

    try
    {
        if(m_hProvider == NULL)
		    throw WinAESException( "SetKeyWithIv: Provider is not valid" );
        
        if(key == NULL)
		    throw WinAESException( "SetKeyWithIv: Key buffer is not valid" );
        
        if(!(ksize == KEYSIZE_128 || ksize == KEYSIZE_192 || ksize == KEYSIZE_256))
		    throw WinAESException( "SetKeyWithIv: Key size is not valid" );
        
        if(iv == NULL)
		    throw WinAESException( "SetKeyWithIv: IV buffer is not valid" );
        
        if(vsize != BLOCKSIZE)
		    throw WinAESException( "SetKeyWithIv: IV size is not valid" );
        
        result = SetKey(key, ksize);
        if(!result)
		    throw WinAESException("SetKeyWithIv: SetKey failed");
        
        result = SetIv(iv, vsize);
        if(!result)
		    throw WinAESException("SetKeyWithIv: SetIv failed");
    }
    catch(const WinAESException& /*e*/)
    {
        if(THROW_EXCEPTION & m_nFlags)
		{
            throw;
        }

        result = false;
    }

    return result;
}

// Sets the Key. The key must be of size KEYSIZE_128, KEYSIZE_192 or KEYSIZE_256.
bool WinAES::SetKey( const unsigned char* key, int ksize )
{
#ifdef _DEBUG
    if(IsBadReadPtr(key, ksize) == TRUE)
		throw WinAESException("SetKey: IsBadWritePtr = True");
#endif

    // Returned to caller (if non-throwing)
    bool result = false;

    // Is someone is re-keying, we need to release the old key here...
    if(m_hAesKey != NULL)
	{
        // Non-fatal
        CryptDestroyKey(m_hAesKey);
        m_hAesKey  = NULL;
        m_bHaveIv = false;
    }

    try
    {
        if(m_hProvider == NULL)
		    throw WinAESException( "SetKey: Provider is not valid" );
        
        if(key == NULL)
		    throw WinAESException( "SetKey: Key buffer is NULL" );
        
        if(!(ksize == KEYSIZE_128 || ksize == KEYSIZE_192 || ksize == KEYSIZE_256))
		    throw WinAESException( "SetKey: Key size is not valid" );
        
        AesKey aeskey;

        switch(ksize)
        {
        case KEYSIZE_128:
            aeskey.Header.aiKeyAlg = CALG_AES_128;
            aeskey.dwKeyLength = KEYSIZE_128;
            break;
        case KEYSIZE_192:
            aeskey.Header.aiKeyAlg = CALG_AES_192;
            aeskey.dwKeyLength = KEYSIZE_192;
            break;
        case KEYSIZE_256:
            aeskey.Header.aiKeyAlg = CALG_AES_256;
            aeskey.dwKeyLength = KEYSIZE_256;
            break;
        default:
            throw WinAESException("SetKey: Key size is not valid");
			break;
        }

        errno_t err = 0;
        err = memcpy_s(aeskey.cbKey, aeskey.dwKeyLength, key, ksize);
        
		if(err != 0)
		    throw WinAESException( "SetKey: Unable to copy key" );
        
        // When the key is imported, the size of the AesKey structure should be exact.
        //  We declared aeskey.cbKey[ KEYSIZE_256 ] to allow a stack allocation.
        //  There are enough unsigned chars available for AES 128, 192, and 256. We do the
        //  proper book keeping now.
        const unsigned structsize = sizeof(AesKey) - KEYSIZE_256 + ksize;

        // Import AES key
        if(!CryptImportKey(m_hProvider, (CONST BYTE*)&aeskey, structsize, NULL, 0, &m_hAesKey))
		    throw WinAESException("SetKey: Import key failed");
        
        // Set Mode
        DWORD dwMode = CRYPT_MODE_CBC;
        if(!CryptSetKeyParam(m_hAesKey, KP_MODE, (BYTE*)&dwMode, 0))
		    throw WinAESException("SetKey: Set CBC mode failed");
        
        result = true;
    }
    catch(const WinAESException& /*e*/)
    {
        if(THROW_EXCEPTION & m_nFlags)
		{
            throw;
        }
        result = false;
    }

    return result;
}

// Sets the IV. The IV must be of size BLOCKSIZE.
bool WinAES::SetIv(const unsigned char* iv, int vsize )
{
#ifdef _DEBUG
    if(IsBadReadPtr(iv, vsize) == TRUE)
		throw WinAESException("SetIv: IsBadWritePtr = True");
#endif

    // Returned to caller (if non-//throwing)
    bool result = false;

    try
    {
        if(m_hAesKey == NULL)
		    throw WinAESException("SetIv: key is not valid");
        
        if(iv == NULL)
		    throw WinAESException("SetIv: IV buffer is NULL");
        
        if(vsize != BLOCKSIZE)
		    throw WinAESException("SetIv: IV block size is not valid");
        
        if(m_hAesKey == NULL)
		    throw WinAESException("SetIv: Key is not valid");
        
        if(!CryptSetKeyParam(m_hAesKey, KP_IV, (BYTE*)iv, 0))
		    throw WinAESException("SetIv: Set IV failed");
        
        // Set Mode
        DWORD dwMode = CRYPT_MODE_CBC;
        if(!CryptSetKeyParam(m_hAesKey, KP_MODE, (BYTE*)&dwMode, 0))
		    throw WinAESException("SetIv: Set CBC mode failed");
        
		m_bHaveIv = true;
        result = true;
    }
    catch(const WinAESException& /*e*/)
    {
        if(THROW_EXCEPTION & m_nFlags)
		{
            throw;
        }

		m_bHaveIv = false;
        result = false;
    }

    return result;
}

// Returns the maximum size of the ciphertext, which includes padding for the plaintext
bool WinAES::MaxCipherTextSize(unsigned int psize, unsigned int& csize )
{
    unsigned int blocks = psize / BLOCKSIZE + 1;

    csize = (blocks * BLOCKSIZE) + BLOCKSIZE; // adding by BLOCKSIZE is so that we can append the iv
    return true;
}

// Returns the maximum size of the plaintext, which includes removal of padding on the plaintext
bool WinAES::MaxPlainTextSize( unsigned int csize, unsigned int& psize )
{
	if(csize % BLOCKSIZE != 0)
		return false;
	
	psize = csize - BLOCKSIZE; //subtracting by BLOCKSIZE is so that we can adjust the cipher after we have removed the iv from the end of it
    
	return true;
}

// Encrpyt a buffer in-place. bsize is the size of the buffer, psize is the size of the plaintext. If successful, csize is the size of the ciphertext. On entry, bsize >= csize.
bool WinAES::Encrypt( unsigned char* buffer, /*In*/unsigned int bsize, /*In*/unsigned int psize, /*Out*/ unsigned int& csize )
{
#ifdef _DEBUG
    // Test the pointer (it is purported good). Not safe for Release builds
	if(IsBadReadPtr(buffer, (DWORD)bsize) == TRUE)
		throw WinAESException("Encrypt: IsBadWritePtr = True");
#endif

    // Returned to caller (if non-throwing)
    bool result = false;

    try
    {
        // sanity check
        if(m_hAesKey == NULL)
		{
            SetLastError((DWORD)NTE_BAD_KEY);
            throw WinAESException("Encrypt(1): Key is not valid");
        }

        // sanity check
        if(buffer == NULL)
		{
            SetLastError(ERROR_INVALID_USER_BUFFER);
            throw WinAESException("Encrypt(1): Buffer is NULL");
        }

        // sanity check
        unsigned int s = 0;
        if(MaxCipherTextSize(psize, s) && bsize < s)
		{
            SetLastError((DWORD)NTE_BUFFER_TOO_SMALL);
            throw WinAESException("Encrypt(1): Buffer is too small");
        }

        // sanity check
        if(m_bHaveIv == false)
		{
            SetLastError((DWORD)NTE_INVALID_PARAMETER);
            throw WinAESException("Encrypt(1): IV has not been set");
        }

        // temporary for API
        DWORD d = (DWORD)psize;
        if(CryptEncrypt(m_hAesKey, NULL, TRUE, 0, buffer, &d, (DWORD)bsize) == FALSE)
		{
			// Build a useful message
            ostringstream emessage;
            emessage << "Encrypt(1): CryptEncrypt failed - " ;
            emessage << ErrorToDefine( GetLastError() );
            emessage << " (0x" << std::hex << GetLastError() << ")";

            csize = 0;
            throw WinAESException(emessage.str().c_str());
        }

        csize = d;
		result = true;
    }
    catch(const WinAESException& /*e*/)
    {
        if(m_nFlags & THROW_EXCEPTION)
		{
            throw;
        }
        result = false;
    }

    return result;
}

// Decrpyt a buffer in-place. bsize is the size of the buffer, csize is the size of the ciphertext. If successful,
//  psize is the size of the recovered plaintext. On entry, bsize > psize (always the case since csize <= bsize).
bool WinAES::Decrypt( unsigned char* buffer, /*In*/unsigned int bsize, /*In*/unsigned int csize, /*Out*/ unsigned int& psize )
{
#ifdef _DEBUG
    // Test the pointer (it is purported good). Not safe for Release builds
	if(IsBadReadPtr(buffer, (DWORD)bsize) == TRUE)
		throw WinAESException("Decrypt: IsBadWritePtr = True");
#endif

    // Returned to caller (if non-//throwing)
    bool result = false;

    try
    {
        // sanity check
        if(m_hAesKey == NULL)
		{
            SetLastError((DWORD)NTE_BAD_KEY);
            throw WinAESException("Decrypt(1): Key is not valid");
        }

        // sanity check
        if(buffer == NULL)
		{
            SetLastError(ERROR_INVALID_USER_BUFFER);
            throw WinAESException("Decrypt(1): Buffer is NULL");
        }

        // sanity check
        if(!(csize % BLOCKSIZE == 0))
		{
            SetLastError((DWORD)NTE_BAD_DATA);
            throw WinAESException("Decrypt(1): Data size is not a multple of block size");
        }

        // sanity check
        unsigned int s = 0;
        if(MaxPlainTextSize(csize, s) && !(bsize > s-BLOCKSIZE))
		{
            SetLastError((DWORD)NTE_BUFFER_TOO_SMALL);
            throw WinAESException("Decrypt(1): Buffer is too small");
        }

        // sanity check
        if(m_bHaveIv == false)
		{
            SetLastError((DWORD)NTE_INVALID_PARAMETER);
            throw WinAESException("Decrypt(1): IV has not been set");
        }

        // Temporary for API
        DWORD d = (DWORD)csize;
        if(CryptDecrypt(m_hAesKey, NULL, TRUE, 0, buffer, &d) == FALSE)
		{
            // Build a useful message
            ostringstream emessage;
            emessage << "Decrypt(1): CryptDecrypt failed - " ;
            emessage << ErrorToDefine( GetLastError() );
            emessage << " (0x" << std::hex << GetLastError() << ")";

            psize = 0;
            throw WinAESException(emessage.str().c_str());
        }

        psize = d;
		result = true;
    }
    catch(const WinAESException& /*e*/)
    {
        if(m_nFlags & THROW_EXCEPTION)
		{
            throw;
        }
        result = false;
    }

    return result;
}

// Encrypt plaintext. psize is the size of the plaintext.  If successful, csize is the size of the ciphertext.
bool WinAES::Encrypt( const unsigned char* plaintext, /*In*/unsigned int psize, /*InOut*/unsigned char* ciphertext, /*InOut*/ unsigned int& csize )
{

#ifdef _DEBUG
    // Test the pointer (it is purported good). Not safe for Release builds
	if(IsBadReadPtr(plaintext, psize) == TRUE)
		throw WinAESException("Encrypt: IsBadWritePtr = True");

	if(IsBadReadPtr(ciphertext, csize) == TRUE)
		throw WinAESException("Encrypt: IsBadWritePtr = True");
#endif

    // Returned to caller (if non-//throwing)
    bool result = false;

    try
    {
        // sanity check
        if(!(plaintext != NULL || (plaintext == NULL && 0 == psize)))
		{
            SetLastError( ERROR_INVALID_USER_BUFFER );
            throw WinAESException( "Encrypt: Plain text buffer is not valid" );
        }

        // sanity check
        if(ciphertext == NULL)
		{
            SetLastError( ERROR_INVALID_USER_BUFFER );
            throw WinAESException( "Encrypt: Cipher text buffer is not valid" );
        }

        // Buffers cannot overlap
        if( !(((unsigned int)ciphertext+csize < (unsigned int)plaintext) || ((unsigned int)plaintext+psize < (unsigned int)ciphertext) ) )
        {
            SetLastError( (DWORD)NTE_BUFFERS_OVERLAP );
            throw WinAESException( "Encrypt: Buffers overlap" );
        }

        errno_t err = memcpy_s( ciphertext, csize, plaintext, psize );
        if(err != 0)
		{
            throw WinAESException( "Encrypt: Unable to prepare plaintext buffer" );
        }
				
        if(!Encrypt(ciphertext, csize, psize, csize))
		{
            // Build a useful message
            ostringstream emessage;
            emessage << "Encrypt: Encryption failed - " ;
            emessage << ErrorToDefine( GetLastError());
            emessage << " (0x" << std::hex << GetLastError() << ")";

            csize = 0;
            throw WinAESException(emessage.str().c_str());
        }

		//cipher has been built now append iv
		std::string ct("",csize);
		
		//copy ciphertext
		for(unsigned int i=0; i<csize; i++)
			ct[i] = ciphertext[i];

		//make room at the end for the iv we are appending
		unsigned int origSize = (int)ct.size();
		ct.resize(ct.size()+BLOCKSIZE);
		
		//add iv to end of cipher
		for(int i=0; i<BLOCKSIZE; i++)
			ct[origSize+i] = IV[i];
		
		csize = (int)ct.size(); //just a formality and not necessary

		//reassign temp cipher to original
		for(unsigned int i=0; i<ct.size(); i++)
			ciphertext[i] = ct[i];

		result = true;
    }
    catch( const WinAESException& /*e*/ )
    {
        if( m_nFlags & THROW_EXCEPTION )
		{
            throw;
        }
        result = false;
    }

    return result;
}

// Decrypt plaintext. csize is the size of the ciphertext.  If successful, psize is the size of the plaintext.
bool WinAES::Decrypt( const unsigned char* ciphertext, /*In*/unsigned int csize, /*InOut*/unsigned char* plaintext, /*InOut*/ unsigned int& psize )
{
#ifdef _DEBUG
    // Test the pointer (it is purported good). Not safe for Release builds
	if(IsBadReadPtr(ciphertext, csize) == TRUE)
		throw WinAESException("Decrypt: IsBadWritePtr = True");

	if(IsBadReadPtr(plaintext, psize) == TRUE)
		throw WinAESException("Decrypt: IsBadWritePtr = True");
#endif

    // Returned to caller
    bool result = false;

	//get/set iv from end of string
	std::string iv("",BLOCKSIZE);
	int origSize = csize-BLOCKSIZE;

	//get iv
	for(int i=0; i<BLOCKSIZE; i++)
		iv[i] = ciphertext[origSize+i];

	//copy iv to member IV
	std::copy(iv.begin(),iv.end(), IV);

	//set iv from end of ciphertext
	SetIv(IV, BLOCKSIZE);

	//get just the ciphertext minus the iv
	unsigned char* temp = new unsigned char[origSize];
	for(int i=0; i<origSize; i++)
		temp[i] = ciphertext[i];

    try
    {
        // sanity check
        if(temp == NULL || plaintext == NULL)
		{
            SetLastError( ERROR_INVALID_USER_BUFFER );
            throw WinAESException( "Decrypt: Buffer is NULL" );
        }

        // Buffers cannot overlap
        if( !(((unsigned int)temp+origSize < (unsigned int)plaintext) || ((unsigned int)plaintext+psize < (unsigned int)temp) ) )
        {
            SetLastError( (DWORD)NTE_BUFFERS_OVERLAP );
            throw WinAESException( "Decrypt: Buffers overlap" );
        }

        errno_t err = memcpy_s( plaintext, psize, temp, origSize );
        if(err != 0)
		{
            throw WinAESException( "Decrypt: Unable to prepare decryption buffer" );
        }

        if(!Decrypt( plaintext, origSize, origSize, psize ))
		{
            // Build a useful message
            ostringstream emessage;
            emessage << "Decrypt: Decryption failed - " ;
            emessage << ErrorToDefine( GetLastError() );
            emessage << " (0x" << std::hex << GetLastError() << ")";

            psize = 0;
            throw WinAESException( emessage.str().c_str() );
        }
		result = true;
    }

    catch( const WinAESException& /*e*/ )
    {
        if( m_nFlags & THROW_EXCEPTION )
		{
            throw;
        }
        result = false;
    }

    return result;
}

const char* WinAES::ErrorToDefine( DWORD dwError )
{
    switch(dwError)
    {
    case ERROR_ACCESS_DENIED:
        return "ERROR_ACCESS_DENIED";  
    case ERROR_INVALID_HANDLE:
        return "ERROR_INVALID_HANDLE";  
    case ERROR_INVALID_PARAMETER:
        return "ERROR_INVALID_PARAMETER";
    case ERROR_DEV_NOT_EXIST:
        return "ERROR_DEV_NOT_EXIST";
    case NTE_BAD_HASH:
        return "NTE_BAD_HASH";        
    case NTE_BAD_HASH_STATE:
        return "NTE_BAD_HASH_STATE";
    case NTE_BAD_UID:
        return "NTE_BAD_UID";
    case NTE_BAD_KEY:
        return "NTE_BAD_KEY";
    case NTE_BAD_LEN:
        return "NTE_BAD_LEN";
    case NTE_BAD_DATA:
        return "NTE_BAD_DATA";
    case NTE_BAD_VER:
        return "NTE_BAD_VER";
    case NTE_BAD_ALGID:
        return "NTE_BAD_ALGID";
    case NTE_BAD_FLAGS:
        return "NTE_BAD_FLAGS";
    case NTE_BAD_TYPE:
        return "NTE_BAD_TYPE";
    case NTE_BAD_KEY_STATE:
        return "NTE_BAD_KEY_STATE";
    case NTE_NO_KEY:
        return "NTE_NO_KEY";
    case NTE_NO_MEMORY:
        return "NTE_NO_MEMORY";
    case NTE_EXISTS:
        return "NTE_EXISTS";
    case NTE_PERM:
        return "NTE_PERM";
    case NTE_NOT_FOUND:
        return "NTE_NOT_FOUND";
    case NTE_DOUBLE_ENCRYPT:
        return "NTE_DOUBLE_ENCRYPT";
    case NTE_BAD_PROVIDER:
        return "NTE_BAD_PROVIDER";
    case NTE_BAD_PROV_TYPE:
        return "NTE_BAD_PROV_TYPE";
    case NTE_BAD_KEYSET:
        return "NTE_BAD_KEYSET";
    case NTE_PROV_TYPE_NOT_DEF:
        return "NTE_PROV_TYPE_NOT_DEF";
    case NTE_PROV_TYPE_ENTRY_BAD:
        return "NTE_PROV_TYPE_ENTRY_BAD";
    case NTE_KEYSET_NOT_DEF:
        return "NTE_KEYSET_NOT_DEF";
    case NTE_KEYSET_ENTRY_BAD:
        return "NTE_KEYSET_ENTRY_BAD";
    case NTE_BAD_KEYSET_PARAM:
        return "NTE_BAD_KEYSET_PARAM";
    case NTE_FAIL:
        return "NTE_FAIL";
    case NTE_SYS_ERR:
        return "NTE_SYS_ERR";
    case NTE_SILENT_CONTEXT:
        return "NTE_SILENT_CONTEXT";
    case NTE_FIXEDPARAMETER:
        return "NTE_FIXEDPARAMETER";
    case NTE_INVALID_HANDLE:
        return "NTE_INVALID_HANDLE";
    case NTE_INVALID_PARAMETER:
        return "NTE_INVALID_PARAMETER";
    case NTE_BUFFER_TOO_SMALL:
        return "NTE_BUFFER_TOO_SMALL";
    case NTE_BUFFERS_OVERLAP:
        return "NTE_BUFFERS_OVERLAP";
    case NTE_DECRYPTION_FAILURE:
        return "NTE_DECRYPTION_FAILURE";
    case NTE_INTERNAL_ERROR:
        return "NTE_INTERNAL_ERROR";
    default: 
		return "Unknown";
    }

    return "Unknown";
}