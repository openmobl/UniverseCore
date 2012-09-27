#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


#ifndef NULL
#define NULL 0
#endif

#ifndef EOF
#define EOF	(-1)
#endif

#define	SEEK_SET			vfsOriginBeginning 
#define	SEEK_CUR			vfsOriginCurrent  
#define	SEEK_END			vfsOriginEnd

#define stdin  ((FILE *)1)
#define stdout ((FILE *)2)
#define stderr ((FILE *)3)
#define _stdin  1
#define _stdout 2
#define _stderr 3

#ifndef _SIZE_T
#define	_SIZE_T
typedef unsigned long	size_t;
typedef long ssize_t;
#endif

typedef struct {
	unsigned long fileRef;
	unsigned short cacheSize, bufSize, bufPos;
	unsigned char *cache;
	unsigned short mode;
	int err;
} FILE;

typedef long fpos_t;


#define	FILENAME_MAX  256


//---------------------------------------------
// Size of cache buffer
//---------------------------------------------
#define	CACHE_SIZE	1024

//---------------------------------------------
// Modes of buffer
//---------------------------------------------
enum 
{
	MODE_BUFREAD = 1,
	MODE_BUFWRITE,
	MODE_BUFNONE
};

//---------------------------------------------
// Defines from PalmOS SDK
//---------------------------------------------
#define vfsOriginBeginning	0	// from the beginning (first data byte of file)
#define vfsOriginCurrent		1	// from the current position
#define vfsOriginEnd				2	// from the end of file (one position beyond last data byte, only negative offsets are legal)

#define vfsModeExclusive	(0x0001U)		// don't let anyone else open it
#define vfsModeRead				(0x0002U)		// open for read access
#define vfsModeWrite			(0x0004U | vfsModeExclusive)	// open for write access, implies exclusive
#define vfsModeCreate			(0x0008U)		// create the file if it doesn't already exist.
#define vfsModeTruncate		(0x0010U)		// Truncate file to 0 bytes after opening, removing all existing data.  
#define vfsModeReadWrite	(vfsModeWrite | vfsModeRead)	// open for read/write access


extern FILE *fopen(const char *filename, const char *type);
extern FILE	*freopen(const char *filename, const char *type, FILE *stream);
extern size_t fread(void *ptr, size_t size, size_t nitems, FILE *stream);
extern int fclose(FILE *stream);
extern size_t fwrite(const void *ptr, size_t size, size_t nitems, FILE *stream);
extern int fflush(FILE *stream);
extern int fseek(FILE *stream, long offset, int origin);
extern void	rewind(FILE *stream);
extern long ftell(FILE *stream);
extern int feof(FILE *stream);
extern int ferror(FILE *stream);
extern void	perror(const char *str);
extern void	clearerr(FILE *stream);
extern int fgetc(FILE *stream);
extern int getc(FILE *stream);
extern int getchar(void);
extern char *fgets(char *s, int n, FILE *stream);
extern int fprintf(FILE *stream, const char *formatStr, ...);
extern int vfprintf(FILE *stream, const char *formatStr, va_list va);
extern int fputc(int c, FILE *stream);
extern int putc(int c, FILE *stream);
extern int putchar(int c);
extern int fputs(const char *string, FILE *stream);
extern int remove(const char *filename);
extern int printf(const char *format, ...);
extern int vprintf(const char *format, va_list va);
extern int sprintf(char *buffer, const char *format, ...);
extern int snprintf(char *buffer, size_t size, const char *format, ...);
extern int vsprintf(char *buf, const char *fmt, va_list args);
extern int puts(unsigned char *str);
extern int sscanf(const char *buffer, const char *format, ...);
extern int vsscanf(const char *buf, const char *fmt, va_list arg_ptr);
extern int fgetpos(FILE *stream, fpos_t *ptr);
extern int fsetpos(FILE *stream, const fpos_t *ptr);

#define getc(s) fgetc(s)
#define getchar() getc(stdin)
#define putc(c,s) fputc(c,s)
#define	putchar(x) putc((x),stdout)


// Not yet implemented
extern int	rename(const char *, const char *);
extern FILE	*tmpfile(void);
extern char	*tmpnam(char *);
extern void	setbuf(FILE *, char *);
extern int	setvbuf(FILE *, char *, int, size_t);
extern int	fscanf(FILE *, const char *, ...);
extern int	scanf(const char *, ...);
extern char	*gets(char *);
extern int	ungetc(int, FILE *);
extern int	__filbuf(FILE *);
extern int	__flsbuf(int, FILE *);
extern FILE	*fdopen(int, const char *);
extern char	*ctermid(char *);
extern int	fileno(FILE *);


#ifdef __cplusplus
}
#endif


#endif /* __STDIO_H__ */
