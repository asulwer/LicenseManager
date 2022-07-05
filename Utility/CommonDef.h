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

#ifndef COMMON_DATA_H
#define COMMON_DATA_H

// ** Library common definitions

typedef enum {
	OBFUNC_OK=0,
	OBFUNC_OUTOFMEM,
	OBFUNC_INVALID,
	OBFUNC_STOP
} OBFUNC_RETV;

typedef	void (*perc_callback_t)(void *desc,BYTE perc);
typedef	BOOL (*test_callback_t)(void *desc);

#define	MIN_PASSW_SIZE			8
#define	MAX_PASSW_SIZE			32 //this is the max unless we change some of the algorithms to accept larger passwords

#define	MAX_ALG					16

#define	DATA_BLOCK_SIZE			16

#define	IV_BLOCK_SIZE			(MAX_ALG*DATA_BLOCK_SIZE)

#define	TEST_AND_FREE(argX)	if(argX) { free(argX); argX=NULL; }

#endif
