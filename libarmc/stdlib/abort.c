#include <stdlib.h>
#include <setjmp.h>

extern jmp_buf _exit_jmp;

void abort(void)
{
  longjmp(_exit_jmp, -1);
}
