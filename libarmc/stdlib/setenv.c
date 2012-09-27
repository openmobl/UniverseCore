#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

extern char **environ;
extern char *_findenv(const char *name, int *offset);

int _env_allocated;

int setenv(const char *name, const char *value, int rewrite)
{
  char *c;
  int len_value, offset;
  
  //-------------------------------------
  // Check pointers
  //-------------------------------------
  if(!environ || !name || !value)
    return 0;

  //-------------------------------------
  // Prepare value
  //-------------------------------------
  if (*value == '=')
    ++value;
  len_value = strlen(value);

  if((c = _findenv(name, &offset)))
  {
    //-------------------------------------
    // Element already exists
    //-------------------------------------
    if(!rewrite)
  	  return 0;
    if(strlen(c) >= len_value)
  	{
      //-------------------------------------
      // Old value larger -> copy new to old
      //-------------------------------------
     while((*c++ = *value++) != 0);
     return 0;
	  }
  }
  else
  {
    //-------------------------------------
    // Create new element
    //-------------------------------------
    int cnt;
    char **p;

    //-------------------------------------
    // Count number of elements
    //-------------------------------------
    for(p = environ, cnt = 0; *p; ++p, ++cnt);

    //-------------------------------------
    // Get memory for new element
    //-------------------------------------
    if (_env_allocated)
	  {
	    environ = (char **) realloc((char *) environ, (size_t) (sizeof(char *) * (cnt + 2)));
  	  if (!environ)
	      return -1;
  	}
    else
	  {
	    _env_allocated = 1;
  	  p = (char **) malloc((size_t) (sizeof(char *) * (cnt + 2)));
  	  if(!p)
	      return -1;
  	  bcopy((char *) environ, (char *) p, cnt * sizeof(char *));
	    environ = p;
	  }
    environ[cnt + 1] = NULL;
    offset = cnt;
    environ[offset] = NULL;
  }

  //-------------------------------------
  // Get len of name
  //-------------------------------------
  for(c = (char *) name; *c && *c != '='; ++c);
  
  //-------------------------------------
  // Get memory for name and value
  //-------------------------------------
  if(environ[offset])
    free(environ[offset]); // Free memory of previous value of element
  if (!(environ[offset] =	malloc((size_t) ((int) (c - name) + len_value + 2))))
    return -1;

  //-------------------------------------
  // Copy name
  //-------------------------------------
  for(c = environ[offset]; (*c = *name++) && *c != '='; ++c);

  //-------------------------------------
  // Copy value
  //-------------------------------------
  for(*c++ = '='; (*c++ = *value++) != 0;);

  return 0;
}


void unsetenv(const char *name)
{
  char **p;
  int offset;

  while(_findenv(name, &offset))
  {
    if(environ[offset])
      free(environ[offset]); // Free memory of name and value
    for(p = &environ[offset];; ++p)
    {
      if(!(*p = *(p + 1)))
	      break;
	  }
	}
}


void _clearenv(void)
{
  char **p;

  if (_env_allocated)
  {
    for(p = environ; *p; ++p)
      free(*p);
    free(environ);
  }
}



