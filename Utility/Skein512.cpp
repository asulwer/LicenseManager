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

#include "Skein512_data.h"

//// DUMMY-DEBUG
#define Skein_Show_Block(bits,ctx,X,blkPtr,wPtr,ksEvenPtr,ksOddPtr)
#define Skein_Show_Round(bits,ctx,r,X)
#define Skein_Show_R_Ptr(bits,ctx,r,X_ptr)
#define Skein_Show_Final(bits,ctx,cnt,outPtr)
#define Skein_Show_Key(bits,ctx,key,keyBytes)
////

#define SKEIN_T1_BIT(BIT)       ((BIT) - 64)            /* offset 64 because it's the second word  */
                                
#define SKEIN_T1_POS_TREE_LVL   SKEIN_T1_BIT(112)       /* bits 112..118: level in hash tree       */
#define SKEIN_T1_POS_BIT_PAD    SKEIN_T1_BIT(119)       /* bit  119     : partial final input byte */
#define SKEIN_T1_POS_BLK_TYPE   SKEIN_T1_BIT(120)       /* bits 120..125: type field               */
#define SKEIN_T1_POS_FIRST      SKEIN_T1_BIT(126)       /* bits 126     : first block flag         */
#define SKEIN_T1_POS_FINAL      SKEIN_T1_BIT(127)       /* bit  127     : final block flag         */

#define SKEIN_T1_FLAG_FIRST     (((QWORD)  1 ) << SKEIN_T1_POS_FIRST)
#define SKEIN_T1_FLAG_FINAL     (((QWORD)  1 ) << SKEIN_T1_POS_FINAL)
#define SKEIN_T1_FLAG_BIT_PAD   (((QWORD)  1 ) << SKEIN_T1_POS_BIT_PAD)

#define SKEIN_T1_TREE_LVL_MASK  (((QWORD)0x7F) << SKEIN_T1_POS_TREE_LVL)
#define SKEIN_T1_TREE_LEVEL(n)  (((QWORD) (n)) << SKEIN_T1_POS_TREE_LVL)

#define SKEIN_BLK_TYPE_KEY      ( 0)                    /* key, for MAC and KDF */
#define SKEIN_BLK_TYPE_CFG      ( 4)                    /* configuration block */
#define SKEIN_BLK_TYPE_PERS     ( 8)                    /* personalization string */
#define SKEIN_BLK_TYPE_PK       (12)                    /* public key (for digital signature hashing) */
#define SKEIN_BLK_TYPE_KDF      (16)                    /* key identifier for KDF */
#define SKEIN_BLK_TYPE_NONCE    (20)                    /* nonce for PRNG */
#define SKEIN_BLK_TYPE_MSG      (48)                    /* message processing */
#define SKEIN_BLK_TYPE_OUT      (63)                    /* output stage */
#define SKEIN_BLK_TYPE_MASK     (63)                    /* bit field mask */

#define SKEIN_T1_BLK_TYPE(T)   (((QWORD) (SKEIN_BLK_TYPE_##T)) << SKEIN_T1_POS_BLK_TYPE)
#define SKEIN_T1_BLK_TYPE_KEY   SKEIN_T1_BLK_TYPE(KEY)  /* key, for MAC and KDF */
#define SKEIN_T1_BLK_TYPE_CFG   SKEIN_T1_BLK_TYPE(CFG)  /* configuration block */
#define SKEIN_T1_BLK_TYPE_PERS  SKEIN_T1_BLK_TYPE(PERS) /* personalization string */
#define SKEIN_T1_BLK_TYPE_PK    SKEIN_T1_BLK_TYPE(PK)   /* public key (for digital signature hashing) */
#define SKEIN_T1_BLK_TYPE_KDF   SKEIN_T1_BLK_TYPE(KDF)  /* key identifier for KDF */
#define SKEIN_T1_BLK_TYPE_NONCE SKEIN_T1_BLK_TYPE(NONCE)/* nonce for PRNG */
#define SKEIN_T1_BLK_TYPE_MSG   SKEIN_T1_BLK_TYPE(MSG)  /* message processing */
#define SKEIN_T1_BLK_TYPE_OUT   SKEIN_T1_BLK_TYPE(OUT)  /* output stage */
#define SKEIN_T1_BLK_TYPE_MASK  SKEIN_T1_BLK_TYPE(MASK) /* field bit mask */

#define SKEIN_T1_BLK_TYPE_CFG_FINAL       (SKEIN_T1_BLK_TYPE_CFG | SKEIN_T1_FLAG_FINAL)
#define SKEIN_T1_BLK_TYPE_OUT_FINAL       (SKEIN_T1_BLK_TYPE_OUT | SKEIN_T1_FLAG_FINAL)

#define SKEIN_VERSION           (1)
#define SKEIN_ID_STRING_LE      (0x33414853)            /* "SHA3" (little-endian)*/

#define SKEIN_MK_64(hi32,lo32)  ((((QWORD) hi32) << 32)+(lo32))
#define SKEIN_SCHEMA_VER        SKEIN_MK_64(SKEIN_VERSION,SKEIN_ID_STRING_LE)
#define SKEIN_KS_PARITY         SKEIN_MK_64(0x1BD11BDA,0xA9FC1A22)

#define SKEIN_CFG_STR_LEN       (4*8)

#define SKEIN_CFG_TREE_LEAF_SIZE_POS  ( 0)
#define SKEIN_CFG_TREE_NODE_SIZE_POS  ( 8)
#define SKEIN_CFG_TREE_MAX_LEVEL_POS  (16)

#define SKEIN_CFG_TREE_LEAF_SIZE_MSK  (((QWORD) 0xFF) << SKEIN_CFG_TREE_LEAF_SIZE_POS)
#define SKEIN_CFG_TREE_NODE_SIZE_MSK  (((QWORD) 0xFF) << SKEIN_CFG_TREE_NODE_SIZE_POS)
#define SKEIN_CFG_TREE_MAX_LEVEL_MSK  (((QWORD) 0xFF) << SKEIN_CFG_TREE_MAX_LEVEL_POS)

#define SKEIN_CFG_TREE_INFO(leaf,node,maxLvl)                   \
    ( (((QWORD)(leaf  )) << SKEIN_CFG_TREE_LEAF_SIZE_POS) |    \
      (((QWORD)(node  )) << SKEIN_CFG_TREE_NODE_SIZE_POS) |    \
      (((QWORD)(maxLvl)) << SKEIN_CFG_TREE_MAX_LEVEL_POS) )

#define SKEIN_CFG_TREE_INFO_SEQUENTIAL SKEIN_CFG_TREE_INFO(0,0,0) /* use as treeInfo in InitExt() call for sequential processing */

#define Skein_Get_Tweak(ctxPtr,TWK_NUM)         ((ctxPtr)->h.T[TWK_NUM])
#define Skein_Set_Tweak(ctxPtr,TWK_NUM,tVal)    {(ctxPtr)->h.T[TWK_NUM] = (tVal);}

#define Skein_Get_T0(ctxPtr)    Skein_Get_Tweak(ctxPtr,0)
#define Skein_Get_T1(ctxPtr)    Skein_Get_Tweak(ctxPtr,1)
#define Skein_Set_T0(ctxPtr,T0) Skein_Set_Tweak(ctxPtr,0,T0)
#define Skein_Set_T1(ctxPtr,T1) Skein_Set_Tweak(ctxPtr,1,T1)

#define Skein_Set_T0_T1(ctxPtr,T0,T1)           \
    {                                           \
    Skein_Set_T0(ctxPtr,(T0));                  \
    Skein_Set_T1(ctxPtr,(T1));                  \
    }

#define Skein_Set_Type(ctxPtr,BLK_TYPE)         \
    Skein_Set_T1(ctxPtr,SKEIN_T1_BLK_TYPE_##BLK_TYPE)

#define Skein_Start_New_Type(ctxPtr,BLK_TYPE)   \
    { Skein_Set_T0_T1(ctxPtr,0,SKEIN_T1_FLAG_FIRST | SKEIN_T1_BLK_TYPE_##BLK_TYPE); (ctxPtr)->h.bCnt=0; }

#define Skein_Clear_First_Flag(hdr)      { (hdr).T[1] &= ~SKEIN_T1_FLAG_FIRST;       }
#define Skein_Set_Bit_Pad_Flag(hdr)      { (hdr).T[1] |=  SKEIN_T1_FLAG_BIT_PAD;     }

#define Skein_Set_Tree_Level(hdr,height) { (hdr).T[1] |= SKEIN_T1_TREE_LEVEL(height);}

enum    
    {   
        /* Skein_256 round rotation constants */
    R_256_0_0=14, R_256_0_1=16,
    R_256_1_0=52, R_256_1_1=57,
    R_256_2_0=23, R_256_2_1=40,
    R_256_3_0= 5, R_256_3_1=37,
    R_256_4_0=25, R_256_4_1=33,
    R_256_5_0=46, R_256_5_1=12,
    R_256_6_0=58, R_256_6_1=22,
    R_256_7_0=32, R_256_7_1=32,

        /* Skein_512 round rotation constants */
    R_512_0_0=46, R_512_0_1=36, R_512_0_2=19, R_512_0_3=37,
    R_512_1_0=33, R_512_1_1=27, R_512_1_2=14, R_512_1_3=42,
    R_512_2_0=17, R_512_2_1=49, R_512_2_2=36, R_512_2_3=39,
    R_512_3_0=44, R_512_3_1= 9, R_512_3_2=54, R_512_3_3=56,
    R_512_4_0=39, R_512_4_1=30, R_512_4_2=34, R_512_4_3=24,
    R_512_5_0=13, R_512_5_1=50, R_512_5_2=10, R_512_5_3=17,
    R_512_6_0=25, R_512_6_1=29, R_512_6_2=39, R_512_6_3=43,
    R_512_7_0= 8, R_512_7_1=35, R_512_7_2=56, R_512_7_3=22,

        /* Skein1024 round rotation constants */
    R1024_0_0=24, R1024_0_1=13, R1024_0_2= 8, R1024_0_3=47, R1024_0_4= 8, R1024_0_5=17, R1024_0_6=22, R1024_0_7=37,
    R1024_1_0=38, R1024_1_1=19, R1024_1_2=10, R1024_1_3=55, R1024_1_4=49, R1024_1_5=18, R1024_1_6=23, R1024_1_7=52,
    R1024_2_0=33, R1024_2_1= 4, R1024_2_2=51, R1024_2_3=13, R1024_2_4=34, R1024_2_5=41, R1024_2_6=59, R1024_2_7=17,
    R1024_3_0= 5, R1024_3_1=20, R1024_3_2=48, R1024_3_3=41, R1024_3_4=47, R1024_3_5=28, R1024_3_6=16, R1024_3_7=25,
    R1024_4_0=41, R1024_4_1= 9, R1024_4_2=37, R1024_4_3=31, R1024_4_4=12, R1024_4_5=47, R1024_4_6=44, R1024_4_7=30,
    R1024_5_0=16, R1024_5_1=34, R1024_5_2=56, R1024_5_3=51, R1024_5_4= 4, R1024_5_5=53, R1024_5_6=42, R1024_5_7=41,
    R1024_6_0=31, R1024_6_1=44, R1024_6_2=47, R1024_6_3=46, R1024_6_4=19, R1024_6_5=42, R1024_6_6=44, R1024_6_7=25,
    R1024_7_0= 9, R1024_7_1=48, R1024_7_2=35, R1024_7_3=52, R1024_7_4=23, R1024_7_5=31, R1024_7_6=37, R1024_7_7=20
    };

#define SKEIN_512_ROUNDS_TOTAL (72)

#define InjectKey(r)                                                \
    for (i=0;i < WCNT;i++)                                          \
         X[i] += ks[((r)+i) % (WCNT+1)];                            \
    X[WCNT-3] += ts[((r)+0) % 3];                                   \
    X[WCNT-2] += ts[((r)+1) % 3];                                   \
    X[WCNT-1] += (r);                    /* avoid slide attacks */  \
    Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_KEY_INJECT,X);

QWORD RotL_64(QWORD x,DWORD N)
{
	return (x << (N & 63)) | (x >> ((64-N) & 63));
}

#define BLK_BITS    (WCNT*64)

QWORD Skein_Swap64(QWORD w64)
{    /* instantiate the function body here */
    static const QWORD ONE = 1;              /* use this to check endianness */

    /* figure out endianness "on-the-fly" */
    if (1 == ((BYTE *) & ONE)[0])
        return w64;                           /* little-endian is fast */
    else
        return  (( w64       & 0xFF) << 56) | /*    big-endian is slow */
                (((w64 >> 8) & 0xFF) << 48) |
                (((w64 >>16) & 0xFF) << 40) |
                (((w64 >>24) & 0xFF) << 32) |
                (((w64 >>32) & 0xFF) << 24) |
                (((w64 >>40) & 0xFF) << 16) |
                (((w64 >>48) & 0xFF) <<  8) |
                (((w64 >>56) & 0xFF)      ) ;
}

void Skein_Put64_LSB_First(BYTE *dst,const QWORD *src,DWORD bCnt)
{ /* this version is fully portable (big-endian or little-endian), but slow */
    DWORD n;

    for (n=0;n<bCnt;n++)
        dst[n] = (BYTE) (src[n>>3] >> (8*(n&7)));
}

void Skein_Get64_LSB_First(QWORD *dst,const BYTE *src,DWORD wCnt)
{ /* this version is fully portable (big-endian or little-endian), but slow */
    size_t n;

    for (n=0;n<8*wCnt;n+=8)
        dst[n/8] = (((QWORD) src[n  ])      ) +
                   (((QWORD) src[n+1]) <<  8) +
                   (((QWORD) src[n+2]) << 16) +
                   (((QWORD) src[n+3]) << 24) +
                   (((QWORD) src[n+4]) << 32) +
                   (((QWORD) src[n+5]) << 40) +
                   (((QWORD) src[n+6]) << 48) +
                   (((QWORD) src[n+7]) << 56) ;
}

void Skein_512_Process_Block(SKEIN512_DATA *ctx,const BYTE *blkPtr,size_t blkCnt,size_t byteCntAdd)
{ /* do it in C */
    enum
        {
        WCNT = SKEIN_512_STATE_WORDS
        };

    size_t  i,r;
    QWORD  ts[3];                            /* key schedule: tweak */
    QWORD  ks[WCNT+1];                       /* key schedule: chaining vars */
    QWORD  X [WCNT];                         /* local copy of vars */
    QWORD  w [WCNT];                         /* local copy of input block */

    do  {
        /* this implementation only supports 2**64 input bytes (no carry out here) */
        ctx->h.T[0] += byteCntAdd;            /* update processed length */

        /* precompute the key schedule for this block */
        ks[WCNT] = SKEIN_KS_PARITY;
        for (i=0;i < WCNT; i++)
            {
            ks[i]     = ctx->X[i];
            ks[WCNT] ^= ctx->X[i];            /* compute overall parity */
            }
        ts[0] = ctx->h.T[0];
        ts[1] = ctx->h.T[1];
        ts[2] = ts[0] ^ ts[1];

        Skein_Get64_LSB_First(w,blkPtr,WCNT); /* get input block in little-endian format */
        Skein_Show_Block(BLK_BITS,&ctx->h,ctx->X,blkPtr,w,ks,ts);
        for (i=0;i < WCNT; i++)               /* do the first full key injection */
            {
            X[i]  = w[i] + ks[i];
            }
        X[WCNT-3] += ts[0];
        X[WCNT-2] += ts[1];

        Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_KEY_INITIAL,X);
        for (r=1;r <= SKEIN_512_ROUNDS_TOTAL/8; r++)
            { /* unroll 8 rounds */
            X[0] += X[1]; X[1] = RotL_64(X[1],R_512_0_0); X[1] ^= X[0];
            X[2] += X[3]; X[3] = RotL_64(X[3],R_512_0_1); X[3] ^= X[2];
            X[4] += X[5]; X[5] = RotL_64(X[5],R_512_0_2); X[5] ^= X[4];
            X[6] += X[7]; X[7] = RotL_64(X[7],R_512_0_3); X[7] ^= X[6];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-7,X);

            X[2] += X[1]; X[1] = RotL_64(X[1],R_512_1_0); X[1] ^= X[2];
            X[4] += X[7]; X[7] = RotL_64(X[7],R_512_1_1); X[7] ^= X[4];
            X[6] += X[5]; X[5] = RotL_64(X[5],R_512_1_2); X[5] ^= X[6];
            X[0] += X[3]; X[3] = RotL_64(X[3],R_512_1_3); X[3] ^= X[0];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-6,X);

            X[4] += X[1]; X[1] = RotL_64(X[1],R_512_2_0); X[1] ^= X[4];
            X[6] += X[3]; X[3] = RotL_64(X[3],R_512_2_1); X[3] ^= X[6];
            X[0] += X[5]; X[5] = RotL_64(X[5],R_512_2_2); X[5] ^= X[0];
            X[2] += X[7]; X[7] = RotL_64(X[7],R_512_2_3); X[7] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-5,X);

            X[6] += X[1]; X[1] = RotL_64(X[1],R_512_3_0); X[1] ^= X[6];
            X[0] += X[7]; X[7] = RotL_64(X[7],R_512_3_1); X[7] ^= X[0];
            X[2] += X[5]; X[5] = RotL_64(X[5],R_512_3_2); X[5] ^= X[2];
            X[4] += X[3]; X[3] = RotL_64(X[3],R_512_3_3); X[3] ^= X[4];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-4,X);
            InjectKey(2*r-1);

            X[0] += X[1]; X[1] = RotL_64(X[1],R_512_4_0); X[1] ^= X[0];
            X[2] += X[3]; X[3] = RotL_64(X[3],R_512_4_1); X[3] ^= X[2];
            X[4] += X[5]; X[5] = RotL_64(X[5],R_512_4_2); X[5] ^= X[4];
            X[6] += X[7]; X[7] = RotL_64(X[7],R_512_4_3); X[7] ^= X[6];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-3,X);

            X[2] += X[1]; X[1] = RotL_64(X[1],R_512_5_0); X[1] ^= X[2];
            X[4] += X[7]; X[7] = RotL_64(X[7],R_512_5_1); X[7] ^= X[4];
            X[6] += X[5]; X[5] = RotL_64(X[5],R_512_5_2); X[5] ^= X[6];
            X[0] += X[3]; X[3] = RotL_64(X[3],R_512_5_3); X[3] ^= X[0];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-2,X);

            X[4] += X[1]; X[1] = RotL_64(X[1],R_512_6_0); X[1] ^= X[4];
            X[6] += X[3]; X[3] = RotL_64(X[3],R_512_6_1); X[3] ^= X[6];
            X[0] += X[5]; X[5] = RotL_64(X[5],R_512_6_2); X[5] ^= X[0];
            X[2] += X[7]; X[7] = RotL_64(X[7],R_512_6_3); X[7] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-1,X);

            X[6] += X[1]; X[1] = RotL_64(X[1],R_512_7_0); X[1] ^= X[6];
            X[0] += X[7]; X[7] = RotL_64(X[7],R_512_7_1); X[7] ^= X[0];
            X[2] += X[5]; X[5] = RotL_64(X[5],R_512_7_2); X[5] ^= X[2];
            X[4] += X[3]; X[3] = RotL_64(X[3],R_512_7_3); X[3] ^= X[4];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r  ,X);
            InjectKey(2*r);
            }
        /* do the final "feedforward" xor, update context chaining vars */
        for (i=0;i < WCNT;i++)
            ctx->X[i] = X[i] ^ w[i];
        Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_FEED_FWD,ctx->X);

		Skein_Clear_First_Flag(ctx->h);		/* clear the start bit */
        blkPtr += SKEIN_512_BLOCK_BYTES;
        }
    while (--blkCnt);
}

void Skein512_init(SKEIN512_DATA *skein)
{
	union {
		BYTE	b[SKEIN_512_STATE_BYTES];
		QWORD	w[SKEIN_512_STATE_WORDS];
		} cfg;                                  /* config block */

	/* build/process config block for hashing */
	skein->h.hashBitLen = 512;	                /* output hash byte count */
	Skein_Start_New_Type(skein,CFG_FINAL);        /* set tweaks: T0=0; T1=CFG | FINAL */

	memset(&cfg.w,0,sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
	cfg.w[0]=Skein_Swap64(SKEIN_SCHEMA_VER);  /* set the schema, version */
	cfg.w[1]=Skein_Swap64(512);        /* hash result length in bits */
	cfg.w[2]=Skein_Swap64(SKEIN_CFG_TREE_INFO_SEQUENTIAL);

	/* compute the initial chaining values from config block */
	memset(skein->X,0,sizeof(skein->X));            /* zero the chaining variables */
	Skein_512_Process_Block(skein,cfg.b,1,SKEIN_CFG_STR_LEN);

	/* The chaining vars ctx->X are now initialized for the given hashBitLen. */
	/* Set up to process the data message portion of the hash (default) */
	Skein_Start_New_Type(skein,MSG);              /* T0=0, T1= MSG type, h.bCnt=0 */
}

void Skein512_data(SKEIN512_DATA *skein,const BYTE *buffer,DWORD len)
{
    DWORD n;

    /* process full blocks, if any */
    if (len + skein->h.bCnt > SKEIN_512_BLOCK_BYTES)
        {
        if (skein->h.bCnt)                              /* finish up any buffered message data */
            {
            n = SKEIN_512_BLOCK_BYTES - skein->h.bCnt;  /* # bytes free in buffer b[] */
            if (n)
                {
                memcpy(&skein->b[skein->h.bCnt],buffer,n);
                len -= n;
                buffer += n;
                skein->h.bCnt += n;
                }
            Skein_512_Process_Block(skein,skein->b,1,SKEIN_512_BLOCK_BYTES);
            skein->h.bCnt = 0;
            }
        /* now process any remaining full blocks, directly from input message data */
        if (len > SKEIN_512_BLOCK_BYTES)
            {
            n = (len-1) / SKEIN_512_BLOCK_BYTES;   /* number of full blocks to process */
            Skein_512_Process_Block(skein,buffer,n,SKEIN_512_BLOCK_BYTES);
            len -= n * SKEIN_512_BLOCK_BYTES;
            buffer += n * SKEIN_512_BLOCK_BYTES;
            }
        }

    /* copy any remaining source message data bytes into b[] */
    if (len)
        {
        memcpy(&skein->b[skein->h.bCnt],buffer,len);
        skein->h.bCnt += len;
        }
}

void Skein512_finalize(SKEIN512_DATA *skein,BYTE *hash)
{
    DWORD i,n,byteCnt;
    QWORD X[SKEIN_512_STATE_WORDS];

    skein->h.T[1] |= SKEIN_T1_FLAG_FINAL;                 /* tag as the final block */
    if (skein->h.bCnt < SKEIN_512_BLOCK_BYTES)   /* zero pad b[] if necessary */
        memset(&skein->b[skein->h.bCnt],0,SKEIN_512_BLOCK_BYTES - skein->h.bCnt);

    Skein_512_Process_Block(skein,skein->b,1,skein->h.bCnt);  /* process the final block */
    
    /* now output the result */
    byteCnt = (skein->h.hashBitLen + 7) >> 3;             /* total number of output bytes */

    /* run Threefish in "counter mode" to generate more output */
    memset(skein->b,0,sizeof(skein->b));  /* zero out b[], so it can hold the counter */
    memcpy(X,skein->X,sizeof(X));       /* keep a local copy of counter mode "key" */
    for (i=0;i*SKEIN_512_BLOCK_BYTES < byteCnt;i++)
        {
        ((QWORD *)skein->b)[0]= Skein_Swap64((QWORD) i); /* build the counter block */
        Skein_Start_New_Type(skein,OUT_FINAL);
        Skein_512_Process_Block(skein,skein->b,1,sizeof(QWORD)); /* run "counter mode" */
        n = byteCnt - i*SKEIN_512_BLOCK_BYTES;   /* number of output bytes left to go */
        if (n >= SKEIN_512_BLOCK_BYTES)
            n  = SKEIN_512_BLOCK_BYTES;
        Skein_Put64_LSB_First(hash+i*SKEIN_512_BLOCK_BYTES,skein->X,n);   /* "output" the ctr mode bytes */
        Skein_Show_Final(512,&ctx->h,n,hashVal+i*SKEIN_512_BLOCK_BYTES);
        memcpy(skein->X,X,sizeof(X));   /* restore the counter mode key for next time */
        }
}
