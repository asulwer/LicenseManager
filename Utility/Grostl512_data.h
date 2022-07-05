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

#ifndef GROSTL512_DATA_H
#define GROSTL512_DATA_H

// ** Thread-safe implementation

// ** Grostl hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

#define ROWS 8
#define LENGTHFIELDLEN ROWS
#define COLS1024 16
#define SIZE1024 (ROWS*COLS1024)
#define ROUNDS1024 14

typedef struct {
  BYTE chaining[ROWS][COLS1024]; /* the actual state */
  QWORD block_counter; /* block counter */
  int hashbitlen; /* output length */
  BYTE buffer[SIZE1024]; /* block buffer */
  int buf_ptr; /* buffer pointer */
  int bits_in_last_byte; /* number of bits in incomplete byte */
  int columns; /* number of columns in state */
  int rounds; /* number of rounds in P and Q */
  int statesize; /* size of state (ROWS*columns) */
} GROSTL512_DATA;

#endif
