#ifndef libc_h
#define libc_h

#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int setupLibC(void);
extern int destroyLibC(void);

extern int main(int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif /* libc_h */
