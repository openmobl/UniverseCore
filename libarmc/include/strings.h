#ifndef _STRINGS_H
#define _STRINGS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void bcopy(const char *b1, char *b2, size_t length);
extern void bzero(char *b, size_t length);
extern int bcmp(const char *m1, const char *m2, size_t n);


#ifdef __cplusplus
}
#endif

#endif
