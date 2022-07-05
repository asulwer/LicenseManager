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

#ifndef SPEED_H
#define SPEED_H

// ** Thread-safe implementation

// ** Speed cipher
// ** 128bit block size
// ** 256bit key

#include "Speed_data.h"

extern	void Speed_set_key(speed_ikey rndkey,const speed_key key);
extern	void Speed_encrypt(const speed_ikey rndkey,const speed_data pt,speed_data ct);
extern	void Speed_decrypt(const speed_ikey rndkey,const speed_data ct,speed_data pt);

#endif
