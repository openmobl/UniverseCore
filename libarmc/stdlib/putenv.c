#include <stdlib.h>
#include <string.h>

int putenv(const char *str)
{
  char *p, *equal;
  int rval;

  p = strdup(str);

  if (!p)
    return 1;

  if (!(equal = strchr(p, '=')))
  {
    free(p);
    return 1;
  }

  *equal = '\0';
  rval = setenv(p, equal + 1, 1);
  free(p);

  return rval;
}

