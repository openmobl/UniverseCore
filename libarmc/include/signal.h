#ifndef _SIGNAL_H
#define	_SIGNAL_H

//#include <sys/signal.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef int	sig_atomic_t;


// Not yet implemented
extern void (*signal(int sig, void (*handler)(int)))(int);
extern int raise(int sig);


#ifdef	__cplusplus
}
#endif

#endif	/* _SIGNAL_H */

