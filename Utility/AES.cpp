#include "stdafx.h"
#include "WinAES.h"
#include "Utility.h"

namespace Utility
{
	License::AES::AES()
	{
		aes = new WinAES();
		aes->GenerateDefaults();
	}

	License::AES::~AES()
	{
		this->!AES();
	}

	License::AES::!AES()
	{
		delete aes;
	}
	
	String^ License::AES::Encrypt(cli::array<Byte>^ b)
	{
		cli::array<Byte>^ ct;
		UInt32 cs = 0;

		if(MaxCipherTextSize(b->Length, cs))
		{
			ct = gcnew cli::array<Byte>(cs);
		}

		pin_ptr<unsigned char> pn = &b[0];
		pin_ptr<unsigned char> cn = &ct[0];

		String^ retResult = nullptr;
		if(aes->Encrypt(pn, b->Length,cn,cs))
		{
			retResult = Convert::ToBase64String(ct);
		}

		return retResult;
	}

	cli::array<Byte>^ License::AES::Decrypt(String^ orig)
	{
		cli::array<Byte>^ buffer = Convert::FromBase64String(orig);
		
		cli::array<Byte>^ rt;
		UInt32 rs = 0;

		if(MaxPlainTextSize(buffer->Length, rs))
		{
			rt = gcnew cli::array<Byte>(rs);
		}

		pin_ptr<unsigned char> cn = &buffer[0];
		pin_ptr<unsigned char> pn = &rt[0];

		if(((buffer != nullptr && rt != nullptr) && (buffer->Length > 0 && rt->Length > 0)) && aes->Decrypt(cn,buffer->Length,pn,rs))
		{
			buffer = rt;
		}
		else
		{
			buffer = nullptr;
		}

		return buffer;
	}

	bool License::AES::GenerateRandom(cli::array<Byte> ^buffer, int size)
	{
		pin_ptr<unsigned char> buf = &buffer[0];

		return aes->GenerateRandom(buf,size);
	}
	
	bool License::AES::GenerateDefaults()
	{
		return aes->GenerateDefaults();
	}
	
	bool License::AES::SetKeyWithIv(cli::array<Byte> ^key, int ksize, cli::array<Byte> ^iv)
	{
		return SetKeyWithIv(key,ksize,iv,(int)Parameters::BLOCKSIZE);
	}

	bool License::AES::SetKeyWithIv(cli::array<Byte> ^key, int ksize, cli::array<Byte> ^iv, int vsize)
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