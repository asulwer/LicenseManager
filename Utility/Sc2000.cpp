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

#define T32(x)  ((x) & ONE32)
#define ONE32 0xffffffffU
#define ROTL32(v,n) (T32((v)<<(n))|((v)>>(32-(n))))

/* S-boxes  (6-bit)(5-bit)  */
const unsigned long S6[64] = {
  47, 59, 25, 42, 15, 23, 28, 39, 26, 38, 36, 19, 60, 24, 29, 56,
  37, 63, 20, 61, 55, 2, 30, 44, 9, 10, 6, 22, 53, 48, 51, 11,
  62, 52, 35, 18, 14, 46, 0, 54, 17, 40, 27, 4, 31, 8, 5, 12,
  3, 16, 41, 34, 33, 7, 45, 49, 50, 58, 1, 21, 43, 57, 32, 13
};
const unsigned long S5[32] = {
  20, 26, 7, 31, 19, 12, 10, 15, 22, 30, 13, 14, 4, 24, 9, 18,
  27, 11, 1, 21, 6, 16, 2, 28, 23, 5, 8, 3, 0, 17, 29, 25
};

/* Bit-slice S-Box (4-bit)*/
/* 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15*/
const unsigned long S4[16] = { 2, 5, 10, 12, 7, 15, 1, 11, 13, 6, 0, 9, 4, 8, 3, 14 };
const unsigned long S4i[16] = { 10, 6, 0, 14, 12, 1, 9, 4, 13, 11, 2, 7, 3, 8, 15, 5 };

/* M-Table */
const unsigned long M[32] = {
  0xd0c19225, 0xa5a2240a, 0x1b84d250, 0xb728a4a1,
  0x6a704902, 0x85dddbe6, 0x766ff4a4, 0xecdfe128,
  0xafd13e94, 0xdf837d09, 0xbb27fa52, 0x695059ac,
  0x52a1bb58, 0xcc322f1d, 0x1844565b, 0xb4a8acf6,
  0x34235438, 0x6847a851, 0xe48c0cbb, 0xcd181136,
  0x9a112a0c, 0x43ec6d0e, 0x87d8d27d, 0x487dc995,
  0x90fb9b4b, 0xa1f63697, 0xfc513ed9, 0x78a37d93,
  0x8d16c5df, 0x9e0c8bbe, 0x3c381f7c, 0xe9fb0779
};

#define _A_ 0
#define _B_ 1
#define _C_ 2
#define _D_ 3
#define _X_ 0
#define _Y_ 1
#define _Z_ 2
#define _W_ 3
/* Order Table */
const unsigned long Order[12][4] = {
  {_A_, _B_, _C_, _D_},
  {_B_, _A_, _D_, _C_},
  {_C_, _D_, _A_, _B_},
  {_D_, _C_, _B_, _A_},
  {_A_, _C_, _D_, _B_},
  {_B_, _D_, _C_, _A_},
  {_C_, _A_, _B_, _D_},
  {_D_, _B_, _A_, _C_},
  {_A_, _D_, _B_, _C_},
  {_B_, _C_, _A_, _D_},
  {_C_, _B_, _D_, _A_},
  {_D_, _A_, _C_, _B_}
};

/* Index Table */
const unsigned long Index[9][4] = {
  {0, 0, 0, 0},
  {1, 1, 1, 1},
  {2, 2, 2, 2},
  {0, 1, 0, 1},
  {1, 2, 1, 2},
  {2, 0, 2, 0},
  {0, 2, 0, 2},
  {1, 0, 1, 0},
  {2, 1, 2, 1}
};

void S_func (unsigned long a, unsigned long * b)
{
  unsigned long q, r, s, t, u, v;
  q = (a >> 26) & 0x3F;
  r = (a >> 21) & 0x1F;
  s = (a >> 16) & 0x1F;
  t = (a >> 11) & 0x1F;
  u = (a >> 6) & 0x1F;
  v = (a >> 0) & 0x3F;
  q = S6[q];
  r = S5[r];
  s = S5[s];
  t = S5[t];
  u = S5[u];
  v = S6[v];
  *b = (q << 26);
  *b |= (r << 21);
  *b |= (s << 16);
  *b |= (t << 11);
  *b |= (u << 6);
  *b |= (v << 0);
  return;
}

void M_func (unsigned long a, unsigned long * b)
{
  int i;
  *b = 0;
  for (i = 31; i >= 0; i--)
    {
      if (a & 1)
        *b ^= M[i];
      a >>= 1;
    }
  return;
}

void L_func (unsigned long a, unsigned long b, unsigned long mask, unsigned long * c, unsigned long * d)
{
  unsigned long s, t;
  unsigned long imask = (mask ^ 0xFFFFFFFF);
  s = a & mask;
  t = b & imask;
  *c = s ^ b;
  *d = t ^ a;
  return;
}

void F_func (unsigned long a, unsigned long b, unsigned long mask, unsigned long * c, unsigned long * d)
{
  unsigned long s, t;
  S_func (a, &s);
  M_func (s, &s);
  S_func (b, &t);
  M_func (t, &t);
  L_func (s, t, mask, c, d);
  return;
}

void R_func (unsigned long a, unsigned long b, unsigned long c, unsigned long d, unsigned long mask, unsigned long * e,
        unsigned long * f, unsigned long * g, unsigned long * h)
{
  unsigned long s, t;
  F_func (c, d, mask, &s, &t);
  *e = a ^ s;
  *f = b ^ t;
  *g = c;
  *h = d;
  return;
}

void B_func (unsigned long a, unsigned long b, unsigned long c, unsigned long d, unsigned long * e, unsigned long * f, unsigned long * g, unsigned long * h)
{
  unsigned long s, t;
  unsigned long m = 1;
  int i;
  *e = 0;
  *f = 0;
  *g = 0;
  *h = 0;
  for (i = 0; i < 32; i++)
    {
      /*T_func */
      s = 0;
      if (a & m)
        s |= 8;
      if (b & m)
        s |= 4;
      if (c & m)
        s |= 2;
      if (d & m)
        s |= 1;
      t = S4[s];

      if (t & 8)
        *e |= m;
      if (t & 4)
        *f |= m;
      if (t & 2)
        *g |= m;
      if (t & 1)
        *h |= m;
      m <<= 1;
    }
  return;
}

void Bi_func (unsigned long a, unsigned long b, unsigned long c, unsigned long d, unsigned long * e, unsigned long * f, unsigned long * g, unsigned long * h)
{
  unsigned long s, t;
  unsigned long m = 1;
  int i;
  *e = 0;
  *f = 0;
  *g = 0;
  *h = 0;
  for (i = 0; i < 32; i++)
    {
      s = 0;
      /*T_func */
      if (a & m)
        s |= 8;
      if (b & m)
        s |= 4;
      if (c & m)
        s |= 2;
      if (d & m)
        s |= 1;
      t = S4i[s];

      if (t & 8)
        *e |= m;
      if (t & 4)
        *f |= m;
      if (t & 2)
        *g |= m;
      if (t & 1)
        *h |= m;
      m <<= 1;
    }
  return;
}

void I_func (unsigned long a, unsigned long b, unsigned long c, unsigned long d, unsigned long ka, unsigned long kb,
        unsigned long kc, unsigned long kd, unsigned long * e, unsigned long * f, unsigned long * g, unsigned long * h)
{
  *e = a ^ ka;
  *f = b ^ kb;
  *g = c ^ kc;
  *h = d ^ kd;
  return;
}

unsigned long make_one_imkey (unsigned long k1, unsigned long k2, unsigned long i, unsigned long j)
{
  unsigned long ka, kb, m;
  ka = k1;
  S_func (ka, &ka);
  M_func (ka, &ka);
  kb = k2;
  S_func (kb, &kb);
  M_func (kb, &kb);
  m = 4 * i + j;
  S_func (m, &m);
  M_func (m, &m);
  ka += m;
  ka &= 0xFFFFFFFF;
  kb *= (i + 1);
  kb &= 0xFFFFFFFF;
  ka ^= kb;
  S_func (ka, &ka);
  M_func (ka, &ka);
  return (ka);
}

void make_imkeys (const unsigned long * ukey, unsigned long keylength, unsigned long imkey[4][3])
{
  unsigned long kl, k2, k3, k4, k5, k6, k7, k8;
  unsigned long i;
  kl = ukey[0];
  k2 = ukey[1];
  k3 = ukey[2];
  k4 = ukey[3];

  k5 = ukey[4];
  k6 = ukey[5];
  k7 = ukey[6];
  k8 = ukey[7];

  for (i = 0; i < 3; i++)
    {
      imkey[_A_][i] = make_one_imkey (kl, k2, i, 0);
      imkey[_B_][i] = make_one_imkey (k3, k4, i, 1);
      imkey[_C_][i] = make_one_imkey (k5, k6, i, 2);
      imkey[_D_][i] = make_one_imkey (k7, k8, i, 3);
    }
}

unsigned long make_one_ekey (unsigned long imkey[4][3], unsigned long t, unsigned long s)
{
  unsigned long x, y, z, w;
  x = imkey[Order[t][_X_]][Index[s][_X_]];
  y = imkey[Order[t][_Y_]][Index[s][_Y_]];
  z = imkey[Order[t][_Z_]][Index[s][_Z_]];
  w = imkey[Order[t][_W_]][Index[s][_W_]];
  x = ROTL32 (x, 1);
  x += y;
  x &= 0xFFFFFFFF;
  z = ROTL32 (z, 1);
  z -= w;
  z &= 0xFFFFFFFF;
  z = ROTL32 (z, 1);
  x ^= z;
  return (x);
}

void make_ekeys (unsigned long imkey[4][3], unsigned long num_ekey, unsigned long * ekey)
{
  unsigned long n, t, s;
  for (n = 0; n < num_ekey; n++)
    {
      t = (n + (n / 36)) % 12;
      s = n % 9;
      ekey[n] = make_one_ekey (imkey, t, s);
    }
}

void Sc2000_set_key(unsigned long *ek,const unsigned long *in_key)
{
  unsigned long imkey[4][3];

  /* make intermediate key */
  make_imkeys (in_key, 256, imkey);

  /* make extend key */
  make_ekeys (imkey, 64, ek);
}

void Sc2000_encrypt(const unsigned long *ek,const unsigned long *in,unsigned long *out)
{
  unsigned long a, b, c, d;
  a = in[0];
  b = in[1];
  c = in[2];
  d = in[3];
  I_func (a, b, c, d, ek[0], ek[1], ek[2], ek[3], &a, &b, &c, &d);
  B_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[4], ek[5], ek[6], ek[7], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x55555555, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x55555555, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[8], ek[9], ek[10], ek[11], &a, &b, &c, &d);
  B_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[12], ek[13], ek[14], ek[15], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x33333333, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x33333333, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[16], ek[17], ek[18], ek[19], &a, &b, &c, &d);
  B_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[20], ek[21], ek[22], ek[23], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x55555555, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x55555555, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[24], ek[25], ek[26], ek[27], &a, &b, &c, &d);
  B_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[28], ek[29], ek[30], ek[31], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x33333333, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x33333333, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[32], ek[33], ek[34], ek[35], &a, &b, &c, &d);
  B_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[36], ek[37], ek[38], ek[39], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x55555555, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x55555555, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[40], ek[41], ek[42], ek[43], &a, &b, &c, &d);
  B_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[44], ek[45], ek[46], ek[47], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x33333333, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x33333333, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[48], ek[49], ek[50], ek[51], &a, &b, &c, &d);
  B_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[52], ek[53], ek[54], ek[55], &a, &b, &c, &d);

  out[0] = a;
  out[1] = b;
  out[2] = c;
  out[3] = d;
}

void Sc2000_decrypt(const unsigned long *ek,const unsigned long *in,unsigned long *out)
{
  unsigned long a, b, c, d;

  a = in[0];
  b = in[1];
  c = in[2];
  d = in[3];

  I_func (a, b, c, d, ek[52], ek[53], ek[54], ek[55], &a, &b, &c, &d);
  Bi_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[48], ek[49], ek[50], ek[51], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x33333333, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x33333333, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[44], ek[45], ek[46], ek[47], &a, &b, &c, &d);
  Bi_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[40], ek[41], ek[42], ek[43], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x55555555, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x55555555, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[36], ek[37], ek[38], ek[39], &a, &b, &c, &d);
  Bi_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[32], ek[33], ek[34], ek[35], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x33333333, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x33333333, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[28], ek[29], ek[30], ek[31], &a, &b, &c, &d);
  Bi_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[24], ek[25], ek[26], ek[27], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x55555555, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x55555555, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[20], ek[21], ek[22], ek[23], &a, &b, &c, &d);
  Bi_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[16], ek[17], ek[18], ek[19], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x33333333, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x33333333, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[12], ek[13], ek[14], ek[15], &a, &b, &c, &d);
  Bi_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[8], ek[9], ek[10], ek[11], &a, &b, &c, &d);
  R_func (a, b, c, d, 0x55555555, &a, &b, &c, &d);
  R_func (c, d, a, b, 0x55555555, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[4], ek[5], ek[6], ek[7], &a, &b, &c, &d);
  Bi_func (a, b, c, d, &a, &b, &c, &d);
  I_func (a, b, c, d, ek[0], ek[1], ek[2], ek[3], &a, &b, &c, &d);

  out[0] = a;
  out[1] = b;
  out[2] = c;
  out[3] = d;
}
