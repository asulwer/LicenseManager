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

#ifndef IDEA_NXT_COMMON_H
#define IDEA_NXT_COMMON_H

// ** Thread-safe implementation

// ** Idea-Nxt cipher
// ** 128bit block size
// ** 256bit key

/*
 * These macros define which algorithms are used. You can comment one of the
 * two macros if you don't want to use both algorithms.
 */
#define USE_NXT64
#define USE_NXT128

/*
 * NXT64 macros
 */
#ifdef USE_NXT64

#if 1
#define NXT64_UNROLL_LOOPS
#endif

#endif /* USE_NXT64 */

/*
 * NXT128 macros
 */
#ifdef USE_NXT128

#if 1
#define NXT128_UNROLL_LOOPS
#endif

#endif /* USE_NXT128 */

#define UNPACK32(x, str)                \
{                                       \
    *((str) + 3) = (BYTE) ((x)      ); \
    *((str) + 2) = (BYTE) ((x) >>  8); \
    *((str) + 1) = (BYTE) ((x) >> 16); \
    *((str)    ) = (BYTE) ((x) >> 24); \
}

#define PACK32(str, x)            \
{                                 \
    *(x) = ( *((str) + 3)      )  \
         | ( *((str) + 2) <<  8)  \
         | ( *((str) + 1) << 16)  \
         | ( *((str)    ) << 24); \
}

#define IRRED_POLY 0x1f9

#define LFSR(reg, lfsr_value) \
do {                          \
    *reg = *reg << 1;         \
    if (*reg & 0x1000000)     \
        *reg ^= 0x100001b;    \
    lfsr_value = *reg;        \
} while(0)

#define NXT_OR(x) \
(x << 16) ^ (x >> 16) ^ (x & 0x0000ffff);

#define NXT_IO(x) \
(x << 16) ^ (x >> 16) ^ (x & 0xffff0000);

extern	const BYTE pad[32];

void nxt_p(const BYTE *key, BYTE l, BYTE *pkey, WORD ek);
void nxt_m(const BYTE *pkey, BYTE *mkey, WORD ek);

#endif
