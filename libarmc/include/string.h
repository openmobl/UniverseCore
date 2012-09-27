#ifndef _STRING_H
#define _STRING_H

#define __need_size_t
#define __need_NULL
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


extern void *memcpy  (void *_dest, const void *_src, size_t _n);
extern void *memmove (void *_dest, const void *_src, size_t _n);
extern char *strcpy  (char *_dest, const char *_src);
extern char *strncpy (char *_dest, const char *_src, size_t _n);
extern char *strcat  (char *_dest, const char *_src);
extern char *strncat (char *_dest, const char *_src, size_t _n);
extern int memcmp  (const void *_s1, const void *_s2, size_t _n);
extern int strcmp  (const char *_s1, const char *_s2);
extern int strncmp (const char *_s1, const char *_s2, size_t _n);
extern void *memset  (void *_s, int _c, size_t _n);
extern void *memchr   (const void *_s, int _c, size_t _n);
extern char *strchr   (const char *_s, int _c);
extern size_t strcspn (const char *_s, const char *_reject);
extern char *strpbrk  (const char *_s, const char *_accept);
extern char *strrchr  (const char *_s, int _c);
extern size_t strspn  (const char *_s, const char *_accept);
extern char *strstr   (const char *_s, const char *_key);
extern char *strtok   (char *_s, const char *_delim);
extern size_t strlen (const char *_s);
extern char *index(const char *s, int c);
extern char *rindex(const char *s, int c);
extern int strcasecmp(const char *s1, const char *s2);
extern int strncasecmp(const char *s1, const char *s2, size_t n);
extern char* strerror(int errnum);
extern char* strlwr(char *a);
extern char* strupr(char *a);
extern char *strdup(const char *str);


#ifdef __cplusplus
}
#endif

#endif
