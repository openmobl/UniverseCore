#ifndef _STDARG_H
#define	_STDARG_H

#ifdef	__cplusplus
extern "C" {
#endif

/* Defined in MSL\MSL_C\MSL_ARM\Include\stdarg.ARM.h */
#if defined(__MWERKS__)

typedef char * __builtin_va_list;

#define __fourbytealign(n)          ((((unsigned long)(n)) + 3U) & ~3U)
#define __va_start(param)           ((__builtin_va_list)((char *)&param + __fourbytealign(sizeof(param))))

#define __builtin_va_start(v,l)     ((v) = __va_start(l))
#define __builtin_va_end(v)         ((void)0)
#define __builtin_va_arg(v,l)       (*(l *) ((v += __fourbytealign(sizeof(l))) - __fourbytealign(sizeof(l))))
#define __builtin_va_copy(d,s)      d = s

#endif


typedef __builtin_va_list va_list;

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_copy(d,s)	__builtin_va_copy(d,s)
#define __va_copy(d,s)	__builtin_va_copy(d,s)


#ifndef _VA_LIST
#define _VA_LIST
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* _STDARG_H */

