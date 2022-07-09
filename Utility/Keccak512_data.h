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

#ifndef KECCAK512_DATA_H
#define KECCAK512_DATA_H

// ** Thread-safe implementation

// ** Keccak hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

#define cKeccakB    1600
#define cKeccakR    512

typedef struct {
	unsigned char	state[cKeccakB / 8];
	int		bitsInQueue;
} KECCAK512_DATA;

#endif
