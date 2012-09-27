#include <sys/time.h>
#include <errno.h>
#include <M68kCalls.h>


int settimeofday(struct timeval* tp, void* tzp)
{
    // If no tp, do nothing
    if (!tp) {
        return 0;
    }

    // if timezone info, set that
    if (tzp) {
        PrefSetPreference(prefMinutesWestOfGMT,
                    ((struct timezone *)tzp)->tz_minuteswest);
    }

    // Set the time
    TimSetSeconds(tp->tv_sec + offset1970);

    // Exit
    errno = 0;
    return 0;
}
