#pragma once

#include <wincrypt.h>
#include <exception>
#include <sstream>
#include <string>
#include "rsg.h"

// Vanilla exception class
class WinAESException : public std::exception
{
public:
    WinAESException(const char* message) : exception(message) { };
};

class WinAES
{
public:
    enum Parameters { KEYSIZE_128 = 16, KEYSIZE_192 = 24, KEYSIZE_256 = 32, BLOCKSIZE = 16 };
	enum Flags { CREATE_CONTAINER = 1, DELETE_CONTAINER = 8, THROW_EXCEPTION = 16, DEFAULT_FLAGS = CREATE_CONTAINER | DELETE_CONTAINER };
public:
    explicit WinAES(const wchar_t* lpszContainer=NULL, int nFlags=DEFAULT_FLAGS);
    explicit WinAES(HCRYPTPROV hProvider);

    virtual ~WinAES();

	// Generate a random block of unsigned chars using the underlying CSP.
    bool GenerateRandom(unsigned char* buffer, int size);

	// hard coded password used to generate key and iv
	bool GenerateDefaults();

    // Sets the Key and IV. The key must be of size KEYSIZE_128, KEYSIZE_192, or KEYSIZE_256. The IV must be of size BLOCKSIZE.
    bool SetKeyWithIv(const unsigned char* key, int ksize, const unsigned char* iv, int vsize=BLOCKSIZE);

    // Sets the Key. The key must be of size KEYSIZE_128,
    // KEYSIZE_192, or KEYSIZE_256. After calling SetKey,
    // for intial keying or re-keying, call SetIv.
    bool SetKey(const unsigned char* key, int ksize = KEYSIZE_128);

    // Sets the IV. The IV must be of size BLOCKSIZE. Call anytime to syncronize the IV under a key.
    bool SetIv(const unsigned char* iv, int vsize=BLOCKSIZE);

    // Returns the maximum size of the ciphertext, which includes padding for the plaintext.
    bool MaxCipherTextSize(unsigned int psize, unsigned int& csize);

    // Returns the maximum size of the plaintext, which includes removal of padding on the plaintext.
    bool MaxPlainTextSize(unsigned int csize, unsigned int& psize);

    // Encrpyt a buffer in-place. bsize is the size of the buffer,
    //  psize is the size of the plaintext. If successful,
    //  csize is the size of the ciphertext. On entry, bsize >= csize.
    bool Encrypt(/*InOut*/unsigned char* buffer, /*In*/unsigned int bsize, /*In*/unsigned int psize, /*Out*/ unsigned int& csize);

    // Decrpyt a buffer in-place. bsize is the size of the buffer, csize is the size of the ciphertext. If successful,
    //  psize is the size of the recovered plaintext.  On entry, bsize >= psize.
    bool Decrypt(/*InOut*/unsigned char* buffer, /*In*/unsigned int bsize, /*In*/unsigned int csize, /*Out*/ unsigned int& psize);

    // Encrypt plaintext. psize is the size of the plaintext.  If successful, csize is the size of the ciphertext.
    bool Encrypt(/*In*/const unsigned char* plaintext, /*In*/unsigned int psize, /*InOut*/unsigned char* ciphertext, /*InOut*/ unsigned int& csize);

    // Decrypt plaintext. csize is the size of the ciphertext.  If successful, psize is the size of the plaintext.
    bool Decrypt(/*In*/const unsigned char* ciphertext, /*In*/unsigned int csize, /*InOut*/unsigned char* plaintext, /*InOut*/ unsigned int& psize);

    // Duplicate a context. The caller is responsible for a call to CryptReleaseContext
    HCRYPTPROV DuplicateContext();

    // Convenience - Converts the value of GetLastError to a #define for easy web searching. No use to users.
    static const char* ErrorToDefine(DWORD dwError);

protected:
    bool AcquireContext(const wchar_t* lpszContainer);

protected:
    typedef struct _AesKey
    {
        BLOBHEADER Header;
        DWORD dwKeyLength;
        BYTE cbKey[KEYSIZE_256];

        _AesKey()
		{
            ZeroMemory( this, sizeof(_AesKey) );
            Header.bType = PLAINTEXTKEYBLOB;
            Header.bVersion = CUR_BLOB_VERSION;
            Header.reserved = 0;
        }

        ~_AesKey() { SecureZeroMemory( this, sizeof(this) ); }
    } AesKey;

private:
    WinAES(WinAES&) { };

private:
    HCRYPTPROV m_hProvider;
    HCRYPTKEY m_hAesKey;
    wchar_t* m_wszContainer;
    bool m_bHaveIv;
    int m_nFlags;

    // Used to destroy a temporary keyset
    enum { INVALID_INDEX = -1 };
    int m_nIndex;

	RandomStringGenerator rsg;
	unsigned char KEY[KEYSIZE_256];
	unsigned char IV[BLOCKSIZE];
};

typedef struct PROV_PARAMS_T
{
    const WCHAR* lpwsz;
    DWORD dwType;
    DWORD dwFlags;
} PROV_PARAMS, PPROV_PARAMS;

typedef struct PROVIDERS_T
{
    PROV_PARAMS params;
} PROVIDERS, PPROVIDERS;

// To get an AES provider, we *cannot* use the Base, Strong, or Enhanced providers since AES is not available.
// http://msdn.microsoft.com/en-us/library/aa375599(VS.85).aspx
// http://msdn.microsoft.com/en-us/library/aa382020(VS.85).aspx
const PROVIDERS AesProviders[] = 
{
    // http://msdn.microsoft.com/en-us/library/aa375545(VS.85).aspx
    { MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0 },
    { MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_NEWKEYSET },
    { MS_ENH_RSA_AES_PROV_XP, PROV_RSA_AES, 0 },
    { MS_ENH_RSA_AES_PROV_XP, PROV_RSA_AES, CRYPT_NEWKEYSET },
};