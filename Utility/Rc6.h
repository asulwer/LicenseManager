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

#ifndef RC6_H
#define RC6_H

// ** Thread-safe implementation

// ** Rc6 cipher
// ** 128bit block size
// ** 256bit key

extern	void Rc6_set_key(unsigned long *l_key,const unsigned long *in_key, const unsigned long key_len);
extern	void Rc6_encrypt(const unsigned long *l_key,const unsigned long *in_blk, unsigned long *out_blk);
extern	void Rc6_decrypt(const unsigned long *l_key,const unsigned long *in_blk, unsigned long *out_blk);

#endif
