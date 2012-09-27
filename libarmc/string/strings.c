#include <strings.h>
#include <string.h>

void bcopy(const void *s1, void *s2, size_t n)
{
    memcpy(s2, s1, n);
}

void bzero(void *s, size_t n)
{
    memset(s, n, 0);
}

int bcmp(const void *s1, const void *s2, size_t n)
{
    return memcmp(s1, s2, n);
}
