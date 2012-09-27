/*
 *    POSIX Standard: 2.6 Primitive System Data Types    <sys/types.h>
 */

#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H    1

#ifndef _USIGN_T
#define _USING_T
typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;
#endif

#ifndef _OFF_T
#define _OFF_T
typedef long int off_t;
#endif

#ifndef _SIZE_T
#define	_SIZE_T
typedef unsigned long	size_t;
typedef long ssize_t;
#endif

#ifndef _MODE_T
#define _MODE_T
typedef int mode_t;
#endif

#ifndef _NLINK_T
#define _NLINK_T
typedef int nlink_t;
#endif

#ifndef _UID_T
#define _UID_T
typedef int uid_t;
#endif

#ifndef _GID_T
#define _GID_T
typedef int gid_t;
#endif

#ifndef _ID_T
#define _ID_T
typedef int id_t;
#endif

#ifndef _BLKCNT_T
#define _BLKCNT_T
typedef signed int blkcnt_t;
#endif

#ifndef _OFF_T
#define _OFF_T
typedef signed int off_t;
#endif

#ifndef _FSBLKCNT_T
#define _FSBLKCNT_T
typedef unsigned int fsblkcnt_t;
#endif

#ifndef _FSFILCNT_T
#define _FSFILCNT_T
typedef unsigned int fsfilcnt_t;
#endif

#ifndef _INO_T
#define _INO_T
typedef unsigned int ino_t;
#endif

#ifndef _BLKSIZE_T
#define _BLKSIZE_T
typedef signed int blksize_t;
#endif

#ifndef _PID_T
#define _PID_T
typedef signed int pid_t;
#endif

#ifndef _TIME_T
#define _TIME_T
typedef int time_t;
#endif

#ifndef _CLOCK_T
#define _CLOCK_T
typedef int clock_t;
#endif

#ifndef _DEV_T
#define _DEV_T
typedef int dev_t;
#endif


#endif /* sys/types.h */
