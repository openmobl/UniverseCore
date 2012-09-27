#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: fflush
//
// Parameter: stream    Reference to file
//
// Returnval: 0 (EOF bei Fehler)
//
// Description: Flush buffer of file.
//***********************************************************
int fflush(FILE *stream)
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
  
  if(stream)
  {
  	if(stream->cacheSize) 
  	{
  		//---------------------------------------------
      // Write cached data
  		//---------------------------------------------
  		if(stream->bufSize > 0 && stream->mode == MODE_BUFWRITE)
  		{
  			if(VFSFileWrite(stream->fileRef, stream->bufSize, stream->cache, &lBytesWritten))
  			  return EOF;
  	  }
  	}
  }
    
  return 0;
}


