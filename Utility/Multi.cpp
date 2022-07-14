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

#include "MultiBase.h"
#include "CSPRNG.h"

#include "Multi_data.h"

void BlockXor(unsigned char *data,const unsigned char *value)
{
	#if	DATA_BLOCK_SIZE==16
	*((unsigned long *) (data+ 0))	^= *((unsigned long *) (value+ 0));
	*((unsigned long *) (data+ 4))	^= *((unsigned long *) (value+ 4));
	*((unsigned long *) (data+ 8))	^= *((unsigned long *) (value+ 8));
	*((unsigned long *) (data+12))	^= *((unsigned long *) (value+12));
	#else
	#error
	#endif
}

void Multi_setkey(MULTI_DATA *pMd,const unsigned char *iv,const unsigned char *passw1,const unsigned char *passw2,unsigned long nonce)
{
	unsigned char passw[MAX_ALG][MAX_PASSW_SIZE];
	unsigned char usedMap[MAX_ALG];
	
	memset(pMd,0,sizeof(MULTI_DATA));
	
	CSPRNG_set_seed(&pMd->cd,SHA256_HASH,passw2,nonce);

	// IVs
	memcpy(pMd->iv,iv,MAX_ALG*DATA_BLOCK_SIZE);

	// passw[] <- KDF4 : random ( hash( passw1 + nonce) )
	for(int index=0;index<MAX_HASH;index++)
	{
		CSPRNG_DATA* tmpCSPRNG = new CSPRNG_DATA();

		switch(index)
		{
			case 0:	CSPRNG_set_seed(tmpCSPRNG, CRC32_HASH, passw1, nonce); break;
			case 1:	CSPRNG_set_seed(tmpCSPRNG, KECCAK_256_HASH, passw1, nonce); break;
			case 2:	CSPRNG_set_seed(tmpCSPRNG, KECCAK_384_HASH, passw1, nonce); break;
			case 3:	CSPRNG_set_seed(tmpCSPRNG, KECCAK_512_HASH, passw1, nonce); break;
			case 4:	CSPRNG_set_seed(tmpCSPRNG, SHA1_HASH, passw1, nonce); break;
			case 5:	CSPRNG_set_seed(tmpCSPRNG, SHA3_256_HASH, passw1, nonce); break;
			case 6:	CSPRNG_set_seed(tmpCSPRNG, SHA3_384_HASH, passw1, nonce); break;
			case 7:	CSPRNG_set_seed(tmpCSPRNG, SHA3_512_HASH, passw1, nonce); break;		
		}
		
		for(int pIndex=0; pIndex<(MAX_ALG/MAX_HASH); pIndex++) //16/8
		{
			for(int sIndex=0; sIndex<MAX_PASSW_SIZE; sIndex++) //32
			{
				int pos = (index * (MAX_ALG / MAX_HASH)) + pIndex;
				passw[pos][sIndex] = CSPRNG_get_uc(tmpCSPRNG);
			}
		}
	}

	CSPRNG_array_init(&pMd->cd,MAX_ALG,usedMap);

	// subcipher setup
	Multi_single_setkey(&pMd->msd,ANUBIS_ALG,		passw[usedMap[0]]);
	Multi_single_setkey(&pMd->msd,CAMELLIA_ALG,		passw[usedMap[1]]);
	Multi_single_setkey(&pMd->msd,CAST256_ALG,		passw[usedMap[2]]);
	Multi_single_setkey(&pMd->msd,CLEFIA_ALG,		passw[usedMap[3]]);
	Multi_single_setkey(&pMd->msd,FROG_ALG,			passw[usedMap[4]]);
	Multi_single_setkey(&pMd->msd,HIEROCRYPT3_ALG,	passw[usedMap[5]]);
	Multi_single_setkey(&pMd->msd,IDEANXT128_ALG,	passw[usedMap[6]]);
	Multi_single_setkey(&pMd->msd,MARS_ALG,			passw[usedMap[7]]);
	Multi_single_setkey(&pMd->msd,RC6_ALG,			passw[usedMap[8]]);
	Multi_single_setkey(&pMd->msd,RIJNDAEL_ALG,		passw[usedMap[9]]);
	Multi_single_setkey(&pMd->msd,SAFERP_ALG,		passw[usedMap[10]]);
	Multi_single_setkey(&pMd->msd,SC2000_ALG,		passw[usedMap[11]]);
	Multi_single_setkey(&pMd->msd,SERPENT_ALG,		passw[usedMap[12]]);
	Multi_single_setkey(&pMd->msd,SPEED_ALG,		passw[usedMap[13]]);
	Multi_single_setkey(&pMd->msd,TWOFISH_ALG,		passw[usedMap[14]]);
	Multi_single_setkey(&pMd->msd,UNICORNA_ALG,		passw[usedMap[15]]);
}

#define	REFRESH_COUNTDOWN 100

OBFUNC_RETV Multi_CBC_encrypt(MULTI_DATA *pMd,const unsigned long len,unsigned char *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc)
{
	unsigned long	tLen=len;
	unsigned char	lastPerc=0;
	unsigned short	refCount = REFRESH_COUNTDOWN;

	while(tLen>=DATA_BLOCK_SIZE)
	{
		unsigned char tmpIN[DATA_BLOCK_SIZE];
		ENUM_ALG curAlg = (ENUM_ALG)(CSPRNG_get_uc(&pMd->cd) % MAX_ALG);
		
		// OUT = encrypt( IN ^ IV_or_previous_out_block )
		memcpy(tmpIN,buf,DATA_BLOCK_SIZE);
		BlockXor(tmpIN,pMd->iv[curAlg]);

		Multi_ECB_single_encrypt(&pMd->msd,curAlg,tmpIN,buf);
		memcpy(pMd->iv[curAlg],buf,DATA_BLOCK_SIZE);

		buf+=DATA_BLOCK_SIZE;
		tLen-=DATA_BLOCK_SIZE;

		if(!refCount)
		{
			refCount=REFRESH_COUNTDOWN;

			if(pFunc)
			{
				unsigned char	tmp=(unsigned char) ((((float) (len-tLen))/((float) len))*((float) 100));
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

OBFUNC_RETV Multi_CBC_decrypt(MULTI_DATA *pMd,const unsigned long len,unsigned char *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc)
{
	unsigned long	tLen=len;
	unsigned char	lastPerc=0;
	unsigned short	refCount=REFRESH_COUNTDOWN;

	while(tLen>=DATA_BLOCK_SIZE)
	{
		unsigned char	tmpOUT[DATA_BLOCK_SIZE];
		ENUM_ALG curAlg = (ENUM_ALG)(CSPRNG_get_uc(&pMd->cd) % MAX_ALG);
		
		// OUT = decrypt( IN ) ^ IV_or_previous_in_block )
		Multi_ECB_single_decrypt(&pMd->msd,curAlg,buf,tmpOUT);

		BlockXor(tmpOUT,pMd->iv[curAlg]);
		memcpy(pMd->iv[curAlg],buf,DATA_BLOCK_SIZE);
		memcpy(buf,tmpOUT,DATA_BLOCK_SIZE);

		buf+=DATA_BLOCK_SIZE;
		tLen-=DATA_BLOCK_SIZE;

		if(!refCount)
		{
			refCount=REFRESH_COUNTDOWN;

			if(pFunc)
			{
				unsigned char	tmp=(unsigned char) ((((float) (len-tLen))/((float) len))*((float) 100));
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
