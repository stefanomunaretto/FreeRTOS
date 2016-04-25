/////////////////////////////////////////////////////////////////////////
//	xtypes.h
/////////////////////////////////////////////////////////////////////////

#ifndef _XTYPES_H_
#define _XTYPES_H_

#include "stdint.h"
#include "yvals.h"


/* Processor & Compiler independent, size specific definitions */

// ## unsigned ##
// ######################################################################

#ifndef BYTE
typedef unsigned char           BYTE;		        // 8-bit unsigned
#endif
typedef unsigned char           UINT8;
#define uint8                   UINT8
#define uchar                   UINT8


#ifndef WORD
typedef unsigned short          WORD;	                // 16-bit unsigned
#endif
typedef unsigned short          UINT16;
#define uint16                  UINT16
#define ushort                  UINT16


typedef unsigned int            UINT;                   // 32-bit unsigned
typedef unsigned int            uint;

typedef unsigned long           DWORD;
typedef unsigned long           UINT32;
#define uint32                  UINT32
#define ulong                   UINT32


typedef unsigned long long      QWORD;			// 64-bit unsigned
typedef unsigned long long      UINT64;
#define uint64                  UINT64


// ## signed ##
// ######################################################################

typedef signed char             CHAR8;                  // 8-bit signed
#define int8                    CHAR8


typedef signed short            INT16;                  // 16-bit signed
#define int16                   INT16


typedef signed int              INT;                    // 32-bit signed

typedef signed long             LONG;
typedef signed long             INT32;
#define int32                   INT32


typedef signed long long	LONGLONG;		// 64-bit signed
typedef signed long long        INT64;
#define int64                   INT64



/******************************************************************************
*       MAX - MIN
******************************************************************************/

#define MIN_OF(x,y)	((x)<(y)?(x):(y))
#define MAX_OF(x,y)	((x)>(y)?(x):(y))



/******************************************************************************
*       EXTRA COMMON DEFS.
******************************************************************************/

#ifndef __bool_true_false_are_defined
   #define bool         _Bool
   #define true         1
   #define false        0
#else
   #include "stdbool.h"
#endif

#define DISABLE         false
#define ENABLE          true

#define NO_SELECT       false
#define SELECT          true

#define OFF             false
#define ON              true

#define FAILURE         false
#define SUCCESS         true


// pre-complier def.
#ifndef NULL
    #define NULL    ((void*)0)
#endif

// string def.
typedef char* STRING;

// point def.
typedef struct _POINT {
	int x;
	int y;
} POINT;

// STREAM STRUCT def
typedef struct _structStream {
    BYTE*  pdata; 	// buffer address that holds the streams
    ulong  length;	// length of the stream in bytes
} STREAM;




// CONVERTION TYPE
// //////////////////////////////////////////////////////////////////

/************************************************************************
// Macro: pointer macro conv.
************************************************************************/
#define BYTE_PTR(x)	((uchar*) x)
#define WORD_PTR(x)	((uint*) x)



/************************************************************************
* Macro: Lo                                                             *
* Overview: This macro extracts a low byte from a 2 byte word.          *
************************************************************************/
#define LO(X)                   (BYTE)(X&0x00ff)
#define LOBYTE(x)		((BYTE*) (&x))[0]

/************************************************************************
* Macro: Hi                                                             *
* Overview: This macro extracts a high byte from a 2 byte word.         *
************************************************************************/
#define HI(X)                   (BYTE)((X>>8)&0x00ff)
#define HIBYTE(x)		((BYTE*) (&x))[1]



/************************************************************************
* Macro: LoW                                                             *
* Overview: This macro extracts a low word from a 4 byte dbword.        *
************************************************************************/
//#define LoW(X)   (WORD)(0x0000ffff&X)
#define LOWORD(x)      ((WORD*) (&x))[0]

/************************************************************************
* Macro: HiW                                                             *
* Overview: This macro extracts a high word from a 4 byte dbword.       *
************************************************************************/
//#define HiW(X)   (WORD)(0x0000ffff&(X>>16))
#define HIWORD(x)       ((WORD*) (&x))[1]

/************************************************************************
// Macro: return byte from DWORD type
************************************************************************/
#define B0_W(x)   ((BYTE*) (&x))[0]
#define B1_W(x)   ((BYTE*) (&x))[1]
#define B2_W(x)   ((BYTE*) (&x))[2]
#define B3_W(x)   ((BYTE*) (&x))[3]



/************************************************************************
// Macro: return bit from byte, word, dword..
************************************************************************/
typedef union _BYTE_VAL
{
   BYTE val;

   struct
   {
     uchar LO:4;
     uchar HI:4;
   } nibble;

   struct
   {
     uchar b0:1;
     uchar b1:1;
     uchar b2:1;
     uchar b3:1;
     uchar b4:1;
     uchar b5:1;
     uchar b6:1;
     uchar b7:1;
   } bits;

} BYTE_VAL, BYTE_BITS;

typedef union _WORD_VAL
{
   WORD val;
   BYTE vAry[2];

   struct
   {
     BYTE LSB;
     BYTE MSB;
   } byte;

   struct
   {
     uchar b0:1;
     uchar b1:1;
     uchar b2:1;
     uchar b3:1;
     uchar b4:1;
     uchar b5:1;
     uchar b6:1;
     uchar b7:1;
     uchar b8:1;
     uchar b9:1;
     uchar b10:1;
     uchar b11:1;
     uchar b12:1;
     uchar b13:1;
     uchar b14:1;
     uchar b15:1;
   } bits;

} WORD_VAL, WORD_BITS;

typedef union _DWORD_VAL
{
   DWORD val;
   WORD  wAry[2];
   BYTE  bAry[4];

   struct
   {
     WORD LW;
     WORD HW;
   } word;

   struct
   {
     BYTE LB;
     BYTE HB;
     BYTE UB;
     BYTE MB;
   } byte;

   struct
   {
     WORD_VAL low;
     WORD_VAL high;
   }wordUnion;

   struct
   {
     uchar b0:1;
     uchar b1:1;
     uchar b2:1;
     uchar b3:1;
     uchar b4:1;
     uchar b5:1;
     uchar b6:1;
     uchar b7:1;
     uchar b8:1;
     uchar b9:1;
     uchar b10:1;
     uchar b11:1;
     uchar b12:1;
     uchar b13:1;
     uchar b14:1;
     uchar b15:1;
     uchar b16:1;
     uchar b17:1;
     uchar b18:1;
     uchar b19:1;
     uchar b20:1;
     uchar b21:1;
     uchar b22:1;
     uchar b23:1;
     uchar b24:1;
     uchar b25:1;
     uchar b26:1;
     uchar b27:1;
     uchar b28:1;
     uchar b29:1;
     uchar b30:1;
     uchar b31:1;
   } bits;

} DWORD_VAL;

typedef union _QWORD_VAL
{
   QWORD val;
   DWORD dAry[2];
   WORD  wAry[4];
   BYTE  bAry[8];

   struct
   {
     DWORD LD;
     DWORD HD;
   } dword;

   struct
   {
     WORD LW;
     WORD HW;
     WORD UW;
     WORD MW;
   } word;

   struct
   {
     uchar b0:1;
     uchar b1:1;
     uchar b2:1;
     uchar b3:1;
     uchar b4:1;
     uchar b5:1;
     uchar b6:1;
     uchar b7:1;
     uchar b8:1;
     uchar b9:1;
     uchar b10:1;
     uchar b11:1;
     uchar b12:1;
     uchar b13:1;
     uchar b14:1;
     uchar b15:1;
     uchar b16:1;
     uchar b17:1;
     uchar b18:1;
     uchar b19:1;
     uchar b20:1;
     uchar b21:1;
     uchar b22:1;
     uchar b23:1;
     uchar b24:1;
     uchar b25:1;
     uchar b26:1;
     uchar b27:1;
     uchar b28:1;
     uchar b29:1;
     uchar b30:1;
     uchar b31:1;
     uchar b32:1;
     uchar b33:1;
     uchar b34:1;
     uchar b35:1;
     uchar b36:1;
     uchar b37:1;
     uchar b38:1;
     uchar b39:1;
     uchar b40:1;
     uchar b41:1;
     uchar b42:1;
     uchar b43:1;
     uchar b44:1;
     uchar b45:1;
     uchar b46:1;
     uchar b47:1;
     uchar b48:1;
     uchar b49:1;
     uchar b50:1;
     uchar b51:1;
     uchar b52:1;
     uchar b53:1;
     uchar b54:1;
     uchar b55:1;
     uchar b56:1;
     uchar b57:1;
     uchar b58:1;
     uchar b59:1;
     uchar b60:1;
     uchar b61:1;
     uchar b62:1;
     uchar b63:1;
   } bits;

} QWORD_VAL;


// macro hardware register (NB: for 16bit micros)
#define HWREG32(add)  (*((volatile uint32*)((uint16)add)))
#define HWREG16(add)  (*((volatile uint16*)((uint16)add)))
#define HWREG8(add)   (*((volatile uint8*)((uint16)add)))


// bit value
#ifndef BV
   #define BITVAL(n)      (1 << (n))
#endif

#endif
