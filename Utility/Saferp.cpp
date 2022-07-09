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

#include "Saferp_data.h"

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

const unsigned char  exp_f[256] =
{     1,  45, 226, 147, 190,  69,  21, 174, 120,   3, 135, 164, 184,  56, 207,  63, 
      8, 103,   9, 148, 235,  38, 168, 107, 189,  24,  52,  27, 187, 191, 114, 247, 
     64,  53,  72, 156,  81,  47,  59,  85, 227, 192, 159, 216, 211, 243, 141, 177, 
    255, 167,  62, 220, 134, 119, 215, 166,  17, 251, 244, 186, 146, 145, 100, 131, 
    241,  51, 239, 218,  44, 181, 178,  43, 136, 209, 153, 203, 140, 132,  29,  20, 
    129, 151, 113, 202,  95, 163, 139,  87,  60, 130, 196,  82,  92,  28, 232, 160, 
      4, 180, 133,  74, 246,  19,  84, 182, 223,  12,  26, 142, 222, 224,  57, 252, 
     32, 155,  36,  78, 169, 152, 158, 171, 242,  96, 208, 108, 234, 250, 199, 217, 
      0, 212,  31, 110,  67, 188, 236,  83, 137, 254, 122,  93,  73, 201,  50, 194, 
    249, 154, 248, 109,  22, 219,  89, 150,  68, 233, 205, 230,  70,  66, 143,  10, 
    193, 204, 185, 101, 176, 210, 198, 172,  30,  65,  98,  41,  46,  14, 116,  80, 
      2,  90, 195,  37, 123, 138,  42,  91, 240,   6,  13,  71, 111, 112, 157, 126, 
     16, 206,  18,  39, 213,  76,  79, 214, 121,  48, 104,  54, 117, 125, 228, 237, 
    128, 106, 144,  55, 162,  94, 118, 170, 197, 127,  61, 175, 165, 229,  25,  97, 
    253,  77, 124, 183,  11, 238, 173,  75,  34, 245, 231, 115,  35,  33, 200,   5, 
    225, 102, 221, 179,  88, 105,  99,  86,  15, 161,  49, 149,  23,   7,  58,  40 
};

const unsigned char log_f[512] = 
{
    128,   0, 176,   9,  96, 239, 185, 253,  16,  18, 159, 228, 105, 186, 173, 248, 
    192,  56, 194, 101,  79,   6, 148, 252,  25, 222, 106,  27,  93,  78, 168, 130, 
    112, 237, 232, 236, 114, 179,  21, 195, 255, 171, 182,  71,  68,   1, 172,  37, 
    201, 250, 142,  65,  26,  33, 203, 211,  13, 110, 254,  38,  88, 218,  50,  15, 
     32, 169, 157, 132, 152,   5, 156, 187,  34, 140,  99, 231, 197, 225, 115, 198, 
    175,  36,  91, 135, 102,  39, 247,  87, 244, 150, 177, 183,  92, 139, 213,  84, 
    121, 223, 170, 246,  62, 163, 241,  17, 202, 245, 209,  23, 123, 147, 131, 188, 
    189,  82,  30, 235, 174, 204, 214,  53,   8, 200, 138, 180, 226, 205, 191, 217, 
    208,  80,  89,  63,  77,  98,  52,  10,  72, 136, 181,  86,  76,  46, 107, 158, 
    210,  61,  60,   3,  19, 251, 151,  81, 117,  74, 145, 113,  35, 190, 118,  42, 
     95, 249, 212,  85,  11, 220,  55,  49,  22, 116, 215, 119, 167, 230,   7, 219, 
    164,  47,  70, 243,  97,  69, 103, 227,  12, 162,  59,  28, 133,  24,   4,  29, 
     41, 160, 143, 178,  90, 216, 166, 126, 238, 141,  83,  75, 161, 154, 193,  14, 
    122,  73, 165,  44, 129, 196, 199,  54,  43, 127,  67, 149,  51, 242, 108, 104, 
    109, 240,   2,  40, 206, 221, 155, 234,  94, 153, 124,  20, 134, 207, 229,  66, 
    184,  64, 120,  45,  58, 233, 100,  31, 146, 144, 125,  57, 111, 224, 137,  48,

    128,   0, 176,   9,  96, 239, 185, 253,  16,  18, 159, 228, 105, 186, 173, 248, 
    192,  56, 194, 101,  79,   6, 148, 252,  25, 222, 106,  27,  93,  78, 168, 130, 
    112, 237, 232, 236, 114, 179,  21, 195, 255, 171, 182,  71,  68,   1, 172,  37, 
    201, 250, 142,  65,  26,  33, 203, 211,  13, 110, 254,  38,  88, 218,  50,  15, 
     32, 169, 157, 132, 152,   5, 156, 187,  34, 140,  99, 231, 197, 225, 115, 198, 
    175,  36,  91, 135, 102,  39, 247,  87, 244, 150, 177, 183,  92, 139, 213,  84, 
    121, 223, 170, 246,  62, 163, 241,  17, 202, 245, 209,  23, 123, 147, 131, 188, 
    189,  82,  30, 235, 174, 204, 214,  53,   8, 200, 138, 180, 226, 205, 191, 217, 
    208,  80,  89,  63,  77,  98,  52,  10,  72, 136, 181,  86,  76,  46, 107, 158, 
    210,  61,  60,   3,  19, 251, 151,  81, 117,  74, 145, 113,  35, 190, 118,  42, 
     95, 249, 212,  85,  11, 220,  55,  49,  22, 116, 215, 119, 167, 230,   7, 219, 
    164,  47,  70, 243,  97,  69, 103, 227,  12, 162,  59,  28, 133,  24,   4,  29, 
     41, 160, 143, 178,  90, 216, 166, 126, 238, 141,  83,  75, 161, 154, 193,  14, 
    122,  73, 165,  44, 129, 196, 199,  54,  43, 127,  67, 149,  51, 242, 108, 104, 
    109, 240,   2,  40, 206, 221, 155, 234,  94, 153, 124,  20, 134, 207, 229,  66, 
    184,  64, 120,  45,  58, 233, 100,  31, 146, 144, 125,  57, 111, 224, 137,  48
};

void Saferp_set_key(SAFERP_DATA *pSpd,const unsigned long *in_key,const unsigned long key_len)
{
	unsigned char  by, lk[33];
    unsigned long  i, j, k, l, m;

    get_key(lk, key_len);

    pSpd->k_bytes = key_len / 8; lk[pSpd->k_bytes] = 0;

    for(i = 0; i < pSpd->k_bytes; ++i)
    {
        lk[pSpd->k_bytes] ^= lk[i]; pSpd->l_key[i] = lk[i];
    }

    for(i = 0; i < pSpd->k_bytes; ++i)
    {
        for(j = 0; j <= pSpd->k_bytes; ++j)
        {
            by = lk[j]; lk[j] = by << 3 | by >> 5;
        }

        k = 17 * i + 35; l = 16 * i + 16; m = i + 1;

        if(i < 16)
        {
            for(j = 0; j < 16; ++j)
            {
                pSpd->l_key[l + j] = lk[m] + exp_f[exp_f[(k + j) & 255]];

                m = (m == pSpd->k_bytes ? 0 : m + 1);
            }
        }
        else
        {
            for(j = 0; j < 16; ++j)
            {
                pSpd->l_key[l + j] = lk[m] + exp_f[(k + j) & 255];

                m = (m == pSpd->k_bytes ? 0 : m + 1);
            }
        }
    }
}

void do_fr(unsigned char *x, unsigned char *kp)
{
	unsigned char  t;

    x[ 0] = exp_f[x[ 0] ^ kp[ 0]] + kp[16];
    x[ 1] = log_f[x[ 1] + kp[ 1]] ^ kp[17]; 
    x[ 2] = log_f[x[ 2] + kp[ 2]] ^ kp[18]; 
    x[ 3] = exp_f[x[ 3] ^ kp[ 3]] + kp[19];

    x[ 4] = exp_f[x[ 4] ^ kp[ 4]] + kp[20];
    x[ 5] = log_f[x[ 5] + kp[ 5]] ^ kp[21]; 
    x[ 6] = log_f[x[ 6] + kp[ 6]] ^ kp[22]; 
    x[ 7] = exp_f[x[ 7] ^ kp[ 7]] + kp[23];
 
    x[ 8] = exp_f[x[ 8] ^ kp[ 8]] + kp[24];
    x[ 9] = log_f[x[ 9] + kp[ 9]] ^ kp[25]; 
    x[10] = log_f[x[10] + kp[10]] ^ kp[26]; 
    x[11] = exp_f[x[11] ^ kp[11]] + kp[27];

    x[12] = exp_f[x[12] ^ kp[12]] + kp[28];
    x[13] = log_f[x[13] + kp[13]] ^ kp[29]; 
    x[14] = log_f[x[14] + kp[14]] ^ kp[30]; 
    x[15] = exp_f[x[15] ^ kp[15]] + kp[31];

    x[ 1] += x[ 0]; x[ 0] += x[ 1];
    x[ 3] += x[ 2]; x[ 2] += x[ 3];
    x[ 5] += x[ 4]; x[ 4] += x[ 5];
    x[ 7] += x[ 6]; x[ 6] += x[ 7];
    x[ 9] += x[ 8]; x[ 8] += x[ 9];
    x[11] += x[10]; x[10] += x[11];
    x[13] += x[12]; x[12] += x[13];
    x[15] += x[14]; x[14] += x[15];

    x[ 7] += x[ 0]; x[ 0] += x[ 7];
    x[ 1] += x[ 2]; x[ 2] += x[ 1];
    x[ 3] += x[ 4]; x[ 4] += x[ 3];
    x[ 5] += x[ 6]; x[ 6] += x[ 5];
    x[11] += x[ 8]; x[ 8] += x[11];
    x[ 9] += x[10]; x[10] += x[ 9];
    x[15] += x[12]; x[12] += x[15];
    x[13] += x[14]; x[14] += x[13];

    x[ 3] += x[ 0]; x[ 0] += x[ 3];
    x[15] += x[ 2]; x[ 2] += x[15];
    x[ 7] += x[ 4]; x[ 4] += x[ 7];
    x[ 1] += x[ 6]; x[ 6] += x[ 1];
    x[ 5] += x[ 8]; x[ 8] += x[ 5];
    x[13] += x[10]; x[10] += x[13];
    x[11] += x[12]; x[12] += x[11];
    x[ 9] += x[14]; x[14] += x[ 9];

    x[13] += x[ 0]; x[ 0] += x[13];
    x[ 5] += x[ 2]; x[ 2] += x[ 5];
    x[ 9] += x[ 4]; x[ 4] += x[ 9];
    x[11] += x[ 6]; x[ 6] += x[11];
    x[15] += x[ 8]; x[ 8] += x[15];
    x[ 1] += x[10]; x[10] += x[ 1];
    x[ 3] += x[12]; x[12] += x[ 3];
    x[ 7] += x[14]; x[14] += x[ 7];

    t = x[0]; x[0] = x[14]; x[14] = x[12]; x[12] = x[10]; x[10] = x[2]; 
    x[2] = x[8]; x[8] = x[4]; x[4] = t;

    t = x[1]; x[1] = x[7]; x[7] = x[11]; x[11] = x[5]; x[5] = x[13]; x[13] = t; 
    
    t = x[15]; x[15] = x[3]; x[3] = t;
}

void do_ir(unsigned char *x, const unsigned char *kp)
{
	unsigned char  t;

    t = x[3]; x[3] = x[15]; x[15] = t; 

    t = x[13]; x[13] = x[5]; x[5] = x[11]; x[11] = x[7]; x[7] = x[1]; x[1] = t; 

    t = x[4]; x[4] = x[8]; x[8] = x[2]; x[2] = x[10]; 
    x[10] = x[12]; x[12] = x[14]; x[14] = x[0]; x[0] = t; 

    x[14] -= x[ 7]; x[ 7] -= x[14]; 
    x[12] -= x[ 3]; x[ 3] -= x[12];
    x[10] -= x[ 1]; x[ 1] -= x[10];
    x[ 8] -= x[15]; x[15] -= x[ 8];
    x[ 6] -= x[11]; x[11] -= x[ 6]; 
    x[ 4] -= x[ 9]; x[ 9] -= x[ 4];
    x[ 2] -= x[ 5]; x[ 5] -= x[ 2]; 
    x[ 0] -= x[13]; x[13] -= x[ 0]; 

    x[14] -= x[ 9]; x[ 9] -= x[14]; 
    x[12] -= x[11]; x[11] -= x[12]; 
    x[10] -= x[13]; x[13] -= x[10]; 
    x[ 8] -= x[ 5]; x[ 5] -= x[ 8]; 
    x[ 6] -= x[ 1]; x[ 1] -= x[ 6]; 
    x[ 4] -= x[ 7]; x[ 7] -= x[ 4]; 
    x[ 2] -= x[15]; x[15] -= x[ 2]; 
    x[ 0] -= x[ 3]; x[ 3] -= x[ 0]; 

    x[14] -= x[13]; x[13] -= x[14]; 
    x[12] -= x[15]; x[15] -= x[12]; 
    x[10] -= x[ 9]; x[ 9] -= x[10]; 
    x[ 8] -= x[11]; x[11] -= x[ 8];     
    x[ 6] -= x[ 5]; x[ 5] -= x[ 6]; 
    x[ 4] -= x[ 3]; x[ 3] -= x[ 4]; 
    x[ 2] -= x[ 1]; x[ 1] -= x[ 2]; 
    x[ 0] -= x[ 7]; x[ 7] -= x[ 0]; 

    x[14] -= x[15]; x[15] -= x[14]; 
    x[12] -= x[13]; x[13] -= x[12];
    x[10] -= x[11]; x[11] -= x[10]; 
    x[ 8] -= x[ 9]; x[ 9] -= x[ 8]; 
    x[ 6] -= x[ 7]; x[ 7] -= x[ 6];
    x[ 4] -= x[ 5]; x[ 5] -= x[ 4]; 
    x[ 2] -= x[ 3]; x[ 3] -= x[ 2]; 
    x[ 0] -= x[ 1]; x[ 1] -= x[ 0]; 
    
    x[ 0] = log_f[x[ 0] - kp[16] + 256] ^ kp[ 0];
    x[ 1] = exp_f[x[ 1] ^ kp[17]] - kp[ 1];
    x[ 2] = exp_f[x[ 2] ^ kp[18]] - kp[ 2];
    x[ 3] = log_f[x[ 3] - kp[19] + 256] ^ kp[ 3];

    x[ 4] = log_f[x[ 4] - kp[20] + 256] ^ kp[ 4];
    x[ 5] = exp_f[x[ 5] ^ kp[21]] - kp[ 5];
    x[ 6] = exp_f[x[ 6] ^ kp[22]] - kp[ 6];
    x[ 7] = log_f[x[ 7] - kp[23] + 256] ^ kp[ 7];

    x[ 8] = log_f[x[ 8] - kp[24] + 256] ^ kp[ 8];
    x[ 9] = exp_f[x[ 9] ^ kp[25]] - kp[ 9];
    x[10] = exp_f[x[10] ^ kp[26]] - kp[10];
    x[11] = log_f[x[11] - kp[27] + 256] ^ kp[11];

    x[12] = log_f[x[12] - kp[28] + 256] ^ kp[12];
    x[13] = exp_f[x[13] ^ kp[29]] - kp[13];
    x[14] = exp_f[x[14] ^ kp[30]] - kp[14];
    x[15] = log_f[x[15] - kp[31] + 256] ^ kp[15];
}

void Saferp_encrypt(const SAFERP_DATA *pSpd,const unsigned long *in_blk, unsigned long *out_blk)
{
	unsigned char  blk[16], *kp;

    get_block(blk);

    do_fr(blk, (unsigned char *) pSpd->l_key);         do_fr(blk, ((unsigned char *) pSpd->l_key) +  32); 
    do_fr(blk, ((unsigned char *) pSpd->l_key) +  64); do_fr(blk, ((unsigned char *) pSpd->l_key) +  96);
    do_fr(blk, ((unsigned char *) pSpd->l_key) + 128); do_fr(blk, ((unsigned char *) pSpd->l_key) + 160);
    do_fr(blk, ((unsigned char *) pSpd->l_key) + 192); do_fr(blk, ((unsigned char *) pSpd->l_key) + 224);
    
    if(pSpd->k_bytes > 16)
    {
        do_fr(blk, ((unsigned char *) pSpd->l_key) + 256); do_fr(blk, ((unsigned char *) pSpd->l_key) + 288); 
        do_fr(blk, ((unsigned char *) pSpd->l_key) + 320); do_fr(blk, ((unsigned char *) pSpd->l_key) + 352);
    }

    if(pSpd->k_bytes > 24)
    {
        do_fr(blk, ((unsigned char *) pSpd->l_key) + 384); do_fr(blk, ((unsigned char *) pSpd->l_key) + 416); 
        do_fr(blk, ((unsigned char *) pSpd->l_key) + 448); do_fr(blk, ((unsigned char *) pSpd->l_key) + 480);
    }

    kp = ((unsigned char *) pSpd->l_key) + 16 * pSpd->k_bytes;

    blk[ 0] ^= kp[ 0]; blk[ 1] += kp[ 1];
    blk[ 2] += kp[ 2]; blk[ 3] ^= kp[ 3]; 
    blk[ 4] ^= kp[ 4]; blk[ 5] += kp[ 5];
    blk[ 6] += kp[ 6]; blk[ 7] ^= kp[ 7]; 
    blk[ 8] ^= kp[ 8]; blk[ 9] += kp[ 9];
    blk[10] += kp[10]; blk[11] ^= kp[11]; 
    blk[12] ^= kp[12]; blk[13] += kp[13];
    blk[14] += kp[14]; blk[15] ^= kp[15]; 

    put_block(blk);
}

void Saferp_decrypt(const SAFERP_DATA *pSpd,const unsigned long *in_blk, unsigned long *out_blk)
{
	unsigned char  blk[16], *kp;

    get_block(blk);

    kp = ((unsigned char *) pSpd->l_key) + 16 * pSpd->k_bytes;

    blk[ 0] ^= kp[ 0]; blk[ 1] -= kp[ 1];
    blk[ 2] -= kp[ 2]; blk[ 3] ^= kp[ 3];
    blk[ 4] ^= kp[ 4]; blk[ 5] -= kp[ 5];
    blk[ 6] -= kp[ 6]; blk[ 7] ^= kp[ 7];
    blk[ 8] ^= kp[ 8]; blk[ 9] -= kp[ 9];
    blk[10] -= kp[10]; blk[11] ^= kp[11];
    blk[12] ^= kp[12]; blk[13] -= kp[13];
    blk[14] -= kp[14]; blk[15] ^= kp[15];

    if(pSpd->k_bytes > 24)
    {
        do_ir(blk, pSpd->l_key + 480); do_ir(blk, pSpd->l_key + 448); 
        do_ir(blk, pSpd->l_key + 416); do_ir(blk, pSpd->l_key + 384);
    }

    if(pSpd->k_bytes > 16)
    {
        do_ir(blk, pSpd->l_key + 352); do_ir(blk, pSpd->l_key + 320); 
        do_ir(blk, pSpd->l_key + 288); do_ir(blk, pSpd->l_key + 256);
    }

    do_ir(blk, pSpd->l_key + 224); do_ir(blk, pSpd->l_key + 192); 
    do_ir(blk, pSpd->l_key + 160); do_ir(blk, pSpd->l_key + 128);
    do_ir(blk, pSpd->l_key +  96); do_ir(blk, pSpd->l_key +  64); 
    do_ir(blk, pSpd->l_key +  32); do_ir(blk, pSpd->l_key);

    put_block(blk);
}
