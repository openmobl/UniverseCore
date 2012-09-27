#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: fclose
//
// Parameter: stream    Reference to file
//
// Returnval: 0
//
// Description: Close file.
//***********************************************************
int fclose(FILE *stream)
{
	UInt16 error;
  UInt32 lBytesWritten;
  
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
  
  if(stream)
  {
  	if(stream->cacheSize) 
  	{
  		//---------------------------------------------
      // Write cached data
  		//---------------------------------------------
  		if(stream->bufSize > 0 && stream->mode == MODE_BUFWRITE)
  			VFSFileWrite(stream->fileRef, stream->bufSize, stream->cache, &lBytesWritten);
  	}

		//---------------------------------------------
    // Close file
		//---------------------------------------------
    if(stream->fileRef)
      error = VFSFileClose(stream->fileRef);
    
		//---------------------------------------------
		// Release memory of cache
		//---------------------------------------------
    if (stream->cache)
      free(stream->cache);
    
  	//---------------------------------------------
  	// Release memory of file struct
  	//---------------------------------------------
    free(stream);
  }
    
  return 0;
}


