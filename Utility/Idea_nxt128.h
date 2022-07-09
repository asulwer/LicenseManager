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

#ifndef IDEANXT128_H
#define IDEANXT128_H

// ** Thread-safe implementation

// ** Idea-Nxt cipher
// ** 128bit block size
// ** 256bit key

#include "Idea_nxt128_data.h"

extern	void Ideanxt128_set_key(nxt128_ctx *ctx,const unsigned char *key,const unsigned short key_len);
extern	void Ideanxt128_encrypt(const nxt128_ctx *ctx,const unsigned char *in,unsigned char *out);
extern	void Ideanxt128_decrypt(const nxt128_ctx *ctx,const unsigned char *in,unsigned char *out);

#endif
