#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: ferror
//
// Parameter: stream    Reference to file
//
// Returnval: Errorcode
//
// Description: Get error state of file.
//***********************************************************
int ferror(FILE *stream)
{
  //--------------------------------
  // Check for stdxxx
  //--------------------------------
  switch((int) stream)
  {
    case _stdin:
      return 0;
    case _stdout:
      stream = fstdout;
      break;
    case _stderr:
      stream = fstderr;
      break;
  }

  if(stream)
	  return (stream->err);
	return 0;
}


void clearerr(FILE *stream)
{
  //--------------------------------
  // Check for stdxxx
  //--------------------------------
  switch((int) stream)
  {
    case _stdin:
      return;
    case _stdout:
      stream = fstdout;
      break;
    case _stderr:
      stream = fstderr;
      break;
  }

  if(stream)
	  stream->err = 0;
}




