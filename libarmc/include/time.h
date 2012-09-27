#ifndef __TIME_H__
#define __TIME_H__

#include <sys/types.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


struct tm {
	short tm_sec;		//	seconds [0,61]
	short tm_min;		//	minutes [0,59]
	short tm_hour;	//	hour [0,23]
	short tm_mday;	//	day of month [1,31]
	short tm_mon;		//	month of year [0,11]
	short tm_year;	//	years since 1900
	short tm_wday;	//	day of week [0,6] (Sunday = 0)
	short tm_yday;	//	day of year [0,365]
	short tm_isdst;	//	daylight savings flag
};


extern time_t time(time_t *tloc);
extern struct tm *localtime(const time_t *timer);
extern clock_t clock(void);
extern double difftime(time_t time2, time_t timel);
extern char *asctime(const struct tm *tim_p);
extern char *ctime(const time_t *tim_p);
extern time_t mktime(struct tm *tim_p);
extern struct tm *gmtime(const time_t *timer);

// Not yet implemented
//extern size_t strftime(char *s, size_t smax, const char *fmt, const struct tm *tp)


#ifdef __cplusplus
}
#endif

#endif /* __TIME_H__ */


