/******************************************************************************
 *
 * Copyright (c) 1996-2004 PalmSource, Inc. All rights reserved.
 *
 * File: netdb.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *      This module contains the interface definitions that are 
 *    typically found in the unix header <netdb.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

/*    @(#)netdb.h 1.11 88/08/19 SMI from UCB 5.9 4/5/88    */
/*
 * Copyright (c) 1980,1983,1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 */



#ifndef _NETDB_H
#define _NETDB_H

#include <stdint.h>
#include <sys/socket.h>

// NOTE: The u_int16_t in the *ent structs would normally be int

/*
 * Structures returned by network data base library.
 * All addresses are supplied in host order, and
 * returned in network order (suitable for use in system calls).
 */

struct    hostent {
    char        *h_name;                        /* official name of host */
    char        **h_aliases;                /* alias list */
    u_int16_t   h_addrtype;                /* host address type */
    u_int16_t   h_length;                    /* length of address */
    char        **h_addr_list;                /* list of addresses from name server */
#define    h_addr    h_addr_list[0]    /* address, for backward compatiblity */
};

/*
 * Assumption here is that a network number
 * fits in 32 bits -- probably a poor one.
 */
struct    netent {
    char        *n_name;                    /* official name of net */
    char        **n_aliases;            /* alias list */
    u_int16_t   n_addrtype;                /* net address type */
    uint32_t    n_net;                    /* network # */
};

struct    servent {
    char        *s_name;                        /* official service name */
    char        **s_aliases;                /* alias list */
    u_int16_t   s_port;                        /* port # */
    char        *s_proto;                    /* protocol to use */
};

struct    protoent {
    char        *p_name;                        /* official protocol name */
    char        **p_aliases;                /* alias list */
    int         p_proto;                    /* protocol # */
};

struct rpcent {
    char        *r_name;                        /* name of server for this rpc program */
    char        **r_aliases;                /* alias list */
#if 0
    int         r_number;                    /* rpc program number */
#else
    int16_t     r_number;
#endif
};



/*
 * Error return codes from gethostbyname() and gethostbyaddr()
 * (when using the resolver)
 */

extern  int         h_errno;                

#define    HOST_NOT_FOUND    1                 /* Authoritive Answer Host not found */
#define    TRY_AGAIN        2                 /* Non-Authoritive Host not found, or SERVERFAIL */
#define    NO_RECOVERY        3                 /* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
#define    NO_DATA            4                 /* Valid name, no data record of requested type */
#define    NO_ADDRESS        NO_DATA        /* no address, look for MX record */


#ifdef	__cplusplus
extern "C" {
#endif

struct hostent *gethostbyaddr(const void *addr, socklen_t len,
       int type);

struct hostent *gethostbyname(const char *name);

struct servent *getservbyname(const char *name, const char *proto);


// Unsupported calls
// struct hostent     *gethostent();
// struct netent    *getnetbyname(), *getnetbyaddr(), *getnetent();
// struct servent    *getservbyport(), *getservent();
// struct protoent    *getprotobyname(), *getprotobynumber(), *getprotoent();
// struct rpcent    *getrpcbyname(), *getrpcbynumber(), *getrpcent();

#ifdef	__cplusplus
}
#endif

#endif    /* __UNIX_NETDB_H__ */
