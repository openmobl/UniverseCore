/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ifndef _MATH_PRIVATE_H_
#define _MATH_PRIVATE_H_

#include <sys/types.h>
#include <stdint.h>

/* The original fdlibm code used statements like:
	n0 = ((*(int*)&one)>>29)^1;		* index of high word *
	ix0 = *(n0+(int*)&x);			* high word of x *
	ix1 = *((1-n0)+(int*)&x);		* low word of x *
   to dig two 32 bit words out of the 64 bit IEEE floating point
   value.  That is non-ANSI, and, moreover, the gcc instruction
   scheduler gets it wrong.  We instead use the following macros.
   Unlike the original code, we determine the endianness at compile
   time, not at run time; I don't see much benefit to selecting
   endianness at run time.  */

/* A union which permits us to convert between a double and two 32 bit
   ints.  */

/*
 * Math on arm is special:
 * For FPA, float words are always big-endian.
 * For VFP, floats words follow the memory system mode.
 */

#if (!defined(__VFP_FP__) && defined(__arm__))

typedef union
{
  double value;
  struct
  {
    u_int32_t msw;
    u_int32_t lsw;
  } parts;
} ieee_double_shape_type;

typedef struct                   /*      Hex representation of a double.      */
{
  u_int32_t high;
  u_int32_t low;
} dHexParts;

#else

typedef union
{
  double value;
  struct
  {
    u_int32_t lsw;
    u_int32_t msw;
  } parts;
} ieee_double_shape_type;

typedef struct                   /*      Hex representation of a double.      */
{
  u_int32_t low;
  u_int32_t high;
} dHexParts;

#endif

typedef union
{
  unsigned char byties[8];
  double dbl;
} DblInHex;

/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)					\
do {								\
  ieee_double_shape_type gh_u;					\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (0)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)					\
do {								\
  ieee_double_shape_type gl_u;					\
  gl_u.value = (d);						\
  (i) = gl_u.parts.lsw;						\
} while (0)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)					\
do {								\
  ieee_double_shape_type iw_u;					\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)					\
do {								\
  ieee_double_shape_type sh_u;					\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)					\
do {								\
  ieee_double_shape_type sl_u;					\
  sl_u.value = (d);						\
  sl_u.parts.lsw = (v);						\
  (d) = sl_u.value;						\
} while (0)

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union
{
  float value;
  u_int32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
do {								\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
do {								\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

///* ieee style elementary functions */
#define __ieee754_sqrt(x) sqrt(x)
//#define __ieee754_acos(x) acos(x)
//#define __ieee754_acosh(x) acosh(x)
#define __ieee754_log(x) log(x)
//#define __ieee754_atanh(x) atanh(x)
//#define __ieee754_asin(x) asin(x)
//#define __ieee754_atan2(x) atan2(x)
#define __ieee754_exp(x) exp(x)
//#define __ieee754_cosh(x) cosh(x)
#define __ieee754_fmod(x,y) fmod(x,y)
//#define __ieee754_pow(x,y) pow(x,y)
//extern double __ieee754_lgamma_r (double,int *) attribute_hidden;
//extern double __ieee754_gamma_r (double,int *) attribute_hidden;
//extern double __ieee754_lgamma (double) attribute_hidden;
//extern double __ieee754_gamma (double) attribute_hidden;
//#define __ieee754_log10(x) log10(x)
//#define __ieee754_sinh(x) sinh(x)
//extern double __ieee754_hypot (double,double) attribute_hidden;
//extern double __ieee754_j0 (double) attribute_hidden;
//extern double __ieee754_j1 (double) attribute_hidden;
//extern double __ieee754_y0 (double) attribute_hidden;
//extern double __ieee754_y1 (double) attribute_hidden;
//extern double __ieee754_jn (int,double) attribute_hidden;
//extern double __ieee754_yn (int,double) attribute_hidden;
//#define __ieee754_remainder(x,y) remainder(x,y)
extern int32_t __ieee754_rem_pio2 (double x,double *py);
#define __ieee754_scalb(x,y) scalb(x,y)


#endif /* _MATH_PRIVATE_H_ */
