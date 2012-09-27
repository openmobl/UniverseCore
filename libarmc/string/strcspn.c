#include <string.h>

size_t strcspn (const char *s, const char *reject) 
{
  const char *start = s;

  while (1) 
  {
    char c = *s++;
    const char *rejs = reject;
    char rejc;
    do 
    {
      rejc = *rejs++;
      if (c == rejc)
	      return --s - start;
    } while (rejc != '\0');
  }
}
