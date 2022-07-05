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

#ifndef SKEIN512_DATA_H
#define SKEIN512_DATA_H

// ** Thread-safe implementation

// ** Skein hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

#define  SKEIN_MODIFIER_WORDS  ( 2)

#define  SKEIN_512_STATE_WORDS ( 8)
#define  SKEIN_512_STATE_BYTES ( 8*SKEIN_512_STATE_WORDS)
#define  SKEIN_512_STATE_BITS  (64*SKEIN_512_STATE_WORDS)
#define  SKEIN_512_BLOCK_BYTES ( 8*SKEIN_512_STATE_WORDS)

typedef struct {
	DWORD  hashBitLen;				/* size of hash result, in bits */
	DWORD  bCnt;					/* current byte count in buffer b[] */
	QWORD  T[SKEIN_MODIFIER_WORDS];	/* tweak words: T[0]=byte cnt, T[1]=flags */
} Skein_Ctxt_Hdr_t;

typedef struct {
	Skein_Ctxt_Hdr_t h;							/* common header context variables */
	QWORD			X[SKEIN_512_STATE_WORDS];	/* chaining variables */
	BYTE			b[SKEIN_512_BLOCK_BYTES];	/* partial block buffer (8-byte aligned) */
} SKEIN512_DATA;

#endif
