#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//***********************************************************
// Procedure: calloc
//
// Parameter: nobj    Number of objects
//            size    Size of each object
//
// Returnval: Pointer to memory or NULL on error
//
// Description: Allocate memory and init with 0.
//***********************************************************
void *calloc(size_t nobj, size_t size)
{
	int bytes;
	void *mem;
	
  //---------------------------------------------
  // Get and init memory
  //---------------------------------------------
  bytes = nobj * size;
  mem = malloc(bytes);
  if(mem)
    memset(mem, 0, bytes);
  return mem;
}


