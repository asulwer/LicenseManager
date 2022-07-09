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

#include "CSPRNG.h"
#include "Scramble_data.h"

void Scramble_build_list(CSPRNG_DATA *pCd,const unsigned long count,const unsigned long max,unsigned long *list)
{
	unsigned long tCount=count;

	while(tCount)
	{
		do
		{
			list[0]= CSPRNG_get_ul(pCd)%max;
			list[1]= CSPRNG_get_ul(pCd)%max;
		} while(list[0]==list[1]);

		list+=2;
		tCount--;
	}
}

OBFUNC_RETV Scramble_seed(SCRAMBLE_DATA *pSd,const unsigned long len,const unsigned char *passw,unsigned long nonce)
{
	memset(pSd,0,sizeof(SCRAMBLE_DATA));

	// CSPRNG <- Skein512(passw + nonce)
	CSPRNG_set_seed(&pSd->cd,SKEIN512_HASH,passw,nonce);

	//pSd->len = len; //put back in IF we are doing a divisable block of 16 bytes at a time (ex, 16, 32, 48, etc)
	pSd->len = len - (len % 16); //remove the ending partial block IF it exists
	
	// 50% scramble : (len/2)*2*sizeof(unsigned long)
	if(pSd->list=(unsigned long *) malloc(len*sizeof(unsigned long)))
	{
		Scramble_build_list(&pSd->cd, pSd->len >> 1, pSd->len, pSd->list);
		return(OBFUNC_OK);
	}
	else
	{ 
		return(OBFUNC_OUTOFMEM); 
	}
}

void Scramble_end(SCRAMBLE_DATA *pSd)
{
	TEST_AND_FREE(pSd->list)
}

#define	REFRESH_COUNTDOWN	10000

OBFUNC_RETV Seg_scramble(SCRAMBLE_DATA *pSd,unsigned char *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc)
{
	unsigned long	tot=pSd->len>>1;	// 50%
	unsigned long	*pL=pSd->list;
	unsigned long	index;		
	unsigned char	lastPerc=0;
	unsigned short	refCount=REFRESH_COUNTDOWN;

	for(index=0;index<tot;index++)
	{
		unsigned long	idx0=*(pL++);
		unsigned long	idx1=*(pL++);
		unsigned char	tmp;

		tmp=buf[idx0];
		buf[idx0]=buf[idx1];
		buf[idx1]=tmp;

		if(!refCount)
		{
			refCount=REFRESH_COUNTDOWN;

			if(pFunc)
			{
				tmp=(unsigned char) ((((float) index)/((float) tot))*((float) 100));
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

OBFUNC_RETV Seg_descramble(SCRAMBLE_DATA *pSd,unsigned char *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc)
{
	unsigned long	tot=pSd->len>>1;	// 50%
	unsigned long	*pL=&pSd->list[pSd->len-1];
	unsigned long	index;		
	unsigned char	lastPerc=0;
	unsigned short	refCount=REFRESH_COUNTDOWN;

	for(index=0;index<tot;index++)
	{
		unsigned long	idx1=*(pL--);
		unsigned long	idx0=*(pL--);
		unsigned char	tmp;

		tmp=buf[idx0];
		buf[idx0]=buf[idx1];
		buf[idx1]=tmp;

		if(!refCount)
		{
			refCount=REFRESH_COUNTDOWN;

			if(pFunc)
			{
				tmp=(unsigned char) ((((float) index)/((float) tot))*((float) 100));
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
