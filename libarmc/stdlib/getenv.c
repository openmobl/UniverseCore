#include <stdlib.h>
#include <stddef.h>
#include <string.h>

extern char **environ;

char *_findenv(const char *name, int *offset)
{
  const char *c;
  int len;
  char **p;
  
  //-------------------------------------
  // Check pointers
  //-------------------------------------
  if(!environ || !name || !offset)
    return NULL;
  
  //-------------------------------------
  // Search for '=' or end of string
  //-------------------------------------
  c = name;
  len = 0;
  while(*c && *c != '=')
  {
    c++;
    len++;
  }

  //-------------------------------------
  // Loop list of environ
  //-------------------------------------
  for(p = environ; *p; ++p)
  {
    if(!strncmp(*p, name, len))
    {
      if(*(c = *p + len) == '=')
	    {
        //-------------------------------------
        // Element found -> return offset and value
        //-------------------------------------
	      *offset = p - environ;
    	  return (char *) (++c);
	    }
    }
  }
  
  //-------------------------------------
  // Element not found -> return NULL
  //-------------------------------------
  return NULL;
}


char *getenv(const char *name)
{
  int offset;
  return _findenv(name, &offset);
}

