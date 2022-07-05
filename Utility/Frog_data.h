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

#ifndef FROG_DATA_H
#define FROG_DATA_H

// ** Thread-safe implementation

// ** Frog cipher
// ** 128bit block size
// ** 256bit key

typedef struct  
{   BYTE  k_xbu[16];
    BYTE  k_spu[256];
    BYTE  k_bpu[16];
} k_str;

typedef struct
{   k_str   f_key[8];
    BYTE  i_key[8][256];
} key_str;

typedef struct {
	key_str loc_key;
	key_str sim_key;
	key_str	*lkp;
} FROG_DATA;

#endif
