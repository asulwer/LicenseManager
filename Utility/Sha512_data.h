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

#ifndef SHA512_DATA_H
#define SHA512_DATA_H

// ** Thread-safe implementation

// ** Sha2 hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

typedef struct {
	unsigned long	inputLen;
	QWORD	A,B,C,D,E,F,G,H;
	QWORD	totalLen;
	unsigned char	input[128];
} SHA512_DATA;

#endif
