#include <string.h>
#include <ctype.h>

char *strupr(char *a)
{
  char *ret = a;

  while (*a != '\0')
  {
    if (islower (*a))
    	*a = toupper (*a);
    ++a;
  }

  return ret;
}
