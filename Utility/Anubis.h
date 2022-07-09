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

#ifndef ANUBIS_H
#define ANUBIS_H

// ** Thread-safe implementation

// ** Anubis cipher
// ** 128bit block size
// ** 256bit key

#include "Anubis_data.h"

extern	void Anubis_set_key(ANUBIS_DATA *pAd,const unsigned char *key);
extern	void Anubis_encrypt(const ANUBIS_DATA *pAd,const unsigned char *plaintext,unsigned char *ciphertext);
extern	void Anubis_decrypt(const ANUBIS_DATA *pAd,const unsigned char *ciphertext,unsigned char *plaintext);

#endif
