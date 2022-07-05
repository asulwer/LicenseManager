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

#ifndef CAMELLIA_H
#define CAMELLIA_H

// ** Thread-safe implementation

// ** Camellia cipher
// ** 128bit block size
// ** 256bit key

extern	void Camellia_set_key(BYTE *e,const int n,const BYTE *k);
extern	void Camellia_encrypt(const BYTE *e,const int n,const BYTE *p,BYTE *c);
extern	void Camellia_decrypt(const BYTE *e,const int n,const BYTE *c,BYTE *p);

#endif
