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

#include "Hierocrypt3_data.h"

#define primitiveGF8 0x163

/*
* constant parameters
*/

const unsigned char sbox[256] =
{
  0x07, 0xFC, 0x55, 0x70, 0x98, 0x8E, 0x84, 0x4E, 0xBC, 0x75, 0xCE, 0x18,
  0x02, 0xE9, 0x5D, 0x80, 0x1C, 0x60, 0x78, 0x42, 0x9D, 0x2E, 0xF5, 0xE8,
  0xC6, 0x7A, 0x2F, 0xA4, 0xB2, 0x5F, 0x19, 0x87, 0x0B, 0x9B, 0x9C, 0xD3,
  0xC3, 0x77, 0x3D, 0x6F, 0xB9, 0x2D, 0x4D, 0xF7, 0x8C, 0xA7, 0xAC, 0x17,
  0x3C, 0x5A, 0x41, 0xC9, 0x29, 0xED, 0xDE, 0x27, 0x69, 0x30, 0x72, 0xA8,
  0x95, 0x3E, 0xF9, 0xD8, 0x21, 0x8B, 0x44, 0xD7, 0x11, 0x0D, 0x48, 0xFD,
  0x6A, 0x01, 0x57, 0xE5, 0xBD, 0x85, 0xEC, 0x1E, 0x37, 0x9F, 0xB5, 0x9A,
  0x7C, 0x09, 0xF1, 0xB1, 0x94, 0x81, 0x82, 0x08, 0xFB, 0xC0, 0x51, 0x0F,
  0x61, 0x7F, 0x1A, 0x56, 0x96, 0x13, 0xC1, 0x67, 0x99, 0x03, 0x5E, 0xB6,
  0xCA, 0xFA, 0x9E, 0xDF, 0xD6, 0x83, 0xCC, 0xA2, 0x12, 0x23, 0xB7, 0x65,
  0xD0, 0x39, 0x7D, 0x3B, 0xD5, 0xB0, 0xAF, 0x1F, 0x06, 0xC8, 0x34, 0xC5,
  0x1B, 0x79, 0x4B, 0x66, 0xBF, 0x88, 0x4A, 0xC4, 0xEF, 0x58, 0x3F, 0x0A,
  0x2C, 0x73, 0xD1, 0xF8, 0x6B, 0xE6, 0x20, 0xB8, 0x22, 0x43, 0xB3, 0x33,
  0xE7, 0xF0, 0x71, 0x7E, 0x52, 0x89, 0x47, 0x63, 0x0E, 0x6D, 0xE3, 0xBE,
  0x59, 0x64, 0xEE, 0xF6, 0x38, 0x5C, 0xF4, 0x5B, 0x49, 0xD4, 0xE0, 0xF3,
  0xBB, 0x54, 0x26, 0x2B, 0x00, 0x86, 0x90, 0xFF, 0xFE, 0xA6, 0x7B, 0x05,
  0xAD, 0x68, 0xA1, 0x10, 0xEB, 0xC7, 0xE2, 0xF2, 0x46, 0x8A, 0x6C, 0x14,
  0x6E, 0xCF, 0x35, 0x45, 0x50, 0xD2, 0x92, 0x74, 0x93, 0xE1, 0xDA, 0xAE,
  0xA9, 0x53, 0xE4, 0x40, 0xCD, 0xBA, 0x97, 0xA3, 0x91, 0x31, 0x25, 0x76,
  0x36, 0x32, 0x28, 0x3A, 0x24, 0x4C, 0xDB, 0xD9, 0x8D, 0xDC, 0x62, 0x2A,
  0xEA, 0x15, 0xDD, 0xC2, 0xA5, 0x0C, 0x04, 0x1D, 0x8F, 0xCB, 0xB4, 0x4F,
  0x16, 0xAB, 0xAA, 0xA0

};

const unsigned char isbox[256] =
{
  0xB8, 0x49, 0x0C, 0x69, 0xF6, 0xBF, 0x80, 0x00, 0x5B, 0x55, 0x8F, 0x20,
  0xF5, 0x45, 0xA4, 0x5F, 0xC3, 0x44, 0x74, 0x65, 0xCB, 0xF1, 0xFC, 0x2F,
  0x0B, 0x1E, 0x62, 0x84, 0x10, 0xF7, 0x4F, 0x7F, 0x96, 0x40, 0x98, 0x75,
  0xE8, 0xE2, 0xB6, 0x37, 0xE6, 0x34, 0xEF, 0xB7, 0x90, 0x29, 0x15, 0x1A,
  0x39, 0xE1, 0xE5, 0x9B, 0x82, 0xCE, 0xE4, 0x50, 0xAC, 0x79, 0xE7, 0x7B,
  0x30, 0x26, 0x3D, 0x8E, 0xDB, 0x32, 0x13, 0x99, 0x42, 0xCF, 0xC8, 0xA2,
  0x46, 0xB0, 0x8A, 0x86, 0xE9, 0x2A, 0x07, 0xFB, 0xD0, 0x5E, 0xA0, 0xD9,
  0xB5, 0x02, 0x63, 0x4A, 0x8D, 0xA8, 0x31, 0xAF, 0xAD, 0x0E, 0x6A, 0x1D,
  0x11, 0x60, 0xEE, 0xA3, 0xA9, 0x77, 0x87, 0x67, 0xC1, 0x38, 0x48, 0x94,
  0xCA, 0xA5, 0xCC, 0x27, 0x03, 0x9E, 0x3A, 0x91, 0xD3, 0x09, 0xE3, 0x25,
  0x12, 0x85, 0x19, 0xBE, 0x54, 0x7A, 0x9F, 0x61, 0x0F, 0x59, 0x5A, 0x71,
  0x06, 0x4D, 0xB9, 0x1F, 0x89, 0xA1, 0xC9, 0x41, 0x2C, 0xEC, 0x05, 0xF8,
  0xBA, 0xE0, 0xD2, 0xD4, 0x58, 0x3C, 0x64, 0xDE, 0x04, 0x68, 0x53, 0x21,
  0x22, 0x14, 0x6E, 0x51, 0xFF, 0xC2, 0x73, 0xDF, 0x1B, 0xF4, 0xBD, 0x2D,
  0x3B, 0xD8, 0xFE, 0xFD, 0x2E, 0xC0, 0xD7, 0x7E, 0x7D, 0x57, 0x1C, 0x9A,
  0xFA, 0x52, 0x6B, 0x76, 0x97, 0x28, 0xDD, 0xB4, 0x08, 0x4C, 0xA7, 0x88,
  0x5D, 0x66, 0xF3, 0x24, 0x8B, 0x83, 0x18, 0xC5, 0x81, 0x33, 0x6C, 0xF9,
  0x72, 0xDC, 0x0A, 0xCD, 0x78, 0x92, 0xD1, 0x23, 0xB1, 0x7C, 0x70, 0x43,
  0x3F, 0xEB, 0xD6, 0xEA, 0xED, 0xF2, 0x36, 0x6F, 0xB2, 0xD5, 0xC6, 0xA6,
  0xDA, 0x4B, 0x95, 0x9C, 0x17, 0x0D, 0xF0, 0xC4, 0x4E, 0x35, 0xAA, 0x8C,
  0x9D, 0x56, 0xC7, 0xB3, 0xAE, 0x16, 0xAB, 0x2B, 0x93, 0x3E, 0x6D, 0x5C,
  0x01, 0x47, 0xBC, 0xBB
};

const unsigned char hconst[4][4] =
{
  0x5a, 0x82, 0x79, 0x99,
  0x6e, 0xd9, 0xeb, 0xa1,
  0x8f, 0x1b, 0xbc, 0xdc,
  0xca, 0x62, 0xc1, 0xd6
};
const int gindex[6][2] =
{
  3, 0, 2, 1, 1, 3, 0, 2, 2, 3, 1, 0
};
const int kconst[3][10] =
{
  {0, 1, 2, 3, 3, 2, 1, -1, -1, -1},
  {1, 0, 3, 2, 2, 3, 0, 1, -1, -1},
  {4, 0, 2, 1, 3, 3, 1, 2, 0, -1}
};

const unsigned char mds[4][4] =
{
  {0xc4, 0x65, 0xc8, 0x8b},
  {0x8b, 0xc4, 0x65, 0xc8},
  {0xc8, 0x8b, 0xc4, 0x65},
  {0x65, 0xc8, 0x8b, 0xc4}
};

const unsigned char mds_inv[4][4] =
{
  {0x82, 0xc4, 0x34, 0xf6},
  {0xf6, 0x82, 0xc4, 0x34},
  {0x34, 0xf6, 0x82, 0xc4},
  {0xc4, 0x34, 0xf6, 0x82}
};

const int mdsh[4][4] =
{
  {0x5, 0x5, 0xa, 0xe},
  {0xe, 0x5, 0x5, 0xa},
  {0xa, 0xe, 0x5, 0x5},
  {0x5, 0xa, 0xe, 0x5}
};

const int mdsh_inv[4][4] =
{
  {0xb, 0xe, 0xe, 0x6},
  {0x6, 0xb, 0xe, 0xe},
  {0xe, 0x6, 0xb, 0xe},
  {0xe, 0xe, 0x6, 0xb}
};

int poly32_deg (unsigned long a)
{
  int n = -1;
  for (; a; a >>= 1)
    n++;
  return n;
}

unsigned long poly32_mul (unsigned long a, unsigned long b)
{
  unsigned long c = 0;

  for (; b; b >>= 1, a <<= 1)
    if (b & 1)
      c ^= a;

  return c;
}

unsigned long poly32_mod (unsigned long a, unsigned long b)
{
  int da = poly32_deg (a);
  int db = poly32_deg (b);
  unsigned long t;

  if (da < db)
    return a;

  if (da == db)
    return a ^ b;

  b <<= da - db;

  for (t = 1 << da; da >= db; da--)
    {
      if (a & t)
        a ^= b;
      b >>= 1;
      t >>= 1;
    }
  return a;
}

void hcrypt_mdsl (unsigned char * in, unsigned char * out)
{
  int i, j;
  unsigned long m;
  for (i = 0; i < 4; i++)
    {
      m = 0;
      for (j = 0; j < 4; j++)
        {
          m ^= poly32_mod (poly32_mul (mds[i][j], in[j]), primitiveGF8);
        }
      out[i] = (unsigned char) m;

    }
}

void hcrypt_xs (unsigned char * in, unsigned char * out, const unsigned char * k1, const unsigned char * k2)
{
  unsigned char t[4], u[4];
  int i;

  for (i = 0; i < 4; i++)
    u[i] = in[i] ^ k1[i];       /* key xOR */

  for (i = 0; i < 4; i++)
    t[i] = sbox[u[i]];          /* sbox */

  hcrypt_mdsl (t, u);           /* MDS_L */

  for (i = 0; i < 4; i++)
    t[i] = u[i] ^ k2[i];        /*keyxOR */
  for (i = 0; i < 4; i++)
    out[i] = sbox[t[i]];        /* sbox */
}

/*
* op. over GF(2^4)  in MDS_H
*/

void mdsh_mul (unsigned char * in, unsigned char * out, int x)
{
  int i;
  unsigned char u[4];
  for (i = 0; i < 4; i++)
    u[i] = 0;
  if (x & 1)
    {
      u[0] ^= in[0];
      u[1] ^= in[1];
      u[2] ^= in[2];
      u[3] ^= in[3];
    }
  if (x & 2)
    {
      u[0] ^= in[1];
      u[1] ^= in[2];
      u[2] ^= in[3] ^ in[0];
      u[3] ^= in[0];
    }
  if (x & 4)
    {
      u[0] ^= in[2];
      u[1] ^= in[3] ^ in[0];
      u[2] ^= in[0] ^ in[1];
      u[3] ^= in[1];
    }
  if (x & 8)
    {
      u[0] ^= in[0] ^ in[3];
      u[1] ^= in[1] ^ in[0];
      u[2] ^= in[2] ^ in[1];
      u[3] ^= in[2];
    }
  for (i = 0; i < 4; i++)
    out[i] = u[i];
}

void hcrypt_mdsh (unsigned char in[4][4], unsigned char out[4][4])
{
  int i, j, k;
  unsigned char tmp[4];
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      out[i][j] = 0;
  for (i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++)
        {
          mdsh_mul (&in[j][0], tmp, mdsh[i][j]);
          for (k = 0; k < 4; k++)
            out[i][k] ^= tmp[k];
        }
    }
}

void Hierocrypt3_encrypt(const HIEROCRYPT3_DATA *pHd,const unsigned char * in,unsigned char *out)
{
  unsigned char t[4][4], u[4][4];
  int i, j, r;
  int n;
  for (r = i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++, r++)
        {
          t[i][j] = in[r];
        }

    }

  n = 7;

  for (r = 0; r < n; r++)
    {
      hcrypt_xs (&t[0][0], &u[0][0], &pHd->ks[r][0][0], &pHd->ks[r][4][0]);
      hcrypt_xs (&t[1][0], &u[1][0], &pHd->ks[r][1][0], &pHd->ks[r][5][0]);
      hcrypt_xs (&t[2][0], &u[2][0], &pHd->ks[r][2][0], &pHd->ks[r][6][0]);
      hcrypt_xs (&t[3][0], &u[3][0], &pHd->ks[r][3][0], &pHd->ks[r][7][0]);
      hcrypt_mdsh (u, t);
    }

  hcrypt_xs (&t[0][0], &u[0][0], &pHd->ks[n][0][0], &pHd->ks[n][4][0]);
  hcrypt_xs (&t[1][0], &u[1][0], &pHd->ks[n][1][0], &pHd->ks[n][5][0]);
  hcrypt_xs (&t[2][0], &u[2][0], &pHd->ks[n][2][0], &pHd->ks[n][6][0]);
  hcrypt_xs (&t[3][0], &u[3][0], &pHd->ks[n][3][0], &pHd->ks[n][7][0]);

  for (r = i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++, r++)
        {
          out[r] = u[i][j] ^ pHd->ks[n + 1][i][j];
        }
    }
}

void hcrypt_imdsl (unsigned char * in, unsigned char * out)
{
  int i, j;
  unsigned long m;
  for (i = 0; i < 4; i++)
    {
      m = 0;
      for (j = 0; j < 4; j++)
        {
          m ^= poly32_mod (poly32_mul (mds_inv[i][j], in[j]), primitiveGF8);
        }
      out[i] = (unsigned char) m;
    }
}

void hcrypt_ixs (unsigned char * in, unsigned char * out, unsigned char * k1, unsigned char * k2)
{
  unsigned char t[4], u[4];
  int i;

  for (i = 0; i < 4; i++)
    u[i] = in[i] ^ k1[i];       /* key xOR */

  for (i = 0; i < 4; i++)
    t[i] = isbox[u[i]];         /* sbox */

  hcrypt_imdsl (t, u);          /*MDS_L */

  for (i = 0; i < 4; i++)
    t[i] = u[i] ^ k2[i];        /* key xOR */

  for (i = 0; i < 4; i++)
    out[i] = isbox[t[i]];       /* sbox */
}

void hcrypt_imdsh (unsigned char in[4][4], unsigned char out[4][4])
{
  int i, j, k;
  unsigned char tmp[4];
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      out[i][j] = 0;
  for (i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++)
        {
          mdsh_mul (&in[j][0], tmp, mdsh_inv[i][j]);

          for (k = 0; k < 4; k++)
            out[i][k] ^= tmp[k];

        }
    }
}

void Hierocrypt3_decrypt(const HIEROCRYPT3_DATA *pHd,const unsigned char * in,unsigned char *out)
{
  unsigned char t[4][4], u[4][4];
  int i, j, r;
  int n;
  for (r = i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++, r++)
        {
          t[i][j] = in[r];
        }
    }
  n = 7;

  for (r = 0; r < n; r++)
    {
      hcrypt_ixs (&t[0][0], &u[0][0], (unsigned char *) &pHd->dks[r][0][0], (unsigned char *) &pHd->dks[r][4][0]);
      hcrypt_ixs (&t[1][0], &u[1][0], (unsigned char *) &pHd->dks[r][1][0], (unsigned char *) &pHd->dks[r][5][0]);
      hcrypt_ixs (&t[2][0], &u[2][0], (unsigned char *) &pHd->dks[r][2][0], (unsigned char *) &pHd->dks[r][6][0]);
      hcrypt_ixs (&t[3][0], &u[3][0], (unsigned char *) &pHd->dks[r][3][0], (unsigned char *) &pHd->dks[r][7][0]);
      hcrypt_imdsh (u, t);
    }

  hcrypt_ixs (&t[0][0], &u[0][0], (unsigned char *) &pHd->dks[r][0][0], (unsigned char *) &pHd->dks[n][4][0]);
  hcrypt_ixs (&t[1][0], &u[1][0], (unsigned char *) &pHd->dks[r][1][0], (unsigned char *) &pHd->dks[n][5][0]);
  hcrypt_ixs (&t[2][0], &u[2][0], (unsigned char *) &pHd->dks[r][2][0], (unsigned char *) &pHd->dks[n][6][0]);
  hcrypt_ixs (&t[3][0], &u[3][0], (unsigned char *) &pHd->dks[r][3][0], (unsigned char *) &pHd->dks[n][7][0]);

  for (r = i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++, r++)
        {
          out[r] = u[i][j] ^ pHd->dks[n + 1][i][j];

        }
    }
}

void hcrypt_keyf (unsigned char * in, unsigned char * fout, unsigned char * fkey)
{
  int i;
  /* F in */
  for (i = 0; i < 8; i++)
    fout[i] = sbox[in[i] ^ fkey[i]];

  /* P (32) */

  fout[0] ^= fout[4];
  fout[1] ^= fout[5];
  fout[2] ^= fout[6];
  fout[3] ^= fout[7];
  fout[4] ^= fout[2];
  fout[5] ^= fout[3];
  fout[6] ^= fout[0];
  fout[7] ^= fout[1];
}

void swap_key (unsigned char * l, unsigned char * r)
{
  unsigned char t;
  int i;
  for (i = 0; i < 8; i++)
    {
      t = l[i];
      l[i] = r[i];
      r[i] = t;
    }
}

void hcrypt_keyp (unsigned char k[4][8], unsigned char kout[8][4], int index)
{
  int i;
  unsigned char fout[8];
  /* P(32) */
  for (i = 0; i < 4; i++)
    {
      k[2][i] ^= k[3][i];
      k[2][i + 4] ^= k[3][i + 4];
    }
  for (i = 0; i < 4; i++)
    {
      k[3][i] ^= k[2][i + 4];
      k[3][i + 4] ^= k[2][i];
    }

  /* multiple */
  mdsh_mul (&k[2][0], &k[2][0], 5);
  mdsh_mul (&k[2][4], &k[2][4], 0xe);
  mdsh_mul (&k[3][0], &k[3][0], 5);
  mdsh_mul (&k[3][4], &k[3][4], 0xe);

  for (i = 0; i < 4; i++)
    k[2][i] ^= hconst[gindex[index][0]][i];

  for (i = 0; i < 4; i++)
    k[2][i + 4] ^= hconst[gindex[index][1]][i];

  hcrypt_keyf (&k[1][0], fout, &k[2][0]);

  for (i = 0; i < 8; i++)
    k[0][i] ^= fout[i];         /* L xor f(R) */

  for (i = 0; i < 4; i++)
    {
      kout[0][i] = k[0][i];
      kout[1][i] = k[0][i + 4];
    }

  for (i = 0; i < 4; i++)
    {
      kout[2][i] = fout[i] ^ k[2][i];
      kout[3][i] = fout[i + 4] ^ k[2][i + 4];
    }

  for (i = 0; i < 4; i++)
    {
      kout[4][i] = fout[i] ^ k[3][i];
      kout[5][i] = fout[i + 4] ^ k[3][i + 4];
    }

  for (i = 0; i < 4; i++)
    {
      kout[6][i] = k[1][i] ^ k[3][i];
      kout[7][i] = k[1][i + 4] ^ k[3][i + 4];
    }

  swap_key (&k[0][0], &k[1][0]);
}

void hcrypt_keyc (unsigned char k[4][8], unsigned char kout[8][4], int index)
{
  int i;
  unsigned char fout[8];
  swap_key (&k[0][0], &k[1][0]);
  hcrypt_keyf (&k[1][0], fout, &k[2][0]);

  for (i = 0; i < 8; i++)
    k[0][i] ^= fout[i];

  for (i = 0; i < 4; i++)
    {
      kout[0][i] = k[0][i] ^ k[2][i];
      kout[1][i] = k[0][i + 4] ^ k[2][i + 4];
    }

  for (i = 0; i < 4; i++)
    k[2][i] ^= hconst[gindex[index][0]][i];


  for (i = 0; i < 4; i++)
    k[2][i + 4] ^= hconst[gindex[index][1]][i];

  mdsh_mul (&k[2][0], &k[2][0], 0xb);
  mdsh_mul (&k[2][4], &k[2][4], 0x3);
  mdsh_mul (&k[3][0], &k[3][0], 0xb);
  mdsh_mul (&k[3][4], &k[3][4], 0x3);

  for (i = 0; i < 4; i++)
    {
      kout[2][i] = fout[i] ^ k[2][i];
      kout[3][i] = fout[i + 4] ^ k[2][i + 4];
    }

  for (i = 0; i < 4; i++)
    {
      kout[4][i] = fout[i] ^ k[3][i];
      kout[5][i] = fout[i + 4] ^ k[3][i + 4];
    }

  for (i = 0; i < 4; i++)
    {
      kout[6][i] = k[1][i] ^ k[3][i];
      kout[7][i] = k[1][i + 4] ^ k[3][i + 4];
    }


  for (i = 0; i < 4; i++)
    {
      k[3][i] ^= k[2][i + 4];
      k[3][i + 4] ^= k[2][i];
    }
  for (i = 0; i < 4; i++)
    {
      k[2][i] ^= k[3][i];
      k[2][i + 4] ^= k[3][i + 4];
    }
}

void Hierocrypt3_set_key(HIEROCRYPT3_DATA *pHd,const unsigned char* key)
{
  unsigned char k[4][8];
  unsigned char fout[8];
  int i, j, pos, r, n;

  pos = 0;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 8; j++)
      k[i][j] = key[pos++];


  mdsh_mul (&k[2][0], &k[2][0], 5);
  mdsh_mul (&k[2][4], &k[2][4], 0xe);
  mdsh_mul (&k[3][0], &k[3][0], 5);
  mdsh_mul (&k[3][4], &k[3][4], 0xe);

  for (i = 0; i < 4; i++)
    k[2][i] ^= hconst[gindex[5][0]][i];

  for (i = 0; i < 4; i++)
    k[2][i + 4] ^= hconst[gindex[5][1]][i];

  hcrypt_keyf (&k[1][0], fout, &k[2][0]);

  for (i = 0; i < 8; i++)
    k[0][i] ^= fout[i];         /*L+f(R) */

  swap_key (&k[0][0], &k[1][0]);

  for (r = 0; r < 5; r++)
    hcrypt_keyp (&k[0], &pHd->ks[r][0], kconst[2][r]);

  for (r = 5; r < 9; r++)
    hcrypt_keyc (&k[0], &pHd->ks[r][0], kconst[2][r]);

  r = 8;

  for (j = 0; j < 4; j++)
    {
      for (n = 0; n < 4; n++)
        {
          pHd->dks[0][j][n] = pHd->ks[r][j][n];
        }
    }

  for (i = 1; i < r; i++)
    {
      for (j = 4; j < 8; j++)
        {
          hcrypt_imdsl (&pHd->ks[r - i][j][0], &pHd->dks[i - 1][j][0]);
        }
      hcrypt_imdsh (&pHd->ks[r - i][0], &pHd->dks[i][0]);
    }

  for (j = 4; j < 8; j++)
    {
      hcrypt_imdsl (&pHd->ks[0][j][0], &pHd->dks[r - 1][j][0]);
    }

  for (j = 0; j < 4; j++)
    {
      for (n = 0; n < 4; n++)
        {
          pHd->dks[r][j][n] = pHd->ks[0][j][n];
        }
    }
}
