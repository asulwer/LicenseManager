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

#ifndef SCRAMBLE_H
#define SCRAMBLE_H

// ** Thread-safe implementation

// ** Scramble data segment
// ** Based on CSPRNG

// "passw" = password, byte[MAX_PASSW_SIZE]
// "nonce" = cryptographic nonce (number used once)

#include "Scramble_data.h"

extern DECLDIR	OBFUNC_RETV Scramble_seed(SCRAMBLE_DATA *pSd,const DWORD len,const BYTE *passw,DWORD nonce);
extern DECLDIR	void Scramble_end(SCRAMBLE_DATA *pSd);

extern DECLDIR	OBFUNC_RETV Seg_scramble(SCRAMBLE_DATA *pSd,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
extern DECLDIR	OBFUNC_RETV Seg_descramble(SCRAMBLE_DATA *pSd,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);

#endif
