/******************************************************************************
 *
 * Copyright (c) 1996-2004 PalmSource, Inc. All rights reserved.
 *
 * File: sys_uio.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *      This module contains the interface definitions that are 
 *    typically found in the unix header <sys/uio.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

/*    @(#)uio.h 2.9 89/06/27 SMI; from UCB 7.1 6/4/86    */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#ifndef _SYS_UIO_H
#define _SYS_UIO_H


struct    iovec {
    void *    iov_base;
/* The size_t way is probably the standards way. The unsigned short is the PalmOS way... */
#if 0
    size_t    iov_len;
#else
    unsigned short  iov_len;
#endif
};


#endif /* _SYS_UIO_H */
