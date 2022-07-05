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

#ifndef SHA512_H
#define SHA512_H

// ** Thread-safe implementation

// ** Sha2 hashing
// ** 512bit hash

#include "Sha512_data.h"

extern	void Sha512_init(SHA512_DATA *sha);
extern	void Sha512_data(SHA512_DATA *sha,const void *buffer,DWORD len);
extern	void Sha512_finalize(SHA512_DATA *sha,BYTE *hash);

#endif
