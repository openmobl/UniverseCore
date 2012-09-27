#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: ftell
//
// Parameter: stream    Reference to file
//
// Returnval: Current position or -1 on error
//
// Description: Get position in open file.
//***********************************************************
long ftell(FILE *stream)
{
	UInt16 error;
  UInt32 filePos;
  
  //--------------------------------
  // Check for stdxxx
  //--------------------------------
  switch((int) stream)
  {
    case _stdin:
    case _stdout:
    case _stderr:
      return 0;
  }

  //---------------------------------------------
  // Check pointer
  //---------------------------------------------
  if(!stream)
    return -1;

  //---------------------------------------------
  // Get position in file
  //---------------------------------------------
  error = VFSFileTell(stream->fileRef ,&filePos);
	if(error) 
	  return -1;
 
	if (stream->cacheSize) 
	{
		switch (stream->mode) 
		{
			case MODE_BUFWRITE:
			  //---------------------------------------------
			  // Add position in cache
			  //---------------------------------------------
				filePos += stream->bufSize;
				break;

			case MODE_BUFREAD:
			  //---------------------------------------------
				// Substract position in cache
				//---------------------------------------------
				filePos -= stream->bufSize;
				break;
		}
	}
		
	return filePos;
}


int fgetpos(FILE *stream, fpos_t *ptr)
{
  long pos = ftell(stream);
  if(pos >= 0)
  {
    if(ptr)
      *ptr = pos;
    return 0;
  }
  return -1;
}


int fsetpos(FILE *stream, const fpos_t *ptr)
{
  if(stream && ptr)
    return fseek(stream, *ptr, SEEK_SET);
  return -1;
}


