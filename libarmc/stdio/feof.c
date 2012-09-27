#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: feof
//
// Parameter: stream    Reference to file
//
// Returnval: True if end of file reached
//
// Description: Check for end of file.
//***********************************************************
int feof(FILE *stream)
{
	UInt16 error;
	
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
  // Check pointer
  //---------------------------------------------
  if(!stream)
    return 1;

	if (stream->cacheSize) 
	{
		switch (stream->mode) 
		{
			case MODE_BUFWRITE:
				return 0;	// EOF never reached when file open for write
				
			case MODE_BUFREAD:
				if (stream->bufSize > 0)
					return 0;
				break;
		}
	}
	
	error = VFSFileEOF(stream->fileRef);
	return error;
}


