#include "stdafx.h"
#include "Encryption.h"

namespace Utility
{
	Encryption::Encryption()
	{
		md = new MULTI_DATA;
	}

	Encryption::~Encryption()
	{
		this->!Encryption();
	}

	Encryption::!Encryption()
	{
		delete md;
	}
	
	cli::array<Byte>^ Encryption::Encrypt(cli::array<Byte>^ orig)
	{
		String^ passwd1 = "PaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRd1234567"; //default password
		String^ passwd2 = "pAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrD1234567"; //default password

		cli::array<unsigned char>^ pw_bytes1 = System::Text::Encoding::ASCII->GetBytes(passwd1);
		cli::array<unsigned char>^ pw_bytes2 = System::Text::Encoding::ASCII->GetBytes(passwd2);

		pin_ptr<unsigned char> pw1 = &pw_bytes1[0];
		pin_ptr<unsigned char> pw2 = &pw_bytes2[0];

		pin_ptr<unsigned char> pn = &orig[0]; //temp plaintext

		BYTE* iv = new BYTE[256]{ '\0' }; //initialize to nothing
		Multi_setkey(md, iv, pw1, pw2, 0);
		Multi_CBC_encrypt(md, orig->Length, pn, NULL, NULL, NULL, NULL);

		return orig;
	}

	cli::array<Byte>^ Encryption::Decrypt(cli::array<Byte>^ orig)
	{	
		String^ passwd1 = "PaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRd1234567"; //default password
		String^ passwd2 = "pAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrD1234567"; //default password

		cli::array<unsigned char>^ pw_bytes1 = System::Text::Encoding::ASCII->GetBytes(passwd1);
		cli::array<unsigned char>^ pw_bytes2 = System::Text::Encoding::ASCII->GetBytes(passwd2);

		pin_ptr<unsigned char> pw1 = &pw_bytes1[0];
		pin_ptr<unsigned char> pw2 = &pw_bytes2[0];

		pin_ptr<unsigned char> pn = &orig[0]; //temp plaintext

		BYTE* iv = new BYTE[256]{ '\0' }; //initialize to nothing
		Multi_setkey(md, iv, pw1, pw2, 0);		
		Multi_CBC_decrypt(md, orig->Length, pn, NULL, NULL, NULL, NULL);

		return orig;
	}
}