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

#  pragma intrinsic(_lrotr,_lrotl)

#  define rotr(x,n) _lrotr(x,n)
#  define rotl(x,n) _lrotl(x,n)

#define bswap(x)    (rotl(x, 8) & 0x00ff00ff | rotr(x, 8) & 0xff00ff00)

#define byte(x,n)   ((BYTE)((x) >> (8 * n)))

#ifdef  BLOCK_SWAP
#define BYTE_SWAP
#define WORD_SWAP
#endif

#ifdef  BYTE_SWAP
#define io_swap(x)  bswap(x)
#else
#define io_swap(x)  (x)
#endif

#ifdef  WORD_SWAP

#define get_block(x)                            \
    ((DWORD *)(x))[0] = io_swap(in_blk[3]);     \
    ((DWORD *)(x))[1] = io_swap(in_blk[2]);     \
    ((DWORD *)(x))[2] = io_swap(in_blk[1]);     \
    ((DWORD *)(x))[3] = io_swap(in_blk[0])

#define put_block(x)                            \
    out_blk[3] = io_swap(((DWORD *)(x))[0]);    \
    out_blk[2] = io_swap(((DWORD *)(x))[1]);    \
    out_blk[1] = io_swap(((DWORD *)(x))[2]);    \
    out_blk[0] = io_swap(((DWORD *)(x))[3])

#define get_key(x,len)                          \
    ((DWORD *)(x))[4] = ((DWORD *)(x))[5] =     \
    ((DWORD *)(x))[6] = ((DWORD *)(x))[7] = 0;  \
    switch((((len) + 63) / 64)) {               \
    case 2:                                     \
    ((DWORD *)(x))[0] = io_swap(in_key[3]);     \
    ((DWORD *)(x))[1] = io_swap(in_key[2]);     \
    ((DWORD *)(x))[2] = io_swap(in_key[1]);     \
    ((DWORD *)(x))[3] = io_swap(in_key[0]);     \
    break;                                      \
    case 3:                                     \
    ((DWORD *)(x))[0] = io_swap(in_key[5]);     \
    ((DWORD *)(x))[1] = io_swap(in_key[4]);     \
    ((DWORD *)(x))[2] = io_swap(in_key[3]);     \
    ((DWORD *)(x))[3] = io_swap(in_key[2]);     \
    ((DWORD *)(x))[4] = io_swap(in_key[1]);     \
    ((DWORD *)(x))[5] = io_swap(in_key[0]);     \
    break;                                      \
    case 4:                                     \
    ((DWORD *)(x))[0] = io_swap(in_key[7]);     \
    ((DWORD *)(x))[1] = io_swap(in_key[6]);     \
    ((DWORD *)(x))[2] = io_swap(in_key[5]);     \
    ((DWORD *)(x))[3] = io_swap(in_key[4]);     \
    ((DWORD *)(x))[4] = io_swap(in_key[3]);     \
    ((DWORD *)(x))[5] = io_swap(in_key[2]);     \
    ((DWORD *)(x))[6] = io_swap(in_key[1]);     \
    ((DWORD *)(x))[7] = io_swap(in_key[0]);     \
    }

#else

#define get_block(x)                            \
    ((DWORD *)(x))[0] = io_swap(in_blk[0]);     \
    ((DWORD *)(x))[1] = io_swap(in_blk[1]);     \
    ((DWORD *)(x))[2] = io_swap(in_blk[2]);     \
    ((DWORD *)(x))[3] = io_swap(in_blk[3])

#define put_block(x)                            \
    out_blk[0] = io_swap(((DWORD *)(x))[0]);    \
    out_blk[1] = io_swap(((DWORD *)(x))[1]);    \
    out_blk[2] = io_swap(((DWORD *)(x))[2]);    \
    out_blk[3] = io_swap(((DWORD *)(x))[3])

#define get_key(x,len)                          \
    ((DWORD *)(x))[4] = ((DWORD *)(x))[5] =     \
    ((DWORD *)(x))[6] = ((DWORD *)(x))[7] = 0;  \
    switch((((len) + 63) / 64)) {               \
    case 4:                                     \
    ((DWORD *)(x))[6] = io_swap(in_key[6]);     \
    ((DWORD *)(x))[7] = io_swap(in_key[7]);     \
    case 3:                                     \
    ((DWORD *)(x))[4] = io_swap(in_key[4]);     \
    ((DWORD *)(x))[5] = io_swap(in_key[5]);     \
    case 2:                                     \
    ((DWORD *)(x))[0] = io_swap(in_key[0]);     \
    ((DWORD *)(x))[1] = io_swap(in_key[1]);     \
    ((DWORD *)(x))[2] = io_swap(in_key[2]);     \
    ((DWORD *)(x))[3] = io_swap(in_key[3]);     \
    }

#endif

#define f_rnd(i,a,b,c,d)                    \
        u = rotl(d * (d + d + 1), 5);       \
        t = rotl(b * (b + b + 1), 5);       \
        a = rotl(a ^ t, u) + l_key[i];      \
        c = rotl(c ^ u, t) + l_key[i + 1]

#define i_rnd(i,a,b,c,d)                    \
        u = rotl(d * (d + d + 1), 5);       \
        t = rotl(b * (b + b + 1), 5);       \
        c = rotr(c - l_key[i + 1], t) ^ u;  \
        a = rotr(a - l_key[i], u) ^ t

void Rc6_set_key(DWORD *l_key,const DWORD *in_key, const DWORD key_len)
{
	DWORD  i, j, k, a, b, l[8], t;

    l_key[0] = 0xb7e15163;

    for(k = 1; k < 44; ++k)        
        l_key[k] = l_key[k - 1] + 0x9e3779b9;

    for(k = 0; k < key_len / 32; ++k)
        l[k] = in_key[k];

    t = (key_len / 32) - 1; // t = (key_len / 32);

    a = b = i = j = 0;

    for(k = 0; k < 132; ++k)
    {
        a = rotl(l_key[i] + a + b, 3);
        b += a;
        b = rotl(l[j] + b, b);
        l_key[i] = a;
        l[j] = b;
        i = (i == 43 ? 0 : i + 1); // i = (i + 1) % 44;  
        j = (j == t ? 0 : j + 1);  // j = (j + 1) % t;
    }
}

void Rc6_encrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk)
{
	DWORD  a,b,c,d,t,u;

    a = in_blk[0]; b = in_blk[1] + l_key[0];
    c = in_blk[2]; d = in_blk[3] + l_key[1];

    f_rnd( 2,a,b,c,d); f_rnd( 4,b,c,d,a);
    f_rnd( 6,c,d,a,b); f_rnd( 8,d,a,b,c);
    f_rnd(10,a,b,c,d); f_rnd(12,b,c,d,a);
    f_rnd(14,c,d,a,b); f_rnd(16,d,a,b,c);
    f_rnd(18,a,b,c,d); f_rnd(20,b,c,d,a);
    f_rnd(22,c,d,a,b); f_rnd(24,d,a,b,c);
    f_rnd(26,a,b,c,d); f_rnd(28,b,c,d,a);
    f_rnd(30,c,d,a,b); f_rnd(32,d,a,b,c);
    f_rnd(34,a,b,c,d); f_rnd(36,b,c,d,a);
    f_rnd(38,c,d,a,b); f_rnd(40,d,a,b,c);

    out_blk[0] = a + l_key[42]; out_blk[1] = b;
    out_blk[2] = c + l_key[43]; out_blk[3] = d;
}

void Rc6_decrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk)
{
	DWORD  a,b,c,d,t,u;

    d = in_blk[3]; c = in_blk[2] - l_key[43]; 
    b = in_blk[1]; a = in_blk[0] - l_key[42];

    i_rnd(40,d,a,b,c); i_rnd(38,c,d,a,b);
    i_rnd(36,b,c,d,a); i_rnd(34,a,b,c,d);
    i_rnd(32,d,a,b,c); i_rnd(30,c,d,a,b);
    i_rnd(28,b,c,d,a); i_rnd(26,a,b,c,d);
    i_rnd(24,d,a,b,c); i_rnd(22,c,d,a,b);
    i_rnd(20,b,c,d,a); i_rnd(18,a,b,c,d);
    i_rnd(16,d,a,b,c); i_rnd(14,c,d,a,b);
    i_rnd(12,b,c,d,a); i_rnd(10,a,b,c,d);
    i_rnd( 8,d,a,b,c); i_rnd( 6,c,d,a,b);
    i_rnd( 4,b,c,d,a); i_rnd( 2,a,b,c,d);

    out_blk[3] = d - l_key[1]; out_blk[2] = c; 
    out_blk[1] = b - l_key[0]; out_blk[0] = a; 
}
