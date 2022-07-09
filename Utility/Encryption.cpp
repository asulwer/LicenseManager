#include "stdafx.h"
#include "Encryption.h"

namespace Utility
{
	Encryption::Encryption()
	{
		md = new MULTI_DATA;
		sd = new SCRAMBLE_DATA;
		iv = new unsigned char[256]{ '\0' }; //initialize to nothing
	}

	Encryption::~Encryption()
	{
		this->!Encryption();
	}

	Encryption::!Encryption()
	{
		delete iv;
		delete sd;
		delete md;
	}
	
	cli::array<Byte>^ Encryption::Encrypt(cli::array<Byte>^ orig)
	{
		String^ passwd1 = "PaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRd"; //default password
		String^ passwd2 = "pAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrD"; //default password

		cli::array<unsigned char>^ pw_bytes1 = System::Text::Encoding::ASCII->GetBytes(passwd1);
		cli::array<unsigned char>^ pw_bytes2 = System::Text::Encoding::ASCII->GetBytes(passwd2);

		pin_ptr<unsigned char> pw1 = &pw_bytes1[0];
		pin_ptr<unsigned char> pw2 = &pw_bytes2[0];

		pin_ptr<unsigned char> pt = &orig[0]; //plaintext

		//Scramble_seed(sd, orig->Length, pw1, nonce); //should do a 16 byte block at a time
		//Seg_scramble(sd, pt, NULL, NULL, NULL, NULL);
		Multi_setkey(md, iv, pw1, pw2, nonce);
		Multi_CBC_encrypt(md, orig->Length, pt, NULL, NULL, NULL, NULL);
		
		return orig;
	}

	cli::array<Byte>^ Encryption::Decrypt(cli::array<Byte>^ orig)
	{	
		String^ passwd1 = "PaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRd"; //default password
		String^ passwd2 = "pAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrD"; //default password

		cli::array<unsigned char>^ pw_bytes1 = System::Text::Encoding::ASCII->GetBytes(passwd1);
		cli::array<unsigned char>^ pw_bytes2 = System::Text::Encoding::ASCII->GetBytes(passwd2);

		pin_ptr<unsigned char> pw1 = &pw_bytes1[0];
		pin_ptr<unsigned char> pw2 = &pw_bytes2[0];

		pin_ptr<unsigned char> ct = &orig[0]; //cyphertext

		Multi_setkey(md, iv, pw1, pw2, nonce);
		Multi_CBC_decrypt(md, orig->Length, ct, NULL, NULL, NULL, NULL);
		//Scramble_seed(sd, orig->Length, pw1, nonce); //should do a 16 byte block at a time
		//Seg_descramble(sd, ct, NULL, NULL, NULL, NULL);
		
		return orig;
	}
}