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

#ifndef MULTI_H
#define	MULTI_H

// ** Thread-safe implementation

// ** Wrapping of implemented ciphers
// ** 128bit block size	(DATA_BLOCK_SIZE<<3)
// ** 2x 256bit keys	(MAX_PASSW_SIZE<<3)
// ** Cypher-Block-Chaining (CBC)

// "iv" = Inizialization Vectors (IVs), byte[MAX_ALG][DATA_BLOCK_SIZE]
// "passw1" = password1, byte[MAX_PASSW_SIZE]
// "passw2" = password2, byte[MAX_PASSW_SIZE]
// "nonce" = cryptographic nonce (number used once)

#include "Multi_data.h"

extern	void Multi_setkey(MULTI_DATA *pMd,const unsigned char *iv,const unsigned char *passw1,const unsigned char *passw2,unsigned long nonce);

extern	OBFUNC_RETV Multi_CBC_encrypt(MULTI_DATA *pMd,const unsigned long len,unsigned char *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
extern	OBFUNC_RETV Multi_CBC_decrypt(MULTI_DATA *pMd,const unsigned long len,unsigned char *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);

#endif
