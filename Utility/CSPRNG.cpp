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

#include "Sha512.h"
#include "Grostl512.h"
#include "Keccak512.h"
#include "Skein512.h"
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

	//hash password using 1 of 4 hashing algorithms
	if(hashE==SHA512_HASH)
	{
		SHA512_DATA		sha;

		Sha512_init(&sha);
		Sha512_data(&sha,inBuf,len);
		Sha512_finalize(&sha,hash);
	}
	else if(hashE==GROSTL512_HASH)
	{
		GROSTL512_DATA	grostl;

		Grostl512_init(&grostl);
		Grostl512_data(&grostl,inBuf,len);
		Grostl512_finalize(&grostl,hash);
	}
	else if(hashE==KECCAK512_HASH)
	{
		KECCAK512_DATA	keccak;

		Keccak512_init(&keccak);
		Keccak512_data(&keccak,inBuf,len);
		Keccak512_finalize(&keccak,hash);
	}
	else if(hashE==SKEIN512_HASH)
	{
		SKEIN512_DATA	skein;

		Skein512_init(&skein);
		Skein512_data(&skein,inBuf,len);
		Skein512_finalize(&skein,hash);
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

		Multi_ECB_single_encrypt(msd, RIJNDAEL_ALG, tmpIN, data); //encrypts and returns the first 16 bytes

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