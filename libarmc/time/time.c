#include <time.h>
#include <M68kCalls.h>


//***********************************************************
// Procedure: time
//
// Parameter: tloc    Pointer to return time
//
// Returnval: Current time
//
// Description: Get current time.
//***********************************************************
time_t time(time_t *tloc)
{
	unsigned long secs;
	unsigned long minSub;
	
  //-----------------------------------
	// since 1/1/1904 12AM.
	//-----------------------------------
	secs = TimGetSeconds();
	secs -= 2082844800; // seconds from 1/1/1904 12AM to 1/1/1970 12AM

  //-----------------------------------
	// Timezone and DST adjust
  //-----------------------------------
	minSub = PrefGetPreference(prefTimeZone);
	secs -= (minSub * 60);
	minSub = PrefGetPreference(prefDaylightSavingAdjustment);
	secs -= (minSub * 60);

  //-----------------------------------
  // Return number of seconds
  //-----------------------------------
	if (tloc)
		*tloc = secs;
	return (secs);
}


clock_t clock(void)
{
  return -1; // Not available
}


double difftime(time_t time2, time_t time1)
{
  return ((double) time2) - ((double) time1);
}


