#include <time.h>
#include <stdint.h>
#include <M68kCalls.h>

struct tm *gmtime(const time_t *timer)
{    
    int16_t     timeZone                    = PrefGetPreference(prefTimeZone);
    int16_t     daylightSavingAdjustment    = PrefGetPreference(prefDaylightSavingAdjustment);
    time_t      utcTime                     = 0;
    struct tm   *lTime                      = NULL;
    
    utcTime = TimTimeZoneToUTC(*timer, timeZone, daylightSavingAdjustment);
    
    lTime   = localtime(&utcTime);
    
    return lTime;
}
