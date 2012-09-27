#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: fwrite
//
// Parameter: ptr       Pointer to data
//            size      Size of one element
//            nitems    Number of elements to write
//            stream    Reference to file
//
// Returnval: Number of items written.
//
// Description: Write to file
//***********************************************************
size_t fwrite(const void *ptr, size_t size, size_t nitems, FILE *stream)
{
	UInt16 error = 0;
	UInt32 lBytesWritten;
	UInt32 lBytesToWrite = (size * nitems);

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
  if(!ptr || !stream || lBytesToWrite == 0)
    return 0;

  //---------------------------------------------
	// try to write on a read only stream ?
  //---------------------------------------------
	if (stream->mode == MODE_BUFREAD || !lBytesToWrite)
		return 0;

	if (stream->cacheSize) 
	{
    //---------------------------------------------
    // Write using cache
    //---------------------------------------------
		if((stream->bufSize + lBytesToWrite) <= stream->cacheSize)
    {
      //---------------------------------------------
      // Copy data to cache
      //---------------------------------------------
			memmove((stream->cache + stream->bufSize), ptr, lBytesToWrite);
			stream->bufSize += lBytesToWrite;
			lBytesWritten = lBytesToWrite;
    }
    else
    {
      //---------------------------------------------
      // Write cache and data
      //---------------------------------------------
			if(stream->bufSize > 0)
			  error = VFSFileWrite(stream->fileRef, stream->bufSize, stream->cache, &lBytesWritten);
			error = VFSFileWrite(stream->fileRef, lBytesToWrite, ptr, &lBytesWritten);
			stream->bufSize = 0;
			lBytesWritten = lBytesToWrite;
		}    
  }
  else
  {
    //---------------------------------------------
    // Write direct to card
    //---------------------------------------------
		error = VFSFileWrite(stream->fileRef, lBytesToWrite, ptr, &lBytesWritten);
  }
  
	if(error == errNone || error == vfsErrFileEOF) 
	{
		return (size_t) (lBytesWritten / size);
	}

	return 0;
}

