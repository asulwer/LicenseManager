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

#ifndef IDEANXT128_DATA_H
#define IDEANXT128_DATA_H

// ** Thread-safe implementation

// ** Idea-Nxt cipher
// ** 128bit block size
// ** 256bit key

// min 16 ... max 255
#define NXT128_TOTAL_ROUNDS 16

typedef struct {
    unsigned long rk[NXT128_TOTAL_ROUNDS * 4];
} nxt128_ctx;

#endif
