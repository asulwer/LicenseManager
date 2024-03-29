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
// You�re free to copy, distribute and make commercial use
// of this software under the following conditions:
//
// * You have to cite the author (and copyright owner): Eng. Cosimo Oliboni
// * You have to provide a link to the author�s Homepage: <http://embeddedsw.net/>
//
///////////////////////////////////////////////

#ifndef SC2000_H
#define SC2000_H

// ** Thread-safe implementation

// ** Sc2000 cipher
// ** 128bit block size
// ** 256bit key

extern	void Sc2000_set_key(unsigned long *ek,const unsigned long *in_key);
extern	void Sc2000_encrypt(const unsigned long *ek,const unsigned long *in, unsigned long *out);
extern	void Sc2000_decrypt(const unsigned long *ek,const unsigned long *in, unsigned long *out);

#endif
