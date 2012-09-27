#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <M68kCalls.h>



//***********************************************************
// Procedure: realloc
//
// Parameter: ptr     Pointer to current memory
//            size    Size of new memory block
//
// Returnval: Pointer to memory or NULL on error
//
// Description: Allocate memory and init with 0.
//***********************************************************
void *realloc(void *ptr, size_t size)
{
	void *mem;
  size_t oldsize = 0;
  
  //---------------------------------------------
  // Get size of current mem
  //---------------------------------------------
  if(ptr)
  {
    oldsize = MemPtrSize(ptr);
  }
  
  //---------------------------------------------
  // Get new memory
  //---------------------------------------------
  mem = malloc(size);
  if(mem && ptr)
  {
    //---------------------------------------------
    // Copy old content to new memory
    //---------------------------------------------
    if(size < oldsize)
      oldsize = size;
    memmove(mem, ptr, oldsize);

    //---------------------------------------------
    // Free old mem when we have new
    //---------------------------------------------
    free(ptr);
  }
  
  return mem;
}


