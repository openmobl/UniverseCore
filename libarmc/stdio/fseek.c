#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: fseek
//
// Parameter: stream    Reference to file
//            offset    Offset to origin
//            origin    Start, set or end
//            
//
// Returnval: 0 if OK, other on error
//
// Description: Set position in open file.
//***********************************************************
int fseek	(FILE *stream, long offset, int origin)
{
	UInt32 lBytesWritten;
	UInt16 error;

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

	if (stream->cacheSize) 
	{
		switch (stream->mode) 
		{
			case MODE_BUFWRITE:
			  //---------------------------------------------
			  // Write cache data before seek
			  //---------------------------------------------
				error = VFSFileWrite(stream->fileRef, stream->bufSize, stream->cache, &lBytesWritten);
				stream->bufSize = 0;
				break;

			case MODE_BUFREAD:
			  //---------------------------------------------
				// Reposition file position if needed
				//---------------------------------------------
				if (origin == SEEK_CUR)
					error = VFSFileSeek(stream->fileRef, vfsOriginCurrent, -stream->bufSize);
				stream->bufSize = 0;
				stream->bufPos = 0;
				break;
		}
	}
		
	error = VFSFileSeek(stream->fileRef, origin, offset);
	return (error ? -1 : 0);
}


void rewind(FILE *stream)
{
  fseek(stream, 0, SEEK_SET); 
  clearerr(stream); 
}

