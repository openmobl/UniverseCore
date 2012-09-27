#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>
#include <errno.h>


#define INTERNAL_BUFFER_LEN 512

static char _buf[INTERNAL_BUFFER_LEN];

//***********************************************************
// Procedure: fprintf
//
// Parameter: stream    Reference to file
//            formatStr Format string
//
// Returnval: Number of items written or -1 on error.
//
// Description: Write string to file
//***********************************************************
int fprintf(FILE *stream, const char *formatStr, ...)
{
	UInt32 lBytesWritten;
	va_list va;

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

  //---------------------------------------------
  // Check pointers
  //---------------------------------------------
	if (!stream || !formatStr)
		return -1;

  //---------------------------------------------
  // Create string to write
  //---------------------------------------------
	va_start(va, formatStr);
	vsprintf(_buf, formatStr, va);
	va_end(va);

	lBytesWritten = fwrite(_buf, 1, strlen(_buf), stream);
	return lBytesWritten;
}


int	fputc(int c, FILE *stream)
{
  unsigned char _c = (unsigned char) c;
  
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

  if(fwrite(&_c, 1, 1, stream) != 1)
    return EOF;
  return c;
}


int	fputs(const char *string, FILE *stream)
{
  int len;

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

  //---------------------------------------------
  // Check pointers
  //---------------------------------------------
	if (!stream || !string)
		return EOF;

  len = strlen(string);
  if(fwrite(string, len, 1, stream) != len)
    return EOF;
  return len;
}


int	vfprintf(FILE *stream, const char *formatStr, va_list va)
{
	UInt32 lBytesWritten;

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

  //---------------------------------------------
  // Check pointers
  //---------------------------------------------
	if (!stream || !formatStr)
		return -1;

  //---------------------------------------------
  // Create string to write
  //---------------------------------------------
	vsprintf(_buf, formatStr, va);

	lBytesWritten = fwrite(_buf, 1, strlen(_buf), stream);
	return lBytesWritten;
}


void perror(const char *str)
{
  fprintf(stderr, "%s: errno=%d\n", str, errno);
}


