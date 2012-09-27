/******************************************************************************
 *
 * Copyright (c) 1996-2004 PalmSource, Inc. All rights reserved.
 *
 * File: netinet_in.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *      This module contains the interface definitions that are 
 *    typically found in the unix header <netinet/in.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

/*    @(#)in.h 1.19 90/07/27 SMI; from UCB 7.5 2/22/88    */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 */

/*
 * Constants and structures defined by the internet system,
 * Per RFC 790, September 1981.
 */

#ifndef _NETINET_IN_H
#define _NETINET_IN_H

// So we don't include  Tapwave files
#define __NETINET_IN_H__

#include <sys/types.h>
#include <stdint.h>

#include <sys/socket.h>

#ifndef _IN_ADDR_T
#define _IN_ADDR_T
typedef uint32_t   in_addr_t;
#endif

#ifndef _IN_PORT_T
#define _IN_PORT_T
typedef uint16_t   in_port_t;
#endif

/*
 * Protocols
 */
#define IPPROTO_IP            0        /* dummy for IP */
#define IPPROTO_ICMP        1        /* control message protocol */
#define IPPROTO_IGMP        2        /* group control protocol */
#define IPPROTO_GGP            3        /* gateway^2 (deprecated) */
#define IPPROTO_TCP            6        /* tcp */
#define IPPROTO_EGP            8        /* exterior gateway protocol */
#define IPPROTO_PUP            12        /* pup */
#define IPPROTO_UDP            17        /* user datagram protocol */
#define IPPROTO_IDP            22        /* xns idp */
#define IPPROTO_HELLO        63        /* "hello" routing protocol */
#define IPPROTO_ND            77        /* UNOFFICIAL net disk proto */

#define IPPROTO_RAW            255        /* raw IP packet */
#define IPPROTO_MAX            256

/*
 * Port/socket numbers: network standard functions
 */
#define    IPPORT_ECHO            7
#define    IPPORT_DISCARD        9
#define    IPPORT_SYSTAT        11
#define    IPPORT_DAYTIME        13
#define    IPPORT_NETSTAT        15
#define    IPPORT_FTP            21
#define    IPPORT_TELNET        23
#define    IPPORT_SMTP            25
#define    IPPORT_TIMESERVER    37
#define    IPPORT_NAMESERVER    42
#define    IPPORT_WHOIS        43
#define    IPPORT_MTP            57

/*
 * Port/socket numbers: host specific functions
 */
#define    IPPORT_TFTP            69
#define    IPPORT_RJE            77
#define    IPPORT_FINGER        79
#define    IPPORT_TTYLINK        87
#define    IPPORT_SUPDUP        95

/*
 * UNIX TCP sockets
 */
#define    IPPORT_EXECSERVER        512
#define    IPPORT_LOGINSERVER        513
#define    IPPORT_CMDSERVER        514
#define    IPPORT_EFSSERVER        520

/*
 * UNIX UDP sockets
 */
#define    IPPORT_BIFFUDP            512
#define    IPPORT_WHOSERVER        513
#define    IPPORT_ROUTESERVER        520    /* 520+1 also used */

/*
 * Ports < IPPORT_RESERVED are reserved for
 * privileged processes (e.g. root).
 * Ports > IPPORT_USERRESERVED are reserved
 * for servers, not necessarily privileged.
 */
#define    IPPORT_RESERVED        1024
#define    IPPORT_USERRESERVED    5000

/*
 * Link numbers
 */
#define    IMPLINK_IP                155
#define    IMPLINK_LOWEXPER        156
#define    IMPLINK_HIGHEXPER        158

/*
 * Internet address
 *    This definition contains obsolete fields for compatibility
 *    with SunOS 3.x and 4.2bsd.  The presence of subnets renders
 *    divisions into fixed fields misleading at best.  New code
 *    should use only the s_addr field.
 */
struct in_addr {
    in_addr_t s_addr;
};

/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define    IN_CLASSA(i)            (((long)(i) & 0x80000000) == 0)
#define    IN_CLASSA_NET            0xff000000
#define    IN_CLASSA_NSHIFT        24
#define    IN_CLASSA_HOST            0x00ffffff
#define    IN_CLASSA_MAX            128

#define    IN_CLASSB(i)            (((long)(i) & 0xc0000000) == 0x80000000)
#define    IN_CLASSB_NET            0xffff0000
#define    IN_CLASSB_NSHIFT        16
#define    IN_CLASSB_HOST            0x0000ffff
#define    IN_CLASSB_MAX            65536

#define    IN_CLASSC(i)            (((long)(i) & 0xe0000000) == 0xc0000000)
#define    IN_CLASSC_NET            0xffffff00
#define    IN_CLASSC_NSHIFT        8
#define    IN_CLASSC_HOST            0x000000ff

#define    IN_CLASSD(i)            (((long)(i) & 0xf0000000) == 0xe0000000)
#define    IN_MULTICAST(i)            IN_CLASSD(i)

#define    IN_EXPERIMENTAL(i)        (((long)(i) & 0xe0000000) == 0xe0000000)
#define    IN_BADCLASS(i)            (((long)(i) & 0xf0000000) == 0xf0000000)

#define    INADDR_ANY                (u_long)0x00000000
#define    INADDR_NONE                (u_long)0xFFFFFFFF    /* error from inet_aton */
#define    INADDR_LOOPBACK            (u_long)0x7F000001
#define    INADDR_BROADCAST        (u_long)0xffffffff    /* must be masked */

#define    IN_LOOPBACKNET            127                        /* official! */

/*
 * Define a macro to stuff the loopback address into an Internet address
 */
#define IN_SET_LOOPBACK_ADDR(a)    {(a)->sin_addr.s_addr  = htonl(INADDR_LOOPBACK); \
    (a)->sin_family = AF_INET;}

/*
 * Socket address, internet style.
 */
struct sockaddr_in {
/* PalmSource headers also don't include sin_len... */
#if 0
    uint8_t         sin_len;
#endif
    sa_family_t     sin_family;
    in_port_t       sin_port;
    struct in_addr  sin_addr;
    char            sin_zero[8];
};

/*
 * Options for use with [gs]etsockopt at the IP level.
 */
#define    IP_OPTIONS    netSocketOptIPOptions        /* set/get IP per-packet options */ 

/*
 * Macros for number representation conversion.
 */
#if defined(__MC68K__) || defined(__m68k)

#define ntohl(x)    (x)
#define ntohs(x)    (x)
#define htonl(x)    (x)
#define htons(x)    (x)

#else

#define ntohl(x)    ((((x) >> 24) & 0x00FF) | (((x) >> 8) & 0xFF00) | (((x) & 0xFF00) << 8) | (((x) & 0x00FF) << 24))
#define ntohs(x)    ((((x) >> 8) & 0xFF) | (((x) & 0xFF) << 8))
#define htonl(x)    ((((x) >> 24) & 0x00FF) | (((x) >> 8) & 0xFF00) | (((x) & 0xFF00) << 8) | (((x) & 0x00FF) << 24))
#define htons(x)    ((((x) >> 8) & 0xFF) | (((x) & 0xFF) << 8))

#endif

#endif /* _NETINET_IN_H */
