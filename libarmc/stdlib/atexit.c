#include <stdlib.h>

extern void (*_at_exit_fptr)(void);

int atexit(void (*fptr)(void))
{
  if(_at_exit_fptr)
    return -1;
  _at_exit_fptr = fptr;
  return 0;
}

