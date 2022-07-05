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

#include "stdafx.h"

#include "Idea_nxt_common.h"

const BYTE pad[32] = {
    0xb7, 0xe1, 0x51, 0x62, 0x8a, 0xed, 0x2a, 0x6a, 0xbf, 0x71, 0x58, 0x80,
    0x9c, 0xf4, 0xf3, 0xc7, 0x62, 0xe7, 0x16, 0x0f, 0x38, 0xb4, 0xda, 0x56,
    0xa7, 0x84, 0xd9, 0x04, 0x51, 0x90, 0xcf, 0xef
};

void nxt_p(const BYTE *key, BYTE l, BYTE *pkey, WORD ek)
{
    memcpy(pkey, key, l);
    memcpy(pkey + l, pad, (ek >> 3) - l);
}

void nxt_m(const BYTE *pkey, BYTE *mkey, WORD ek)
{
    const BYTE mkey_m2 = 0x6a;
    const BYTE mkey_m1 = 0x76;
    int bound;
    int i;

    bound = (ek >> 3) - 1;

    mkey[0] = pkey[0] ^ (mkey_m1 + mkey_m2);
    mkey[1] = pkey[1] ^ (mkey[0] + mkey_m1);

    for (i = 2; i <= bound; i++) {
        mkey[i] = pkey[i] ^ (mkey[i - 1] + mkey[i - 2]);
    }
}
