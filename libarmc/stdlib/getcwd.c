#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


//---------------------------------------------
// Default working directory (set by 68k code)
//---------------------------------------------
const char *CurrentWorkingDir = "/PALM/";


//***********************************************************
// Procedure: getcwd
//
// Parameter: tloc    Pointer to return time
//
// Returnval: Current time
//
// Description: Get current working directoy.
//***********************************************************
char *getcwd(char *buf, size_t size)
{
  if(strlen(CurrentWorkingDir) >= size) 
  {
    errno = ERANGE;
    return 0;
  }
  
  if (!buf) 
  {
    buf = (char*) malloc(size);
    if(!buf) 
    {
      errno = ENOMEM;
	    return 0;
    }
  }

  strcpy (buf, CurrentWorkingDir);
  return buf;
}



