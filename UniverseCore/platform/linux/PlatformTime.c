/********************************************************************************
 * UniverseCore Web Core                                                        *
 * Copyright (c) 2007 OpenMobl Systems                                          *
 * Copyright (c) 2006-2007 Donald C. Kirker                                     *
 *                                                                              *
 * http://www.openmobl.com/                                                     *
 * dev-support@openmobl.com                                                     *
 *                                                                              *
 * This library is free software; you can redistribute it and/or                *
 * modify it under the terms of the GNU Lesser General Public                   *
 * License as published by the Free Software Foundation; either                 *
 * version 2.1 of the License, or (at your option) any later version.           *
 *                                                                              *
 * This library is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU            *
 * Lesser General Public License for more details.                              *
 *                                                                              *
 * You should have received a copy of the GNU Lesser General Public             *
 * License along with this library; if not, write to the Free Software          *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA*
 *                                                                              *
 ********************************************************************************/

#include "LinuxConfig.h"
#include "PlatformTime.h"

#include <time.h>

static unsigned long timeMakePartsTime(char *weekDay, char *day, char *month, char *year,
                                       char *hour, char *min, char *sec);

unsigned long timeGetCurrentTick(void)
{
    return clock();
}

unsigned long timeGetTicksPerSecond(void)
{
    return CLOCKS_PER_SEC;
}

unsigned long timeSinceEpoch(void)
{
    return time(NULL);
}


/*
  Parses dates in the format:
  
      Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
      Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
      Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format
      
 */

typedef enum TimeDateEnum {
    dateRFC822,
    dateRFC850,
    dateANSI
} TimeDateEnum;

static unsigned long timeMakePartsTime(char *weekDay, char *day, char *month, char *year,
                                       char *hour, char *min, char *sec)
{
    char        *monthArray[13]     = { "january", "february", "march", "april", "may", "june", "july",
                                        "august", "september", "october", "november", "december", NULL };
    char        *weekDayArray[8]    = { "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", NULL };
    int         monthPart           = 0;
    int         weekDayPart         = 0;
    struct tm   timeParts;
    
    if (!weekDay || !day || !month || !year || !hour || !min || !sec) {
        return 0;
    }
    
    memMgrChunkSet(&timeParts, 0, sizeof(struct tm));
    
    timeParts.tm_sec    = stringAToI(sec);
    timeParts.tm_min    = stringAToI(min);
    timeParts.tm_hour   = stringAToI(hour);
    timeParts.tm_mday   = stringAToI(day);
    while (monthArray[monthPart] && stringNCompareCaseless(month, monthArray[monthPart],
                                                           MIN(stringLen(monthArray[monthPart]), stringLen(month))))
    {
        monthPart++;
    }
    if (monthPart == 12)
        return 0;
    timeParts.tm_mon    = monthPart;
    timeParts.tm_year   = stringAToI(year) - 1900;
    while (weekDayArray[weekDayPart] && stringNCompareCaseless(weekDay, weekDayArray[weekDayPart],
                                                           MIN(stringLen(weekDayArray[weekDayPart]), stringLen(weekDay))))
    {
        weekDayPart++;
    }
    if (weekDayPart == 7)
        return 0;
    timeParts.tm_wday   = weekDayPart;
    
    return mktime(&timeParts);
}

unsigned long timeStringToSeconds(char *time)
{
    unsigned long       i               = 0;
    char                *timeP          = time;
    char                weekDay[50];
    char                day[50];
    char                month[50];
    char                year[50];
    char                hour[50];
    char                min[50];
    char                sec[50];
    TimeDateEnum        dateType        = dateRFC822;
    
    if (!time || !stringLen(time))
        return 0;
    
    // We should start off with the day of a week
    if (!stringIsAlpha(time[0]))
        return 0;
    
    i = 0;
    while (stringIsAlpha(*timeP) && (i < 50)) {
        weekDay[i] = *timeP++;
        i++;
    }
    weekDay[i] = '\0';
    
    while (!stringIsAlpha(*timeP) && !stringIsNumeric(*timeP))
        timeP++;
    
    // Now we do some weird magic...
    if (stringIsAlpha(*timeP)) {
        dateType = dateANSI;
        
        i = 0;
        while (stringIsAlpha(*timeP) && (i < 50)) {
            month[i] = *timeP++;
            i++;
        }
        month[i] = '\0';
        
        while (*timeP && !stringIsAlpha(*timeP) && !stringIsNumeric(*timeP))
            timeP++;
    }
    
    // Sanity check
    if (!*timeP || !stringIsNumeric(*timeP))
        return 0;
    
    // Lets get the Day, all formats should be at the same point
    i = 0;
    while (stringIsNumeric(*timeP) && (i < 50)) {
        day[i] = *timeP++;
        i++;
    }
    day[i] = '\0';
    
    if (*timeP == '-') {
        dateType = dateRFC850;
    }
    
    // If we are dateType of anything but ANSI, then pull the month and year
    if (dateType != dateANSI) {
        while (!stringIsAlpha(*timeP))
            timeP++;
            
        i = 0;
        while (stringIsAlpha(*timeP) && (i < 50)) {
            month[i] = *timeP++;
            i++;
        }
        month[i] = '\0';
        
        while (!stringIsNumeric(*timeP))
            timeP++;
            
        i = 0;
        while (stringIsNumeric(*timeP) && (i < 50)) {
            year[i] = *timeP++;
            i++;
        }
        year[i] = '\0';
    }
    
    while (!stringIsNumeric(*timeP))
        timeP++;
    
    // Get the hour:min:sec
    i = 0;
    while (stringIsNumeric(*timeP) && (i < 50)) {
        hour[i] = *timeP++;
        i++;
    }
    hour[i] = '\0';

    while (!stringIsNumeric(*timeP))
        timeP++;
        
    i = 0;
    while (stringIsNumeric(*timeP) && (i < 50)) {
        min[i] = *timeP++;
        i++;
    }
    min[i] = '\0';
    
    while (!stringIsNumeric(*timeP))
        timeP++;
        
    i = 0;
    while (stringIsNumeric(*timeP) && (i < 50)) {
        sec[i] = *timeP++;
        i++;
    }
    sec[i] = '\0';
    
    // If ANSI, get the Year
    if (dateType == dateANSI) {
        while (!stringIsNumeric(*timeP))
            timeP++;
        
        i = 0;
        while (stringIsNumeric(*timeP) && (i < 50)) {
            year[i] = *timeP++;
            i++;
        }
        year[i] = '\0';
    }
    
    // TODO: Should we handle the Timezone in !dateASNI ?
    
    // We now have the date in strings, now we need to get the seconds out.
    
        
    return timeMakePartsTime(weekDay, day, month, year, hour, min, sec);
}
