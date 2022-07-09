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

#ifndef FROG_H
#define FROG_H

// ** Thread-safe implementation

// ** Frog cipher
// ** 128bit block size
// ** 256bit key

#include "Frog_data.h"

extern	void Frog_set_key(FROG_DATA *pFd,const unsigned long *in_key,const unsigned long key_len);
extern	void Frog_encrypt(const FROG_DATA *pFd,const unsigned long in_blk[4], unsigned long out_blk[4]);
extern	void Frog_decrypt(const FROG_DATA *pFd,const unsigned long in_blk[4], unsigned long out_blk[4]);

#endif
