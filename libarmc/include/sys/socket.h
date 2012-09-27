/******************************************************************************
 *
 * Parts Copyright (c) 1996-2004 PalmSource, Inc. All rights reserved.
 *
 * File: sys_socket.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *      This module contains the interface definitions that are 
 *    typically found in the unix header <sys/socket.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H

// So we don't include Tapwave files
#define __SYS_SOCKET_H__

#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdint.h>

/*===================================================================
 * Definitions related to sockets: types, address families, options.
 ===================================================================*/

#ifndef _SA_FAMILY_T
#define _SA_FAMILY_T
// should be unsigned int
typedef int16_t    sa_family_t;
#endif

#ifndef _SOCKLEN_T
#define _SOCKLEN_T
// Should be unsigned int
typedef	u_int16_t   socklen_t;
#endif

/*
 * Types
 */
#define SOCK_STREAM     netSocketTypeStream            /* stream socket */
#define SOCK_DGRAM      netSocketTypeDatagram        /* datagram socket */
#define SOCK_RAW        netSocketTypeRaw                /* raw-protocol interface */
#define SOCK_RDM        netSocketTypeReliableMsg    /* reliable message */

/*
 * Option flags per-socket.
 */
#define SO_DEBUG        netSocketOptSockDebug            /* turn on debugging info recording */
#define SO_ACCEPTCONN   netSocketOptSockAcceptConn        /* socket has had listen() */
#define SO_REUSEADDR    netSocketOptSockReuseAddr        /* allow local address reuse */
#define SO_KEEPALIVE    netSocketOptSockKeepAlive        /* keep connections alive */
#define SO_DONTROUTE    netSocketOptSockDontRoute        /* just use interface addresses */
#define SO_BROADCAST    netSocketOptSockBroadcast        /* permit sending of broadcast msgs */
#define SO_USELOOPBACK  netSocketOptSockUseLoopback    /* bypass hardware when possible */
#define SO_LINGER       netSocketOptSockLinger            /* linger on close if data present */
#define SO_OOBINLINE    netSocketOptSockOOBInLine        /* leave received OOB data in line */
/*
 * N.B.: The following definition is present only for compatibility
 * with release 3.0.  It will disappear in later releases.
 */
#define SO_DONTLINGER    (~SO_LINGER)    /* ~SO_LINGER */

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF       netSocketOptSockSndBufSize            /* send buffer size */
#define SO_RCVBUF       netSocketOptSockRcvBufSize            /* receive buffer size */
#define SO_SNDLOWAT     netSocketOptSockSndLowWater        /* send low-water mark */
#define SO_RCVLOWAT     netSocketOptSockRcvLowWater        /* receive low-water mark */
#define SO_SNDTIMEO     netSocketOptSockSndTimeout            /* send timeout */
#define SO_RCVTIMEO     netSocketOptSockRcvTimeout            /* receive timeout */
#define SO_ERROR        netSocketOptSockErrorStatus        /* get error status and clear */
#define SO_TYPE         netSocketOptSockSocketType            /* get socket type */

/*
 * Structure used for manipulating linger option.
 */
struct linger {
    int    l_onoff;                                                    /* option on/off */
    int    l_linger;                                                /* linger time */
};

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define SOL_SOCKET    netSocketOptLevelSocket                /* options for socket level */

/*
 * Address families.
 */
#define AF_INET     netSocketAddrINET                /* internetwork: UDP, TCP, etc. */
#define AF_RAW      netSocketAddrRaw                /* raw access to data-link interface */

/*
 * Structure used by kernel to store most
 * addresses.
 */
struct sockaddr {
    sa_family_t     sa_family;                                        /* address family */
    char            sa_data[14];                                    /* up to 14 bytes of direct address */
};

/*
 * Structure used by kernel to pass protocol
 * information in raw sockets.
 */
struct sockproto {
    u_int16_t    sp_family;                                        /* address family */
    u_int16_t    sp_protocol;                                    /* protocol */
};

/*
 * Protocol families, same as address families for now.
 */  
#define PF_INET     AF_INET
#define PF_RAW      AF_RAW

/*
 * Maximum queue length specifiable by listen.
 */
#define SOMAXCONN   5

/*
 * Message header for recvmsg and sendmsg calls.
 */
struct msghdr {
    void *      msg_name;                                    /* optional address */
    socklen_t   msg_namelen;                                /* size of address */
    struct      iovec *msg_iov;                            /* scatter/gather array */
    u_int16_t   msg_iovlen;                                    /* # elements in msg_iov */
    void *      msg_accrights;                                /* access rights sent/received */
    socklen_t   msg_accrightslen;
/* The PalmSource headers do not define it this way... */
#if 0
    int         msg_flags;
#endif
    };

#define MSG_OOB         netIOFlagOutOfBand        /* process out-of-band data */
#define MSG_PEEK        netIOFlagPeek                /* peek at incoming message */
#define MSG_DONTROUTE   netIOFlagDontRoute        /* send without using routing tables */
#define MSG_MAXIOVLEN   netIOVecMaxLen


#ifdef __cplusplus
extern "C" {
#endif

extern int accept(int socket, struct sockaddr *address,
                socklen_t *address_len);
extern int bind(int socket, const struct sockaddr *address,
                socklen_t address_len);
extern int connect(int socket, const struct sockaddr *address,
                socklen_t address_len);
extern int getsockopt(int socket, int level, int option_name,
                void *option_value, socklen_t *option_len);
extern int listen(int socket, int backlog);
extern ssize_t recv(int socket, void *buffer, size_t length, int flags);
extern ssize_t recvfrom(int socket, void *buffer, size_t length,
                int flags, struct sockaddr *address,
                socklen_t *address_len);
extern ssize_t recvmsg(int socket, struct msghdr *message, int flags);
extern ssize_t send(int socket, const void *buffer, size_t length, int flags);
extern ssize_t sendmsg(int socket, const struct msghdr *message, int flags);
extern ssize_t sendto(int socket, const void *message, size_t length,
                int flags, const struct sockaddr *dest_addr,
                socklen_t dest_len);
extern int setsockopt(int socket, int level, int option_name,
                const void *option_value, socklen_t option_len);
extern int shutdown(int socket, int how);
extern int socket(int domain, int type, int protocol);
extern int getpeername(int socket, struct sockaddr *address,
                socklen_t *address_len);
extern int getsockname(int socket, struct sockaddr *address,
                socklen_t *address_len);

#if 0
Int16        getdomainname(Char * nameP, Int16 namelen);

Int16        setdomainname(Char * nameP, Int16 namelen);

Int16        gethostname(Char * nameP, Int16 namelen);

Int16        sethostname(Char * nameP, Int16 namelen);

Int16        select(Int16 width, fd_set* readfds, fd_set* writefds,
                    fd_set* exceptfds, struct timeval* timeout);

Int16        gettimeofday(struct timeval* tp, struct timezone* tzp);

Int16        settimeofday(struct timeval* tp, struct timezone* tzp);
#endif


#ifdef __cplusplus 
}
#endif




#endif /* _SYS_SOCKET_H */
