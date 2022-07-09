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

#define	RESEED_COUNTDOWN	1000

void CSPRNG_set_seed(CSPRNG_DATA *pCd,ENUM_HASH hashE,const unsigned char *passw,unsigned long nonce)
{
	unsigned char	inBuf[64];
	unsigned char	hash[64];
	unsigned long	len = MAX_PASSW_SIZE;

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
	memcpy(pCd->ctrBuf,&hash[MAX_PASSW_SIZE],DATA_BLOCK_SIZE);
	
	pCd->availCount=0;	
}

// little endian
void BlockInc(unsigned char *data, int len)
{
	unsigned int pos = len;
	while (pos-- && ++data[pos] == 0);
}

//TODOL change this behavior so its isnt needed anymore
unsigned char CSPRNG_get_uc(CSPRNG_DATA *pCd)
{
	if(!pCd->availCount)
	{
		// random = AES-256(CTR)
		Multi_ECB_single_encrypt(&pCd->msd,RIJNDAEL_ALG,pCd->ctrBuf,pCd->randBuf);
		BlockInc(pCd->ctrBuf, sizeof(pCd->ctrBuf));

		pCd->availCount=DATA_BLOCK_SIZE-1;

		return(*pCd->randBuf);
	}
	else
	{
		return(pCd->randBuf[DATA_BLOCK_SIZE-(pCd->availCount--)]);
	}
}

unsigned short CSPRNG_get_us(CSPRNG_DATA *pCd)
{
	unsigned short	retV;

	retV=(unsigned short)CSPRNG_get_uc(pCd);
	retV<<=8;
	retV|=(unsigned short)CSPRNG_get_uc(pCd);

	return(retV);
}

unsigned long CSPRNG_get_ul(CSPRNG_DATA *pCd)
{
	unsigned long	retV;

	retV=(unsigned long)CSPRNG_get_us(pCd);
	retV<<=16;
	retV|=(unsigned long)CSPRNG_get_us(pCd);

	return(retV);
}

#define	REFRESH_COUNTDOWN	1000

OBFUNC_RETV CSPRNG_randomize(CSPRNG_DATA *pCd,const unsigned long len,unsigned char *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc)
{
	unsigned long tLen=len;
	unsigned char lastPerc=0;
	unsigned short refCount=REFRESH_COUNTDOWN;

	while(tLen--)
	{
		*(buf++)= CSPRNG_get_uc(pCd);

		if(!refCount)
		{
			refCount=REFRESH_COUNTDOWN;

			if(pFunc)
			{
				unsigned char tmp=(unsigned char) ((((float) (len-tLen))/((float) len))*((float) 100));
				if(tmp>lastPerc)
				{
					lastPerc=tmp;
					pFunc(pDesc,lastPerc);
				}
			}
			if(tFunc&&tFunc(tDesc))
			{ 
				return(OBFUNC_STOP);
			}
		}

		refCount--;
	}

	return(OBFUNC_OK);
}

void CSPRNG_array_init(CSPRNG_DATA *pCd,unsigned long max,unsigned char *buf)
{
	unsigned long index;

	memset(buf,0xFF,max);
	for(index=0;index<max;index++)
	{
		unsigned long rIndex;

		do
		{ 
			rIndex= CSPRNG_get_uc(pCd)%max;
		} while(buf[rIndex]!=0xFF);

		buf[rIndex]=(unsigned char) index;
	}
}
