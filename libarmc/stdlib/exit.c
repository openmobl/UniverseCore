#include <stdlib.h>
#include <setjmp.h>

extern jmp_buf _exit_jmp;

void exit(int exitcode)
{
  if(exitcode == 0)
    exitcode = -1;
  longjmp(_exit_jmp, exitcode);
}

