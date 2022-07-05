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

#ifndef TWOFISH_DATA_H
#define TWOFISH_DATA_H

// ** Thread-safe implementation

// ** Twofish cipher
// ** 128bit block size
// ** 256bit key

#define Q_TABLES
#define M_TABLE
#define MK_TABLE
#define ONE_STEP

typedef struct {
	DWORD	k_len;
	DWORD	l_key[40];
	DWORD	s_key[4];

	#ifdef  Q_TABLES
	DWORD	qt_gen;
	BYTE	q_tab[2][256];
	#endif

	#ifdef  M_TABLE
	DWORD	mt_gen;
	DWORD	m_tab[4][256];
	#endif

	#ifdef  MK_TABLE
	#ifdef  ONE_STEP
	DWORD	mk_tab[4][256];
	#else
	BYTE	sb[4][256];
	#endif
	#endif
} TWOFISH_DATA;

#endif
