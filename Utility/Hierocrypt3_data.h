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

#ifndef HC3_DATA_H
#define HC3_DATA_H

// ** Thread-safe implementation

// ** Hierocrypt3 cipher
// ** 128bit block size
// ** 256bit key

#define MAX_ROUND (8)

typedef BYTE HC3_KS[MAX_ROUND + 1][8][4];

typedef struct {
	HC3_KS ks;
	HC3_KS dks;
} HIEROCRYPT3_DATA;

#endif
