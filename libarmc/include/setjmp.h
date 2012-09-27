#ifndef _SETJMP_H
#define	_SETJMP_H

#ifdef	__cplusplus
extern "C" {
#endif


#ifndef _ASM
/* Jump buffer contains v1-v6, sl, fp, sp and pc.  Other registers are not saved.  */
/*typedef int jmp_buf[22];*/

/* For Codewarrior. */
typedef unsigned long jmp_buf[16];
#endif


extern int setjmp(jmp_buf env);
extern void longjmp(jmp_buf env, int val);


#ifdef	__cplusplus
}
#endif

#endif	/* _SETJMP_H */
