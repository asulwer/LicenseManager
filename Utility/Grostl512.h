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

#ifndef GROSTL512_H
#define GROSTL512_H

// ** Thread-safe implementation

// ** Grostl hashing
// ** 512bit hash

#include "Grostl512_data.h"

extern	void Grostl512_init(GROSTL512_DATA *grostl);
extern	void Grostl512_data(GROSTL512_DATA *grostl,const BYTE *buffer,DWORD len);
extern	void Grostl512_finalize(GROSTL512_DATA *grostl,BYTE *hash);

#endif
