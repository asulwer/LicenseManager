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

#ifndef CSPRNG_DATA_H
#define CSPRNG_DATA_H

// ** Thread-safe implementation

// ** Cryptographically Secure Pseudo Random Number Generator (by construction)
// ** Based on AES-256 + CTR

#include "CommonDef.h"
#include "MultiBase_data.h"

typedef enum {
	SHA512_HASH=0,
	GROSTL512_HASH,
	KECCAK512_HASH,
	SKEIN512_HASH
} ENUM_HASH;

#define	MAX_HASH		4

typedef struct {
	MULTI_STATIC_DATA	msd;
	BYTE				ctrBuf[DATA_BLOCK_SIZE];
	BYTE				randBuf[DATA_BLOCK_SIZE];
	DWORD				availCount;
} CSPRNG_DATA;

#endif
