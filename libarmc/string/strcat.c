#include <string.h>

char *strcat (char *dest, const char *src) 
{
  char *d = dest;

  while (*d++ != '\0')
    ;
  d--;

  while ((*d++ = *src++) != '\0')
    ;

  return dest;
}


char *strncat(char *dest, const char *src, size_t n) 
{
  char *d = dest;
  char c;

  while (*d++ != '\0')
    ;
  d--;

  while (n > 0 && (c = *src++) != '\0')
    *d++ = c, n--;

  *d = '\0';
  return dest;
}
