#ifndef _ASSERT_H
#define	_ASSERT_H


#ifdef	__cplusplus
extern "C" {
#endif


// Not yet implemented:
extern void __assert(const char *, const char *, int);


#ifdef	__cplusplus
}
#endif

#endif	/* _ASSERT_H */

/*
 * Note that the ANSI C Standard requires all headers to be idempotent except
 * <assert.h> which is explicitly required not to be idempotent (section 4.1.2).
 * Therefore, it is by intent that the header guards (#ifndef _ASSERT_H) do
 * not span this entire file.
 */

#undef	assert

#if defined(DEBUG) || defined(_DEBUG)
#define	assert(EX) (void)((EX) || (__assert(#EX, __FILE__, __LINE__), 0))
#else
#define	assert(EX) ((void)0)
#endif	/* NDEBUG */
