#include <string.h>

char *strrchr(const char *s, int ic) 
{
  char c = ic;
  const char *location = NULL;

  do 
  {
    if (*s == c)
      location = s;
  } while (*s++ != '\0');

  return (char *) location;
}
