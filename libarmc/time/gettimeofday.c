#include <sys/time.h>
#include <errno.h>
#include <M68kCalls.h>

static 	unsigned long   PrvSecondsOffset = 0;

/* NOT THREAD SAFE */
int gettimeofday(struct timeval *tp, void *tzp)
{
    unsigned long   ticks, secondsOn, subTicksOn, estSeconds;
    unsigned long   actSeconds;
    long            dSecs;


    // If tzp is non-nil, get the time zone info
    if (tzp) {
        ((struct timezone *)tzp)->tz_minuteswest    = PrefGetPreference(prefMinutesWestOfGMT);
        ((struct timezone *)tzp)->tz_dsttime        = DST_NONE;
    }

    // If no tp, do nothing
    if (!tp) {
        return 0;
    }


    // Get the current value from our real-time clock and our current
    //  tick count.
    actSeconds  = TimGetSeconds();
    ticks       = TimGetTicks();


    //---------------------------------------------------------------
    // We use ticks to estimate the time since we desire microseconds
    //  granularity, not 1 second granularity like our real-time clock
    //  provides.
    //---------------------------------------------------------------
    secondsOn   = ticks / SysTicksPerSecond();
    subTicksOn  = ticks % SysTicksPerSecond();


    // If we have a calculated offset (this is not the first invocation),
    //   add that to our current seconds calculation to get the seconds
    //   since 1970.
    estSeconds  = secondsOn + PrvSecondsOffset;



    //---------------------------------------------------------------
    // Now, make sure we're in the ballpark by comparing our tick time
    //  with our real-time clock time.
    //---------------------------------------------------------------
    // If way off, re-calculate PrvSecondsOffset;
    dSecs = actSeconds - estSeconds;
    if (dSecs < 0) {
        dSecs = -dSecs;
    }
    if (dSecs > 10) {
        PrvSecondsOffset    = actSeconds - secondsOn;
        estSeconds          = actSeconds;
        subTicksOn          = 0;
    }


    // Use subTicksOn to estimate microseconds and return the time
    //  since 1970, not 1904 like our native code does.
    tp->tv_sec  = estSeconds - offset1970;
    tp->tv_usec = (subTicksOn * 1000000L) / sysTicksPerSecond;


    // Exit	
    errno = 0;
    return 0;
}
