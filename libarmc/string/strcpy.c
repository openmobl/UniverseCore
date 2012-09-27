#include <string.h>


char *strcpy(char *dest, const char *src) 
{
  char *d = dest;

  while ((*d++ = *src++) != '\0')
    ;

  return dest;
}


char *strncpy(char *dest, const char *src, size_t n) 
{
  char *d = dest;
  char c;

  while (n > 0 && (c = *src++) != '\0')
    *d++ = c, n--;

  while (n > 0)
    *d++ = '\0', n--;

  return dest;
}
