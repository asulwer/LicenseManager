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

#ifndef MULTIBASE_DATA_H
#define MULTIBASE_DATA_H

// ** Thread-safe implementation

// ** Wrapping of implemented ciphers
// ** 128bit block size	(DATA_BLOCK_SIZE<<3)
// ** 2x 256bit keys	(MAX_PASSW_SIZE<<3)
// ** Electronic Codebook (ECB)

#include "Anubis_data.h"
#include "Clefia_data.h"
#include "Frog_data.h"
#include "Hierocrypt3_data.h"
#include "Idea_nxt128_data.h"
#include "Rijndael_data.h"
#include "Saferp_data.h"
#include "Sha512_data.h"
#include "Speed_data.h"
#include "Twofish_data.h"

typedef struct {
	ANUBIS_DATA	key;
} ANUBIS_STATIC_DATA;

typedef struct {
	BYTE	key[34*16];
} CAMELLIA_STATIC_DATA;

typedef struct {	
	DWORD	key[96];
} CAST256_STATIC_DATA;

typedef struct {
	CLEFIA_DATA	key;
} CLEFIA_STATIC_DATA;

typedef struct {
	FROG_DATA	key;
} FROG_STATIC_DATA;

typedef struct {
	HIEROCRYPT3_DATA	key;
} HIEROCRYPT3_STATIC_DATA;

typedef struct {
	nxt128_ctx	key;
} IDEANXT128_STATIC_DATA;

typedef struct {
	DWORD	key[40];
	DWORD	vk[47];
} MARS_STATIC_DATA;

typedef struct {
	DWORD	key[44];
} RC6_STATIC_DATA;

typedef struct {
	DWORD	key[RKLENGTH(256)];
	int		nrounds;
} RIJNDAEL_STATIC_DATA;

typedef struct {
	SAFERP_DATA	key;
} SAFERP_STATIC_DATA;

typedef struct {
	DWORD	key[64];
} SC2000_STATIC_DATA;

typedef struct {
	DWORD	key[140];
} SERPENT_STATIC_DATA;

typedef struct {
	speed_ikey	key;
} SPEED_STATIC_DATA;

typedef struct {
	TWOFISH_DATA	key;
} TWOFISH_STATIC_DATA;

typedef struct {
	BYTE	key[288];
} UNICORNA_STATIC_DATA;

typedef struct {
	ANUBIS_STATIC_DATA		anubis;
	CAMELLIA_STATIC_DATA	camellia;
	CAST256_STATIC_DATA		cast256;
	CLEFIA_STATIC_DATA		clefia;
	FROG_STATIC_DATA		frog;
	HIEROCRYPT3_STATIC_DATA	hc3;
	IDEANXT128_STATIC_DATA	ideanxt128;
	MARS_STATIC_DATA		mars;
	RC6_STATIC_DATA			rc6;
	RIJNDAEL_STATIC_DATA	rijndael_enc;
	RIJNDAEL_STATIC_DATA	rijndael_dec;
	SAFERP_STATIC_DATA		saferp;
	SC2000_STATIC_DATA		sc2000;
	SERPENT_STATIC_DATA		serpent;
	SPEED_STATIC_DATA		speed;
	TWOFISH_STATIC_DATA		twofish;
	UNICORNA_STATIC_DATA	unicorn;
} MULTI_STATIC_DATA;

typedef enum {
	ANUBIS_ALG=0,
	CAMELLIA_ALG,
	CAST256_ALG,
	CLEFIA_ALG,
	FROG_ALG,
	HIEROCRYPT3_ALG,
	IDEANXT128_ALG,
	MARS_ALG,
	RC6_ALG,
	RIJNDAEL_ALG,
	SAFERP_ALG,
	SC2000_ALG,
	SERPENT_ALG,
	SPEED_ALG,
	TWOFISH_ALG,
	UNICORNA_ALG
} ENUM_ALG;

#endif
