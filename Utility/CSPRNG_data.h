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
	CRC32_HASH,
	KECCAK_224_HASH,
	KECCAK_256_HASH,
	KECCAK_384_HASH,
	KECCAK_512_HASH,
	MD5_HASH,
	SHA1_HASH,
	SHA3_224_HASH,
	SHA3_256_HASH,
	SHA3_384_HASH,
	SHA3_512_HASH,
	SHA256_HASH
} ENUM_HASH;

#define	MAX_HASH 8

typedef struct {
	MULTI_STATIC_DATA msd;
	unsigned char ctrBuf[64];
	unsigned char randBuf[64];
	int availCount; //0-15
} CSPRNG_DATA;

#endif
