#ifndef __STDLIB_H__
#define __STDLIB_H__


#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct 
{
  int quot; /* quotient */
  int rem; /* remainder */
} div_t;

typedef struct {
	long	quot;
	long	rem;
} ldiv_t;

#ifndef _SIZE_T
#define	_SIZE_T
typedef unsigned long size_t;
#endif

#ifndef _UID_T
#define	_UID_T
typedef long uid_t;
#endif

#ifndef NULL
#define	NULL	0
#endif

#define	EXIT_FAILURE	1
#define	EXIT_SUCCESS  0
#define	RAND_MAX	INT_MAX

#ifndef _WCHAR_T
#define	_WCHAR_T
#ifndef __cplusplus
typedef long wchar_t;
#endif
#endif


extern void *malloc(size_t size);
extern void *calloc(size_t n, size_t size);
extern void *realloc(void *ptr, size_t size);
extern void free(void *ptr);
extern char *getenv(const char *name);
extern int setenv(const char *name, const char *value, int rewrite);
extern void unsetenv(const char *name);
extern int putenv(const char *str);
extern void qsort(void *a, size_t n, size_t es, int(*cmp)());
extern void *bsearch(const void *key, const void *base, size_t high, size_t size, int (*compar)(const void *, const void *));
extern int rand(void);
extern void srand(unsigned int seed);
extern div_t div(int num, int denom);
extern ldiv_t ldiv(long int numer, long int denom);
extern double strtod(const char *str, char **endptr);
extern double atof(const char *str);
extern long int strtol(const char *s, char **endptr, int base);
extern long int atol(const char *s);
extern int atoi(const char *s);
extern unsigned long int strtoul(const char *s, char **endptr, int base);
extern int abs(int j);
extern long int labs(long j);
extern int system(const char *str);
extern char *getcwd(char *buf, size_t len);


//----------------------------------------
// should be from pealstub, we need to implement
//----------------------------------------
extern void abort(void);
extern void exit(int exitcode);
extern int atexit(void (*fptr)(void));


//Not yet implemented
//extern int mbtowc(wchar_t *, const char *, size_t);
//extern int mblen(const char *, size_t);
//extern int wctomb(char *, wchar_t);
//
//extern size_t mbstowcs(wchar_t *, const char *, size_t);
//extern size_t wcstombs(char *, const wchar_t *, size_t);
//
//extern double drand48(void);
//extern double erand48(unsigned short *);
//extern long jrand48(unsigned short *);
//extern void lcong48(unsigned short *);
//extern long lrand48(void);
//extern long mrand48(void);
//extern long nrand48(unsigned short *);
//extern unsigned short *seed48(unsigned short *);
//extern void srand48(long);
//extern void setkey(const char *);
//
//extern void swab(const char *, char *, int);


#ifdef __cplusplus
}
#endif

#endif /* __STDLIB_H__ */


