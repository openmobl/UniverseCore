#include <string.h>

void *memchr(const void *sv, int ic, size_t n) 
{
  const unsigned char *s = sv;
  unsigned char c = ic;

  while (n > 0) 
  {
    if (*s++ == c)
      return (void *) --s;
    n--;
  }

  return NULL;
}
