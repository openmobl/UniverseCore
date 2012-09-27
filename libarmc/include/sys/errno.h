#ifndef _SYS_ERRNO_H
#define	_SYS_ERRNO_H


#include <ArmNetMgr.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define	EPERM	            1
#define	ENOENT	          2
#define	ESRCH	            3
#define	EINTR	            4
#define	EIO             	5
#define	ENXIO	            6
#define	E2BIG	            7
#define	ENOEXEC	          8
#define	EBADF           	9
#define	ECHILD	          10
#define	EAGAIN	          11
#define	ENOMEM	          12
#define	EACCES	          13
#define	EFAULT	          14
#define	ENOTBLK	          15
#define	EBUSY	            16
#define	EEXIST	          17
#define	EXDEV	            18
#define	ENODEV	          19
#define	ENOTDIR	          20
#define	EISDIR	          21
#define	EINVAL	          22
#define	ENFILE	          23
#define	EMFILE	          24
#define	ENOTTY	          25
#define	ETXTBSY	          26
#define	EFBIG	            27
#define	ENOSPC	          28
#define	ESPIPE	          29
#define	EROFS	            30
#define	EMLINK	          31
#define	EPIPE	            32
#define	EDOM	            33
#define	ERANGE	          34
#define	ENOMSG	          35
#define	EIDRM	            36
#define	ECHRNG	          37
#define	EL2NSYNC          38	
#define	EL3HLT	          39
#define	EL3RST	          40
#define	ELNRNG	          41
#define	EUNATCH           42
#define	ENOCSI	          43
#define	EL2HLT	          44
#define	EDEADLK	          45
#define	ENOLCK	          46
#define	ECANCELED         47
#define	ENOTSUP	          48
#define	EDQUOT	          49
#define	EBADE	            50
#define	EBADR	            51
#define	EXFULL	          52
#define	ENOANO	          53
#define	EBADRQC	          54
#define	EBADSLT	          55
#define	EDEADLOCK         56
#define	EBFONT	          57
#define	ENOSTR	          60
#define	ENODATA	          61
#define	ETIME	            62
#define	ENOSR	            63
#define	ENONET	          64
#define	ENOPKG	          65
#define	EREMOTE	          66
#define	ENOLINK	          67
#define	EADV	            68
#define	ESRMNT	          69
#define	ECOMM	            70
#define	EPROTO	          71
#define	EMULTIHOP         74
#define	EBADMSG           77
#define	ENAMETOOLONG      78
#define	EOVERFLOW         79
#define	ENOTUNIQ          80
#define	EBADFD	          81
#define	EREMCHG	          82
#define	ELIBACC	          83
#define	ELIBBAD	          84
#define	ELIBSCN	          85
#define	ELIBMAX	          86
#define	ELIBEXEC          87	
#define	EILSEQ	          88
#define	ENOSYS	          89
#define	ELOOP	            90
#define	ERESTART          91
#define	ESTRPIPE          92
#define	ENOTEMPTY         93
#define	EUSERS	          94
#define	ENOTSOCK	        95
#define	EDESTADDRREQ	    96
#define	EMSGSIZE	        97
#define	EPROTOTYPE	      98
#define	ENOPROTOOPT	      99
#define	EPROTONOSUPPORT	  120
#define	ESOCKTNOSUPPORT	  121
#define	EOPNOTSUPP	      122
#define	EPFNOSUPPORT	    123
#define	EAFNOSUPPORT	    124
#define	EADDRINUSE	      125
#define	EADDRNOTAVAIL	    126	
#define	ENETDOWN	        127
#define	ENETUNREACH	      128	
#define	ENETRESET	        129
#define	ECONNABORTED	    130
#define	ECONNRESET	      131
#define	ENOBUFS		        132
#define	EISCONN		        133
#define	ENOTCONN	        134	
#define	ESHUTDOWN	        143
#define	ETOOMANYREFS	    144
#define	ETIMEDOUT	        145
#define	ECONNREFUSED	    146
#define	EHOSTDOWN	        147
#define	EHOSTUNREACH	    148
#define	EALREADY	        149	
#define	EINPROGRESS	      150
#define	ESTALE		        151

#include <ArmNetMgr.h>
#define EWOULDBLOCK         netErrWouldBlock


#if 0
#define EPERM           1               /* Not owner */
#define ENOENT          2               /* No such file or directory */
#define ESRCH           3               /* No such process */
#define EINTR           netErrTimeout	 /* Interrupted system call */
#define EIO             5               /* I/O error */
#define ENXIO           6               /* No such device or address */
#define E2BIG           7               /* Arg list too long */
#define ENOEXEC         8               /* Exec format error */
#define EBADF           9               /* Bad file number */
#define ECHILD          10              /* No children */
#define EAGAIN          11              /* No more processes */
#define ENOMEM          12              /* Not enough core */
#define EACCES          13              /* Permission denied */
#define EFAULT          14              /* Bad address */
#define ENOTBLK         15              /* Block device required */
#define EBUSY           16              /* Mount device busy */
#define EEXIST          17              /* File exists */
#define EXDEV           18              /* Cross-device link */
#define ENODEV          19              /* No such device */
#define ENOTDIR         20              /* Not a directory*/
#define EISDIR          21              /* Is a directory */
#define EINVAL          22              /* Invalid argument */
#define ENFILE          23              /* File table overflow */
#define EMFILE          24              /* Too many open files */
#define ENOTTY          25              /* Not a typewriter */
#define ETXTBSY         26              /* Text file busy */
#define EFBIG           27              /* File too large */
#define ENOSPC          28              /* No space left on device */
#define ESPIPE          29              /* Illegal seek */
#define EROFS           30              /* Read-only file system */
#define EMLINK          31              /* Too many links */
#define EPIPE           32              /* Broken pipe */

/* math software */
#define EDOM            33              /* Argument too large */
#define ERANGE          34              /* Result too large */

/* non-blocking and interrupt i/o */
#define EWOULDBLOCK     netErrWouldBlock  	 	/* Operation would block */
#define EINPROGRESS     netErrSocketBusy   		/* Operation now in progress */
#define EALREADY        netErrAlreadyInProgress   	/* Operation already in progress */
/* ipc/network software */

        /* argument errors */
#define ENOTSOCK        38              /* Socket operation on non-socket */
#define EDESTADDRREQ    39              /* Destination address required */
#define EMSGSIZE        40              /* Message too long */
#define EPROTOTYPE      41              /* Protocol wrong type for socket */
#define ENOPROTOOPT     42              /* Protocol not available */
#define EPROTONOSUPPORT 43              /* Protocol not supported */
#define ESOCKTNOSUPPORT 44              /* Socket type not supported */
#define EOPNOTSUPP      45              /* Operation not supported on socket */
#define EPFNOSUPPORT    46              /* Protocol family not supported */
#define EAFNOSUPPORT    47              /* Address family not supported by proto
col family */
#define EADDRINUSE      netErrPortInUse /* Address already in use */
#define EADDRNOTAVAIL   49              /* Can't assign requested address */

        /* operational errors */
#define ENETDOWN        50              /* Network is down */
#define ENETUNREACH     netErrNoInterfaces              /* Network is unreachable */
#define ENETRESET       52              /* Network dropped connection on reset */
#define ECONNABORTED    53              /* Software caused connection abort */
#define ECONNRESET      54              /* Connection reset by peer */
#define ENOBUFS         55              /* No buffer space available */
#define EISCONN         netErrSocketAlreadyConnected  /* Socket is already connected */
#define ENOTCONN        57              /* Socket is not connected */
#define ESHUTDOWN       58              /* Can't send after socket shutdown */
#define ETOOMANYREFS    59              /* Too many references: can't splice */
#define ETIMEDOUT       netErrTimeout   /* Connection timed out */
#define ECONNREFUSED    61              /* Connection refused */

        /* */
#define ELOOP           62              /* Too many levels of symbolic links */
#define ENAMETOOLONG    63              /* File name too long */

/* should be rearranged */
#define EHOSTDOWN       64              /* Host is down */
#define EHOSTUNREACH    65              /* No route to host */
#define ENOTEMPTY       66              /* Directory not empty */

/* quotas & mush */
#define EPROCLIM        67              /* Too many processes */
#define EUSERS          68              /* Too many users */
#define EDQUOT          69              /* Disc quota exceeded */

/* Network File System */
#define ESTALE          70              /* Stale NFS file handle */
#define EREMOTE         71              /* Too many levels of remote in path */

/* streams */
#define ENOSTR          72              /* Device is not a stream */
#define ETIME           73              /* Timer expired */
#define ENOSR           74              /* Out of streams resources */
#define ENOMSG          75              /* No message of desired type */
#define EBADMSG         76              /* Trying to read unreadable message */

/* SystemV IPC */
#define EIDRM           77              /* Identifier removed */

/* SystemV Record Locking */
#define EDEADLK         78              /* Deadlock condition. */
#define ENOLCK          79              /* No record locks available. */

/* RFS */
#define ENONET          80              /* Machine is not on the network */
#define ERREMOTE        81              /* Object is remote */
#define ENOLINK         82              /* the link has been severed */
#define EADV            83              /* advertise error */
#define ESRMNT          84              /* srmount error */
#define ECOMM           85              /* Communication error on send */
#define EPROTO          86              /* Protocol error */
#define EMULTIHOP       87              /* multihop attempted */
#define EDOTDOT         88              /* Cross mount point (not an error) */
#define EREMCHG         89              /* Remote address changed */

/* POSIX */
#define ENOSYS          90              /* function not implemented */
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_ERRNO_H */
