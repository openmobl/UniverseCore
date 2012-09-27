#ifndef _MATH_H___
#define _MATH_H___

#ifdef __cplusplus
extern "C" {
#endif

#define	M_E		2.7182818284590452354
#define	M_LOG2E		1.4426950408889634074
#define	M_LOG10E	0.43429448190325182765
#define	M_LN2		0.69314718055994530942
#define	M_LN10		2.30258509299404568402
#ifndef M_PI
#define	M_PI		3.14159265358979323846
#endif
#define	M_PI_2		1.57079632679489661923
#define	M_PI_4		0.78539816339744830962
#define	M_1_PI		0.31830988618379067154
#define	M_2_PI		0.63661977236758134308
#define	M_2_SQRTPI	1.12837916709551257390
#define	M_SQRT2		1.41421356237309504880
#define	M_SQRT1_2	0.70710678118654752440


/* All floating-point numbers can be put in one of these categories.  */
enum
  {
    FP_NAN,
# define FP_NAN FP_NAN
    FP_INFINITE,
# define FP_INFINITE FP_INFINITE
    FP_ZERO,
# define FP_ZERO FP_ZERO
    FP_SUBNORMAL,
# define FP_SUBNORMAL FP_SUBNORMAL
    FP_NORMAL
# define FP_NORMAL FP_NORMAL
  };


extern double floor (double x);
extern double copysign (double x, double y);
extern double scalbn (double x, int n);
extern double scalb (double x, double y);
extern double cos (double x);
extern double cosh (double x);
extern double sin (double x);
extern double sinh (double x);
extern double acos (double x);
extern double acosh (double x);
extern double asin (double x);
extern double asinh (double x);
extern double fabs (double x);
extern double ceil (double x);
extern double exp (double x);
extern double log (double x);
extern double log10 (double x);
extern double logb (double x);
extern double fmod (double x, double y);
extern double modf (double x, double *py);
extern double pow (double x, double y);
extern double sqrt (double x);
extern double remainder (double x, double y);
extern double nextafter (double x, double y);
extern double significand (double x);
extern double ldexp (double x, int n);
extern double frexp (double x, int * pn);
extern double cbrt (double x);
extern double tan (double x);
extern double atan (double x);
extern double atan2 (double x, double y);
extern double tanh (double x);
extern double atanh (double x);

extern int finite(double x);
extern int isnan(double x);
extern double rint(double x);
extern int ilogb(double x);
extern double expm1(double x);
extern double log1p(double x);


#ifdef __cplusplus
}
#endif

#endif /* _MATH_H___ */
