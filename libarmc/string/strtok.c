#include <string.h>

static char *_saveds = 0;

char *strtok(char *s, const char *delim) 
{
  char *token = NULL;

  if (s == NULL)
    s = _saveds;

  s += strspn (s, delim);

  if (*s) 
  {
    token = s;
    s += strcspn (s, delim);
    if (*s)
      *s++ = '\0';
  }

  _saveds = s;
  return token;
}
