#include <implementation.h>
#include <stdlib.h>
#include <M68kCalls.h>

#ifdef CHECK_MEMUSAGE
unsigned long _max_mem_usage;
unsigned long _curr_mem_usage;
#endif


MemPtr MemGluePtrNew(UInt32 size)
{
    // Allocate a chunk of memory where the size of the memory might exceed
    // 64k.  There are a few potential problems here:
    //  (a) the memory cannot be saved to a database
    //  (b) if you forget to free the large chunk before
    //      you application quits, you will cause a MEMORY LEAK on
    //      Palm OS 5 and 5.1 .
    if (size > 65500L) {
        SysAppInfoType *unusedAppInfoP;
        UInt16 currentOwnerID = SysGetAppInfo(&unusedAppInfoP, &unusedAppInfoP)->memOwnerID;
        return MemChunkNew(0, size, currentOwnerID | memNewChunkFlagNonMovable | memNewChunkFlagAllowLarge);
    }
    
    return MemPtrNew(size);
}

void *malloc(size_t size)
{
  void *ptr = 0;
  
  if(size == 0)
    return 0;

#if 0  
  if(size < 64000)
  {
    ptr = (void *) Call68KFunc(PceNativeTrapNo(sysTrapMemPtrNew), &sizeBE, 4 | kPceNativeWantA0);
  }
  else
  {
    if(MemGluePtrNew)
    {
      ptr = (void *) Call68KFunc((unsigned long) MemGluePtrNew, &sizeBE, 4 | kPceNativeWantA0);
    }
    else
      return 0;
  }
#endif

  ptr = MemGluePtrNew(size);
    
#ifdef CHECK_MEMUSAGE
  if(ptr)
  {
    _curr_mem_usage = _curr_mem_usage + size;
    if(_curr_mem_usage > _max_mem_usage)
      _max_mem_usage = _curr_mem_usage;
  }
#endif

  return ptr;
}


void free(void *ptr)
{
  if(ptr)
  {
#ifdef CHECK_MEMUSAGE
    _curr_mem_usage = _curr_mem_usage - MemPtrSize(ptr);
#endif
  	MemPtrFree(ptr);
  }
}


