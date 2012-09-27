#include <string.h>

int bcmp(const char *m1, const char *m2, size_t n)
{
  return memcmp(m1, m2, n);
}
