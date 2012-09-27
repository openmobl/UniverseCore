#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H

#include <stdio.h>
#include <stdarg.h>
#include <M68kCalls.h>

extern unsigned short DefaultSDCard;
extern FILE *fstdout;
extern FILE *fstderr;

#ifdef __cplusplus
extern "C" {
#endif

extern void PrintConsole(const char *strText);

extern int setupLibC(void);
extern int destroyLibC(void);

#ifdef __cplusplus
}
#endif

#endif /* IMPLEMENTATION_H */
