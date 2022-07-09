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

#ifndef CLEFIA_H
#define CLEFIA_H

// ** Thread-safe implementation

// ** Clefia cipher
// ** 128bit block size
// ** 256bit key

#include "Clefia_data.h"

extern	void Clefia_set_key(CLEFIA_DATA *pCd,const unsigned char *skey,const int key_bitlen);
extern	void Clefia_encrypt(const CLEFIA_DATA *pCd,const unsigned char *pt,unsigned char *ct);
extern	void Clefia_decrypt(const CLEFIA_DATA *pCd,const unsigned char *ct,unsigned char *pt);

#endif
