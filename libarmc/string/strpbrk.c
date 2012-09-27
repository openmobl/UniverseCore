#include <string.h>

char *strpbrk(const char *s, const char *accept) 
{
  char c, ac;

  while ((c = *s++) != '\0') 
  {
    const char *accs = accept;
    while ((ac = *accs++) != '\0')
    {
      if (c == ac)
	      return (char *) --s;
	  }
  }

  return NULL;
}
