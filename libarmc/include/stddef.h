#ifndef _STDDEF_H
#define	_STDDEF_H


#ifdef	__cplusplus
extern "C" {
#endif


typedef long ptrdiff_t;

#ifndef _SIZE_T
#define	_SIZE_T
typedef unsigned long	size_t;
typedef long ssize_t;
#endif

#ifndef NULL
#define	NULL	0
#endif


#define offsetof(type, member)	((unsigned long) &(((type *) 0)->member))

#ifdef	__cplusplus
}
#endif

#endif	/* _STDDEF_H */

