#include <string.h>

size_t strspn(const char *s, const char *accept) 
{
  const char *start = s;

  while (1) 
  {
    char c = *s++;
    const char *accs = accept;
    char ac;

    do 
    {
      ac = *accs++;
      if (ac == '\0')
	      return --s - start;
    } while (c != ac);
  }
}
