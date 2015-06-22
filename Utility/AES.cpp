#include "stdafx.h"
#include "WinAES.h"
#include "Utility.h"

namespace Utility
{
	License::AES::AES()
	{
		aes = new WinAES();
		deflate = gcnew Deflate();
		aes->GenerateDefaults();
	}

	License::AES::~AES()
	{
		delete deflate;
		this->!AES();
	}

	License::AES::!AES()
	{
		delete aes;
	}
	
	String^ License::AES::Encrypt(array<Byte>^ b)
	{
		array<Byte>^ buffer = deflate->Compress(b);

		array<Byte>^ ct;
		UInt32 cs = 0;

		if(MaxCipherTextSize(buffer->Length, cs))
		{
			ct = gcnew array<Byte>(cs);
		}

		pin_ptr<unsigned char> pn = &buffer[0];
		pin_ptr<unsigned char> cn = &ct[0];

		String^ retResult = nullptr;
		if(aes->Encrypt(pn, buffer->Length,cn,cs))
		{
			retResult = Convert::ToBase64String(ct);
		}

		return retResult;
	}

	array<Byte>^ License::AES::Decrypt(String^ orig)
	{
		array<Byte>^ buffer = Convert::FromBase64String(orig);
		
		array<Byte>^ rt;
		UInt32 rs = 0;

		if(MaxPlainTextSize(buffer->Length, rs))
		{
			rt = gcnew array<Byte>(rs);
		}

		if((buffer != nullptr && rt != nullptr) && (buffer->Length > 0 && rt->Length > 0))
		{
			pin_ptr<unsigned char> cn = &buffer[0];
			pin_ptr<unsigned char> pn = &rt[0];

			if(aes->Decrypt(cn,buffer->Length,pn,rs))
			{
#if (_MSC_VER != 1800) //this causes an error in VS2013 so its unecessary
				Array::Resize(rt,rt->Length - rt[rt->Length-1]);
#endif
				buffer = deflate->Decompress(rt);
			}
			else
			{
				buffer = nullptr;
			}
		}
		else
		{
			buffer = nullptr;
		}

		return buffer;
	}

	bool License::AES::GenerateRandom(array<Byte> ^buffer, int size)
	{
		pin_ptr<unsigned char> buf = &buffer[0];

		return aes->GenerateRandom(buf,size);
	}
	
	bool License::AES::GenerateDefaults()
	{
		return aes->GenerateDefaults();
	}
	
	bool License::AES::SetKeyWithIv(array<Byte> ^key, int ksize, array<Byte> ^iv)
	{
		return SetKeyWithIv(key,ksize,iv,(int)Parameters::BLOCKSIZE);
	}

	bool License::AES::SetKeyWithIv(array<Byte> ^key, int ksize, array<Byte> ^iv, int vsize)
	{
		pin_ptr<unsigned char> Key = &key[0];
		pin_ptr<unsigned char> IV = &iv[0];

		return aes->SetKeyWithIv(Key,ksize,IV,vsize);
	}
	
	bool License::AES::MaxCipherTextSize(UInt32 psize, UInt32% csize)
	{
		UInt32 cs = 0;
		bool result = aes->MaxCipherTextSize(psize, cs);
		csize = cs;
		return result;
	}

	bool License::AES::MaxPlainTextSize(UInt32 csize, UInt32% psize)
	{
		UInt32 ps = 0;
		bool result = aes->MaxPlainTextSize(csize, ps);
		psize = ps;
		return result;
	}
}