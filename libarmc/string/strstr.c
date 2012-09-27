#include <string.h>

char *strstr(const char *haystack, const char *needle) 
{
  do 
  {
    const char *s = haystack;
    const char *n = needle;
    char sc, nc;

    do 
    {
      sc = *s++;
      nc = *n++;
      if (nc == '\0')
	      return (char *) haystack;
    } while (sc == nc);
  } while (*haystack++ != '\0');

  return NULL;
}
