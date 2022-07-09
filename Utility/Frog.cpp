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

#include "Frog_data.h"

#define BLOCK_SWAP

#  pragma intrinsic(_lrotr,_lrotl)

#  define rotr(x,n) _lrotr(x,n)
#  define rotl(x,n) _lrotl(x,n)

#define bswap(x)    (rotl(x, 8) & 0x00ff00ff | rotr(x, 8) & 0xff00ff00)

#define byte(x,n)   ((unsigned char)((x) >> (8 * n)))

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
    ((unsigned long *)(x))[0] = io_swap(in_blk[3]);     \
    ((unsigned long *)(x))[1] = io_swap(in_blk[2]);     \
    ((unsigned long *)(x))[2] = io_swap(in_blk[1]);     \
    ((unsigned long *)(x))[3] = io_swap(in_blk[0])

#define put_block(x)                            \
    out_blk[3] = io_swap(((unsigned long *)(x))[0]);    \
    out_blk[2] = io_swap(((unsigned long *)(x))[1]);    \
    out_blk[1] = io_swap(((unsigned long *)(x))[2]);    \
    out_blk[0] = io_swap(((unsigned long *)(x))[3])

#define get_key(x,len)                          \
    ((unsigned long *)(x))[4] = ((unsigned long *)(x))[5] =     \
    ((unsigned long *)(x))[6] = ((unsigned long *)(x))[7] = 0;  \
    switch((((len) + 63) / 64)) {               \
    case 2:                                     \
    ((unsigned long *)(x))[0] = io_swap(in_key[3]);     \
    ((unsigned long *)(x))[1] = io_swap(in_key[2]);     \
    ((unsigned long *)(x))[2] = io_swap(in_key[1]);     \
    ((unsigned long *)(x))[3] = io_swap(in_key[0]);     \
    break;                                      \
    case 3:                                     \
    ((unsigned long *)(x))[0] = io_swap(in_key[5]);     \
    ((unsigned long *)(x))[1] = io_swap(in_key[4]);     \
    ((unsigned long *)(x))[2] = io_swap(in_key[3]);     \
    ((unsigned long *)(x))[3] = io_swap(in_key[2]);     \
    ((unsigned long *)(x))[4] = io_swap(in_key[1]);     \
    ((unsigned long *)(x))[5] = io_swap(in_key[0]);     \
    break;                                      \
    case 4:                                     \
    ((unsigned long *)(x))[0] = io_swap(in_key[7]);     \
    ((unsigned long *)(x))[1] = io_swap(in_key[6]);     \
    ((unsigned long *)(x))[2] = io_swap(in_key[5]);     \
    ((unsigned long *)(x))[3] = io_swap(in_key[4]);     \
    ((unsigned long *)(x))[4] = io_swap(in_key[3]);     \
    ((unsigned long *)(x))[5] = io_swap(in_key[2]);     \
    ((unsigned long *)(x))[6] = io_swap(in_key[1]);     \
    ((unsigned long *)(x))[7] = io_swap(in_key[0]);     \
    }

#else

#define get_block(x)                            \
    ((unsigned long *)(x))[0] = io_swap(in_blk[0]);     \
    ((unsigned long *)(x))[1] = io_swap(in_blk[1]);     \
    ((unsigned long *)(x))[2] = io_swap(in_blk[2]);     \
    ((unsigned long *)(x))[3] = io_swap(in_blk[3])

#define put_block(x)                            \
    out_blk[0] = io_swap(((unsigned long *)(x))[0]);    \
    out_blk[1] = io_swap(((unsigned long *)(x))[1]);    \
    out_blk[2] = io_swap(((unsigned long *)(x))[2]);    \
    out_blk[3] = io_swap(((unsigned long *)(x))[3])

#define get_key(x,len)                          \
    ((unsigned long *)(x))[4] = ((unsigned long *)(x))[5] =     \
    ((unsigned long *)(x))[6] = ((unsigned long *)(x))[7] = 0;  \
    switch((((len) + 63) / 64)) {               \
    case 4:                                     \
    ((unsigned long *)(x))[6] = io_swap(in_key[6]);     \
    ((unsigned long *)(x))[7] = io_swap(in_key[7]);     \
    case 3:                                     \
    ((unsigned long *)(x))[4] = io_swap(in_key[4]);     \
    ((unsigned long *)(x))[5] = io_swap(in_key[5]);     \
    case 2:                                     \
    ((unsigned long *)(x))[0] = io_swap(in_key[0]);     \
    ((unsigned long *)(x))[1] = io_swap(in_key[1]);     \
    ((unsigned long *)(x))[2] = io_swap(in_key[2]);     \
    ((unsigned long *)(x))[3] = io_swap(in_key[3]);     \
    }

#endif

#ifdef CORE_TIME
#  undef BLOCK_SWAP
#endif

#define	ik_len		2304

const unsigned char  seed[256] =
{
    113,  21, 232,  18, 113,  92,  63, 157, 124, 193, 166, 197, 126,  56, 229, 229, 
    156, 162,  54,  17, 230,  89, 189,  87, 169,   0,  81, 204,   8,  70, 203, 225, 
    160,  59, 167, 189, 100, 157,  84,  11,   7, 130,  29,  51,  32,  45, 135, 237, 
    139,  33,  17, 221,  24,  50,  89,  74,  21, 205, 191, 242,  84,  53,   3, 230, 

    231, 118,  15,  15, 107,   4,  21,  34,   3, 156,  57,  66,  93, 255, 191,   3, 
     85, 135, 205, 200, 185, 204,  52,  37,  35,  24,  68, 185, 201,  10, 224, 234, 
      7, 120, 201, 115, 216, 103,  57, 255,  93, 110,  42, 249,  68,  14,  29,  55, 
    128,  84,  37, 152, 221, 137,  39,  11, 252,  50, 144,  35, 178, 190,  43, 162, 

    103, 249, 109,   8, 235,  33, 158, 111, 252, 205, 169,  54,  10,  20, 221, 201, 
    178, 224,  89, 184, 182,  65, 201,  10,  60,   6, 191, 174,  79,  98,  26, 160, 
    252,  51,  63,  79,   6, 102, 123, 173,  49,   3, 110, 233,  90, 158, 228, 210, 
    209, 237,  30,  95,  28, 179, 204, 220,  72, 163,  77, 166, 192,  98, 165,  25, 

    145, 162,  91, 212,  41, 230, 110,   6, 107, 187, 127,  38,  82,  98,  30,  67, 
    225,  80, 208, 134,  60, 250, 153,  87, 148,  60,  66, 165,  72,  29, 165,  82, 
    211, 207,   0, 177, 206,  13,   6,  14,  92, 248,  60, 201, 132,  95,  35, 215, 
    118, 177, 121, 180,  27,  83, 131,  26,  39,  46,  12,   0,   0,   0,   0,   0
};

// top is 1 greater than that used in FROG specification
// this routine makes a permutation containing 'top' values

void make_perm(unsigned char *ip, unsigned long top)
{
	unsigned char  ua[260];
    unsigned long  i, j, ie, ne;

    for(i = 0; i < top; ++i)

        ua[i] = (unsigned char)i;

    ie = 0; ne = top;

    for(i = 0; i < top - 1; ++i)
    {
        ie = (ie + ip[i]) % ne;

        ip[i] = ua[ie]; ne--;

        for(j = ie; j < ne; ++j)

            ua[j] = ua[j + 1];
    }

    ip[top - 1] = ua[0];
}

void make_ikey(key_str *kp)
{
	unsigned long  i, j, k, ix, ll;
    unsigned char  ua[16];
        
    for(i = 0; i < 8; ++i)
    {
        make_perm(kp->f_key[i].k_spu, 256);

        for(j = 0; j < 256; ++j)

            kp->i_key[i][kp->f_key[i].k_spu[j]] = (unsigned char)j;

        make_perm(kp->f_key[i].k_bpu, 16);

        for(j = 0; j < 16; ++j)

            ua[j] = 0;

        for(j = ix = 0; j < 15; ++j)
        {
            if(!kp->f_key[i].k_bpu[ix])
            {
                k = ix;

                do
                {
                    k = (k + 1) & 15;
                }
                while
                    (ua[k]);

                kp->f_key[i].k_bpu[ix] = (unsigned char)k; ll = k;

                while(kp->f_key[i].k_bpu[ll] != k)
                
                    ll = kp->f_key[i].k_bpu[ll];

                kp->f_key[i].k_bpu[ll] = 0;
            }

            ua[ix] = 1; ix = kp->f_key[i].k_bpu[ix];
        }

        for(j = 0; j < 16; ++j)

            if(kp->f_key[i].k_bpu[j] == ((j + 1) & 15))

                kp->f_key[i].k_bpu[j] = (unsigned char)((j + 2) & 15);
    }
}

#define f_rnd(j)                    \
    k = pp[j];                      \
    blk[j] = sp[blk[j] ^ xp[j]];    \
    blk[(j + 1) & 15] ^= blk[j];    \
    blk[k] ^= blk[j]

#define b_rnd(j)                    \
    ct = blk[j];                    \
    blk[pp[j]] ^= blk[j];           \
    blk[(j + 1) & 15] ^= blk[j];    \
    blk[j] = sp[ct] ^ xp[j]

void enc(const FROG_DATA *pFd,const unsigned long in_blk[4], unsigned long out_blk[4])
{
	unsigned long  i, k;
    unsigned char  blk[16], *xp, *sp, *pp;
    
    *(unsigned long *)(blk +  0) = in_blk[0]; *(unsigned long *)(blk +  4) = in_blk[1];
    *(unsigned long *)(blk +  8) = in_blk[2]; *(unsigned long *)(blk + 12) = in_blk[3];
    
    for(i = 0; i < 8; ++i)
    {
        xp = pFd->lkp->f_key[i].k_xbu;
        sp = pFd->lkp->f_key[i].k_spu;
        pp = pFd->lkp->f_key[i].k_bpu;

        f_rnd( 0); f_rnd( 1); f_rnd( 2); f_rnd( 3);
        f_rnd( 4); f_rnd( 5); f_rnd( 6); f_rnd( 7);
        f_rnd( 8); f_rnd( 9); f_rnd(10); f_rnd(11);
        f_rnd(12); f_rnd(13); f_rnd(14); f_rnd(15);
    }

    out_blk[0] = *(unsigned long *)(blk +  0); out_blk[1] = *(unsigned long *)(blk +  4);
    out_blk[2] = *(unsigned long *)(blk +  8); out_blk[3] = *(unsigned long *)(blk + 12);
}

/* initialise the key schedule from the user supplied key   */
void Frog_set_key(FROG_DATA *pFd,const unsigned long *in_key,const unsigned long key_len)
{
	unsigned long  i, j, k;
    unsigned char  kb[32];

    get_key(kb, key_len);

    for(i = j = k = 0; i < ik_len; ++i)
    {
        ((unsigned char *)(pFd->sim_key.f_key))[i] = seed[j] ^ kb[k];

        j = (j < 250 ? j + 1 : 0);

        k  = (k < (key_len / 8) - 1 ? k + 1 : 0);
    }

    make_ikey(&pFd->sim_key); pFd->lkp = &pFd->sim_key;

    kb[0] ^= (key_len / 8); 

    for(i = 0; i < ik_len / 16; ++i)
    {
        enc(pFd,(unsigned long *)kb, (unsigned long *)kb);

        for(j = 0; j < 16; ++j)

            ((unsigned char *)(pFd->loc_key.f_key))[16 * i + j] = kb[j];
    }

    make_ikey(&pFd->loc_key);

    pFd->lkp = &pFd->loc_key;
}

/* encrypt a block of text  */

void Frog_encrypt(const FROG_DATA *pFd,const unsigned long in_blk[4], unsigned long out_blk[4])
{
	unsigned long  i, k;
    unsigned char  blk[16], *xp, *sp, *pp;
    
    get_block(blk);
    
    for(i = 0; i < 8; ++i)
    {
        xp = pFd->lkp->f_key[i].k_xbu;
        sp = pFd->lkp->f_key[i].k_spu;
        pp = pFd->lkp->f_key[i].k_bpu;

        f_rnd( 0); f_rnd( 1); f_rnd( 2); f_rnd( 3);
        f_rnd( 4); f_rnd( 5); f_rnd( 6); f_rnd( 7);
        f_rnd( 8); f_rnd( 9); f_rnd(10); f_rnd(11);
        f_rnd(12); f_rnd(13); f_rnd(14); f_rnd(15);
    }

    put_block(blk);
}

/* decrypt a block of text  */

void Frog_decrypt(const FROG_DATA *pFd,const unsigned long in_blk[4], unsigned long out_blk[4])
{
	int  i;
    unsigned char  blk[16], ct, *xp, *sp, *pp;
    
    get_block(blk);
    
    for(i = 7; i >= 0; --i)
    {
        xp = pFd->lkp->f_key[i].k_xbu;
        sp = pFd->lkp->i_key[i];
        pp = pFd->lkp->f_key[i].k_bpu;

        b_rnd(15); b_rnd(14); b_rnd(13); b_rnd(12);
        b_rnd(11); b_rnd(10); b_rnd( 9); b_rnd( 8);
        b_rnd( 7); b_rnd( 6); b_rnd( 5); b_rnd( 4);
        b_rnd( 3); b_rnd( 2); b_rnd( 1); b_rnd( 0);
    }

    put_block(blk);
}
