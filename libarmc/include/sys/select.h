/*
 *    POSIX Standard: 2.6 Primitive System Data Types    <sys/types.h>
 */

#ifndef _SYS_SELECT_H
#define _SYS_SELECT_H    1

#include <sys/types.h>

#include <ArmNetMgr.h>

typedef long int fd_set;

struct timeval {
	time_t	tv_sec;			/* seconds */
	time_t	tv_usec;			/* and microseconds */
};

#define	FD_SETSIZE		netFDSetSize

#define	FD_SET(n,p)		netFDSet(n,p)
#define	FD_CLR(n,p)		netFDClr(n,p)
#define	FD_ISSET(n,p)	netFDIsSet(n,p)
#define	FD_ZERO(p)		netFDZero(p)

int select(int nfds, fd_set *readfds,
       fd_set *writefds, fd_set *errorfds,
       struct timeval *timeout);

#endif /* sys/types.h */
