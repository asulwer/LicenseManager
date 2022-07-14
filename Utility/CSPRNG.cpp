///////////////////////////////////////////////
//
// **************************
// ** ENGLISH - 07/07/2012 **
//
// Project/Software name: libObfuscate v2.00
// Author: "Eng. Cosimo Oliboni" <oliboni@embeddedsw.net>
// Company: EmbeddedSw.net, Italy
//
// THIS IS A FREE SOFTWARE
//
// This software is released under GNU LGPL:
//
// * LGPL 3.0 <http://www.gnu.org/licenses/lgpl.html>
//
// You’re free to copy, distribute and make commercial use
// of this software under the following conditions:
//
// * You have to cite the author (and copyright owner): Eng. Cosimo Oliboni
// * You have to provide a link to the author’s Homepage: <http://embeddedsw.net/>
//
///////////////////////////////////////////////

#include "stdafx.h"

#include "crc32.h"
#include "keccak.h"
#include "md5.h"
#include "sha1.h"
#include "sha3.h"
#include "sha256.h"

#include "MultiBase.h"
#include "CSPRNG_data.h"

void CSPRNG_set_seed(CSPRNG_DATA *pCd,ENUM_HASH hashE,const unsigned char *passw,unsigned long nonce)
{
	unsigned char inBuf[64];
	unsigned char hash[64];
	unsigned long len = MAX_PASSW_SIZE;

	memset(pCd,0,sizeof(CSPRNG_DATA));

	memset(inBuf,0,64);
	memcpy(inBuf,passw,MAX_PASSW_SIZE);
	
	//password could be less than max
	if(len < MAX_PASSW_SIZE)
		len = strlen((char*)inBuf); //gives a warning but returns the correct size
	
	memcpy(&inBuf[len],&nonce, sizeof(unsigned long));
	len += sizeof(unsigned long);

	//hash password using 1 of 5 hashing algorithms
	if (hashE == CRC32_HASH)
	{
		CRC32 crc;
		std::string sHash = crc(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if(hashE== KECCAK_224_HASH)
	{
		Keccak k(Keccak::Keccak224);
		std::string sHash = k(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if (hashE == KECCAK_256_HASH)
	{
		Keccak k(Keccak::Keccak256);
		std::string sHash = k(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if (hashE == KECCAK_384_HASH)
	{
		Keccak k(Keccak::Keccak384);
		std::string sHash = k(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if (hashE == KECCAK_512_HASH)
	{
		Keccak k(Keccak::Keccak512);
		std::string sHash = k(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if(hashE==MD5_HASH)
	{
		MD5 md5;
		std::string sHash = md5(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if(hashE==SHA1_HASH)
	{
		SHA1 sha;
		std::string sHash = sha(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if(hashE == SHA3_224_HASH)
	{
		SHA3 sha(SHA3::Bits224);
		std::string sHash = sha(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if (hashE == SHA3_256_HASH)
	{
		SHA3 sha(SHA3::Bits256);
		std::string sHash = sha(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if (hashE == SHA3_384_HASH)
	{
		SHA3 sha(SHA3::Bits384);
		std::string sHash = sha(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if (hashE == SHA3_512_HASH)
	{
		SHA3 sha(SHA3::Bits512);
		std::string sHash = sha(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}
	else if (hashE == SHA256_HASH)
	{
		SHA256 sha;
		std::string sHash = sha(inBuf, 64);
		std::copy(sHash.begin(), sHash.end(), hash);
	}

	//set key using hashed password
	Multi_single_setkey(&pCd->msd,RIJNDAEL_ALG,hash);
	memcpy(pCd->ctrBuf, hash, 64);

	pCd->availCount=0;	
}

void CSPRNG_encrypt(MULTI_STATIC_DATA* msd, unsigned char* data, int len)
{
	int tlen = len;

	while (tlen >= len)
	{
		unsigned char tmpIN[DATA_BLOCK_SIZE];
		memcpy(tmpIN, data, DATA_BLOCK_SIZE);

		Multi_ECB_single_encrypt(msd, RIJNDAEL_ALG, tmpIN, data);

		data += DATA_BLOCK_SIZE;
		tlen -= DATA_BLOCK_SIZE;
	}
}

unsigned char CSPRNG_get_uc(CSPRNG_DATA *pCd)
{
	if (!pCd->availCount)
	{
		// random = AES-256(CTR)
		memcpy(pCd->randBuf, pCd->ctrBuf, 64);
		CSPRNG_encrypt(&pCd->msd, pCd->randBuf, sizeof(pCd->randBuf));
		CSPRNG_encrypt(&pCd->msd, pCd->ctrBuf, sizeof(pCd->ctrBuf));

		pCd->availCount=DATA_BLOCK_SIZE-1; //always 15

		return(*pCd->randBuf);
	}
	else
	{
		return(pCd->randBuf[DATA_BLOCK_SIZE-(pCd->availCount--)]);
	}
}

void CSPRNG_array_init(CSPRNG_DATA* pCd, unsigned long max, unsigned char* buf)
{
	bool numExists[DATA_BLOCK_SIZE] = { false };

	int i = 0;
	while (i != max)
	{
		int randomNum = (CSPRNG_get_uc(pCd) % max) % max;

		if (numExists[randomNum] == false)
		{
			buf[i++] = (unsigned char)randomNum;
			numExists[randomNum] = true;
		}
	}
}