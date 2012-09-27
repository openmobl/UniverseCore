/* ctype.h: ISO/IEC 9899:1999  7.4  Character handling.

   Placed in the public domain by John Marshall.

   This file is part of prc-tools, but is not licensed in the same way
   as the majority of prc-tools.  The contents of this file, and also
   the implementations of the functions declared in this file, are in
   the public domain.

   One particular consequence of this is that you may use these functions
   in Palm OS executables without concern that doing so by itself causes
   the resulting executable to be covered by any particular license, such as
   the GNU General Public License that covers much of the rest of prc-tools.
   However this does not invalidate any other reasons why the executable
   might be covered by such a license (such as the use of other functions
   from other header files that are so-covered).  */

#ifndef _CTYPE_H
#define _CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

extern int isalnum (int _c);
extern int isalpha (int _c);
extern int isblank (int _c);
extern int iscntrl (int _c);
extern int isdigit (int _c);
extern int isgraph (int _c);
extern int islower (int _c);
extern int isprint (int _c);
extern int ispunct (int _c);
extern int isspace (int _c);
extern int isupper (int _c);
extern int isxdigit(int _c);

extern int tolower (int _c);
extern int toupper (int _c);

extern int isascii(int _c);
extern int toascii(int _c);

/* The magic numbers in the following are explained in the library source
   code, ctype.c.  The individual flags are unnamed here because there's
   nothing you can do with them and because they would be bunnies staring
   into the headlights of namespace collision.  */
extern const unsigned char _Ctype[], _Cconv[];

#define isalnum(c)  (_Ctype[(int) (c)] & 0x1c)
#define isalpha(c)  (_Ctype[(int) (c)] & 0x0c)
#define isblank(c)  (_Ctype[(int) (c)] & 0x40)
#define iscntrl(c)  (_Ctype[(int) (c)] & 0x80)
#define isdigit(c) ((_Ctype[(int) (c)] & 0x1c) == 0x10)
#define isgraph(c) ((_Ctype[(int) (c)] & 0x60) == 0x20)
#define islower(c) ((_Ctype[(int) (c)] & 0x0c) == 0x04)
#define isprint(c)  (_Ctype[(int) (c)] & 0x20)
#define ispunct(c)  (_Ctype[(int) (c)] == 0x20)
#define isspace(c)  (_Ctype[(int) (c)] & 0xc0)
#define isupper(c) ((_Ctype[(int) (c)] & 0x0c) == 0x08)
#define isxdigit(c) (_Ctype[(int) (c)] & 0x10)

#define tolower(c)  ({ int _c = (c); _c + _Cconv[_Ctype[_c] & 0x07]; })
#define toupper(c)  ({ int _c = (c); _c - _Cconv[_Ctype[_c] & 0x0b]; })

#define	isascii(c)	(!((c) & ~0177))
#define	toascii(c)	((c) & 0177)

#ifdef __cplusplus
}
#endif

#endif /* _CTYPE_H */
