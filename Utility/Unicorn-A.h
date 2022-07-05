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

#ifndef UNICORN_H
#define UNICORN_H

// ** Thread-safe implementation

// ** Unicorn-A cipher
// ** 128bit block size
// ** 256bit key

extern	void Unicorn_set_key(BYTE *eKey,const BYTE *secret);
extern	void Unicorn_encrypt(const BYTE *eKey,const BYTE *src,BYTE *dst);
extern	void Unicorn_decrypt(const BYTE *eKey,const BYTE *src,BYTE *dst);

#endif
