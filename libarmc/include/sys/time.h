/******************************************************************************
 *
 * Copyright (c) 1996-2004 PalmSource, Inc. All rights reserved.
 *
 * File: sys_time.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *	  This module contains the equivalent of the unix header <sys/time.h>
 *	for use in Pilot applications that wish to use the Berkeley sockets API
 *
 *****************************************************************************/

#ifndef _SYS_TIME_H
#define _SYS_TIME_H

// So we don't include Tapwave files
#define __SYS_TIME_H__

#include <time.h>

#include <sys/select.h>

/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */

struct timezone {
	int	tz_minuteswest;	/* minutes west of Greenwich */
	int	tz_dsttime;			/* type of dst correction */
};

#define	DST_NONE		0		/* not on dst */
#define	DST_USA		1		/* USA style dst */
#define	DST_AUST		2		/* Australian style dst */
#define	DST_WET		3		/* Western European dst */
#define	DST_MET		4		/* Middle European dst */
#define	DST_EET		5		/* Eastern European dst */
#define	DST_CAN		6		/* Canada */
#define	DST_GB		7		/* Great Britain and Eire */
#define	DST_RUM		8		/* Rumania */
#define	DST_TUR		9		/* Turkey */
#define	DST_AUSTALT	10		/* Australian style with shift in 1986 */

int gettimeofday(struct timeval *tp, void *tzp); /* NOT THREAD SAFE */
int settimeofday(struct timeval* tp, void* tzp);

	
#endif /* _SYS_TIME_H */
