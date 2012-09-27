#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: fgets
//
// Parameter: s         Buffer for data
//            n         Max. number of characters
//            stream    Reference to file
//
// Returnval: s when OK or NULL on failure
//
// Description: Get line from file.
//***********************************************************
char *fgets(char *s, int n, FILE *stream)
{
	UInt32 lBytesRead = 0;
  int chr;
  
  //---------------------------------------------
  // Check pointers
  //---------------------------------------------
  if(!s || !stream)
    return NULL;
    
  //---------------------------------------------
  // Read requested number of bytes
  //---------------------------------------------
  while(lBytesRead < n - 1)
  {
    chr = fgetc(stream);
    if(chr == EOF)
      break; // End of file -> exit loop
    s[lBytesRead] = (char) chr;
    ++lBytesRead;
    if((char) chr == '\n' || chr == '\r')
      break; // End of line -> exit loop
  }
  
	if(lBytesRead) 
	{
    //---------------------------------------------
    // We have at least one char -> return string
    //---------------------------------------------
    s[lBytesRead] = '\0';
		return s;
	}

	return NULL;
}




