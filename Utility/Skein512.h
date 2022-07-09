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

#ifndef SKEIN512_H
#define SKEIN512_H

// ** Thread-safe implementation

// ** Skein hashing
// ** 512bit hash

#include "Skein512_data.h"

extern	void Skein512_init(SKEIN512_DATA *skein);
extern	void Skein512_data(SKEIN512_DATA *skein,const unsigned char *buffer,unsigned long len);
extern	void Skein512_finalize(SKEIN512_DATA *skein,unsigned char *hash);

#endif
