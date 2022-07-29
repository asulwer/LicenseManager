#include "stdafx.h"
#include "Encryption.h"

namespace Utility
{
	Encryption::Encryption()
	{
		md = new MULTI_DATA;
		sd = new SCRAMBLE_DATA;
		nonce = 1;
	} 

	Encryption::~Encryption()
	{
		this->!Encryption();
	}

	Encryption::!Encryption()
	{
		delete sd;
		delete md;
	}
	
	cli::array<Byte>^ Encryption::Encrypt(cli::array<Byte>^ orig)
	{
		String^ sPasswd1 = "PaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRd"; //default password
		String^ sPasswd2 = "pAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrD"; //default password
		String^ sPasswd3 = "abcdefgh01234567ABCDEFG76543210"; //default password
		String^ sIv = "ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567"; //default iv

		cli::array<unsigned char>^ pw_bytes1 = System::Text::Encoding::ASCII->GetBytes(sPasswd1);
		cli::array<unsigned char>^ pw_bytes2 = System::Text::Encoding::ASCII->GetBytes(sPasswd2);
		cli::array<unsigned char>^ pw_bytes3 = System::Text::Encoding::ASCII->GetBytes(sPasswd3);
		cli::array<unsigned char>^ iv_bytes = System::Text::Encoding::ASCII->GetBytes(sIv);

		pin_ptr<unsigned char> pw1 = &pw_bytes1[0];
		pin_ptr<unsigned char> pw2 = &pw_bytes2[0];
		pin_ptr<unsigned char> pw3 = &pw_bytes3[0];
		pin_ptr<unsigned char> iv = &iv_bytes[0];

		pin_ptr<unsigned char> pt = &orig[0]; //plaintext

		Multi_setkey(md, iv, pw1, pw2, nonce);
		Multi_CBC_encrypt(md, orig->Length, pt, NULL, NULL, NULL, NULL);
		Scramble_seed(sd, orig->Length, pw3, nonce); //should do a 16 byte block at a time
		Seg_scramble(sd, pt, NULL, NULL, NULL, NULL);

		return orig;
	}

	cli::array<Byte>^ Encryption::Decrypt(cli::array<Byte>^ orig)
	{	
		String^ sPasswd1 = "PaSsWoRdPaSsWoRdPaSsWoRdPaSsWoRd"; //default password
		String^ sPasswd2 = "pAsSwOrDpAsSwOrDpAsSwOrDpAsSwOrD"; //default password
		String^ sPasswd3 = "abcdefgh01234567ABCDEFG76543210"; //default password
		String^ sIv = "ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567ABCDEDGH01234567"; //default iv

		cli::array<unsigned char>^ pw_bytes1 = System::Text::Encoding::ASCII->GetBytes(sPasswd1);
		cli::array<unsigned char>^ pw_bytes2 = System::Text::Encoding::ASCII->GetBytes(sPasswd2);
		cli::array<unsigned char>^ pw_bytes3 = System::Text::Encoding::ASCII->GetBytes(sPasswd3);
		cli::array<unsigned char>^ iv_bytes = System::Text::Encoding::ASCII->GetBytes(sIv);

		pin_ptr<unsigned char> pw1 = &pw_bytes1[0];
		pin_ptr<unsigned char> pw2 = &pw_bytes2[0];
		pin_ptr<unsigned char> pw3 = &pw_bytes3[0];
		pin_ptr<unsigned char> iv = &iv_bytes[0];

		pin_ptr<unsigned char> ct = &orig[0]; //cyphertext

		Scramble_seed(sd, orig->Length, pw3, nonce); //should do a 16 byte block at a time
		Seg_descramble(sd, ct, NULL, NULL, NULL, NULL);
		Multi_setkey(md, iv, pw1, pw2, nonce);
		Multi_CBC_decrypt(md, orig->Length, ct, NULL, NULL, NULL, NULL);
		
		return orig;
	}
}