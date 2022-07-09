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

#ifndef RIJNDAEL_H
#define RIJNDAEL_H

// ** Thread-safe implementation

// ** Aes cipher
// ** 128bit block size
// ** 256bit key

#include "Rijndael_data.h"

extern	int Rijndael_set_key_encrypt(unsigned long *rk,const unsigned char *key,const int keybits);
extern	int Rijndael_set_key_decrypt(unsigned long *rk,const unsigned char *key,const int keybits);
extern	void Rijndael_encrypt(const unsigned long *rk,const int nrounds,const unsigned char *plaintext,unsigned char *ciphertext);
extern	void Rijndael_decrypt(const unsigned long *rk,const int nrounds,const unsigned char *ciphertext,unsigned char *plaintext);

#endif
