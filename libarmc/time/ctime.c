#include <time.h>


char *ctime(const time_t *tim_p)
{
  return asctime(localtime(tim_p));
}

