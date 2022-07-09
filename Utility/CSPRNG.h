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

#ifndef CSPRNG_H
#define CSPRNG_H

// ** Thread-safe implementation

// ** Cryptographically Secure Pseudo Random Number Generator (by construction)
// ** Based on AES-256 + CTR

// "hashE" = hashing function
// "passw" = password, byte[MAX_PASSW_SIZE]
// "nonce" = cryptographic nonce (number used once)

#include "CSPRNG_data.h"

extern	void CSPRNG_set_seed(CSPRNG_DATA *pCd,ENUM_HASH hashE,const unsigned char *passw,unsigned long nonce);
//extern	void CSPRNG_autoseed(CSPRNG_DATA *pCd,perc_callback_t backFunc,void *desc);

extern	unsigned char CSPRNG_get_byte(CSPRNG_DATA *pCd);
extern	unsigned short CSPRNG_get_word(CSPRNG_DATA *pCd);
extern	unsigned long CSPRNG_get_dword(CSPRNG_DATA *pCd);
extern	OBFUNC_RETV CSPRNG_randomize(CSPRNG_DATA *pCd,const unsigned long len,unsigned char *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
extern	void CSPRNG_array_init(CSPRNG_DATA *pCd,unsigned long max,unsigned char *buf);

#endif
