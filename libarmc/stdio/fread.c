#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: fread
//
// Parameter: ptr       Pointer for data
//            size      Size of one element
//            nitems    Number of elements to read
//            stream    Reference to file
//
// Returnval: Number of items read.
//
// Description: Read from file
//***********************************************************
size_t fread(void *ptr, size_t size, size_t nitems, FILE *stream)
{
	UInt16 error = 0;
	UInt32 lBytesRead;
	UInt32 lBytesToRead = (size * nitems);

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
  // Check pointers
  //---------------------------------------------
  if(!ptr || !stream)
    return 0;

  //---------------------------------------------
	// try to read on a write only stream ?
  //---------------------------------------------
	if (stream->mode == MODE_BUFWRITE || !lBytesToRead)
		return 0;

	if (stream->cacheSize) 
	{
    //---------------------------------------------
    // Read using cache
    //---------------------------------------------
		if (stream->bufSize == 0 && lBytesToRead < stream->cacheSize) 
		{
      //---------------------------------------------
      // Fill cache
      //---------------------------------------------
			error = VFSFileRead(stream->fileRef, stream->cacheSize, stream->cache, &lBytesRead);
			stream->bufSize = lBytesRead;
			stream->bufPos = 0;
		} 

		if (stream->bufSize >= lBytesToRead) 
		{
      //---------------------------------------------
      // Use data from cache
      //---------------------------------------------
			memmove(ptr, (stream->cache + stream->bufPos), lBytesToRead);
			stream->bufPos += lBytesToRead;
			stream->bufSize -= lBytesToRead;
			lBytesRead = lBytesToRead;
		} 
		else if (stream->bufSize > 0) 
		{
      //---------------------------------------------
      // Use rest of buffer
      //---------------------------------------------
			UInt8 *next = (UInt8 *) ptr;
			memmove(ptr, (stream->cache + stream->bufPos), stream->bufSize);
			lBytesToRead -= stream->bufSize;
			error = VFSFileRead(stream->fileRef, lBytesToRead, (next + stream->bufSize), &lBytesRead);
			lBytesRead += stream->bufSize;
			stream->bufSize = 0;
			stream->bufPos = 0;
		} 
		else 
		{
      //---------------------------------------------
      // Nothing in buffer -> read from card
      //---------------------------------------------
			error = VFSFileRead(stream->fileRef, lBytesToRead, ptr, &lBytesRead);
		}
  }
  else
  {
    //---------------------------------------------
    // Read direct from card
    //---------------------------------------------
		error = VFSFileRead(stream->fileRef, lBytesToRead, ptr, &lBytesRead);
  }
  
  //---------------------------------------------
  // Return number of read elements if successful
  //---------------------------------------------
	if (error == errNone || error == vfsErrFileEOF)
		return (UInt32) (lBytesRead / size);

  return 0;
}


