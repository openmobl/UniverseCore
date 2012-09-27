#include <string.h>

char *strchr (const char *s, int ic) 
{
  char c = ic;

  while (*s != c)
  {
    if (*s++ == '\0')
      return NULL;
  }
  
  return (char *) s;
}
