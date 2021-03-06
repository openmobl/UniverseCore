/*
 * This module is based on vsscanf.c and input.c from emx 0.8f library
 * source which is Copyright (c) 1990-1992 by Eberhard Mattes.
 * Eberhard Mattes has kindly agreed to allow this module to be incorporated
 * into PDCurses.
 */

/**********************************************************************

 	Scan a series of input fields. Each field is formatted according to
 	a supplied format string and the formatted input is stored in the
 	variable number of addresses passed.

************************************************************************/
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>


int sscanf(const char *buffer, const char *format, ...)
{
	va_list args;
	int len;

	va_start(args, format);
	len = vsscanf(buffer, format, args);
	va_end(args);
	return len;
}


#define WHITE(x) ((x) == ' ' || (x) == '\t' || (x) == '\n')

#define NEXT(x) \
  do { \
    x = *buf++; \
    if (x == '\0') \
      return (count == 0 ? EOF : count); \
    ++chars; \
  } while (0)

#define UNGETC(x) \
  do { \
    --buf; /* *buf = x; */ --chars; \
  } while (0)


int vsscanf(const char *buf, const char *fmt, va_list arg_ptr)
{
	int count, chars, c, width, radix, d, i;
	int *int_ptr;
	long *long_ptr;
	short *short_ptr;
	char *char_ptr;
	unsigned char f;
	char neg, assign, ok, size;
	unsigned long n;
	char map[256], end;
	double dx, dd, *dbl_ptr;
	float *flt_ptr;
	int exp;
	char eneg;
  
  count = 0; 
  chars = 0; 
  c = 0;
  while ((f = *fmt) != 0)
  {
    if(WHITE (f))
    {
      do
      {
        ++fmt; 
        f = *fmt;
      } while (WHITE (f));
      do
      {
        c = *buf++;
        if (c == '\0')
        {
          if (f == 0 || count != 0)
            return (count);
          else
            return (EOF);
        }
        else
          ++chars;
      } while (WHITE (c));
      UNGETC (c);
    }
    else if (f != '%')
    {
      NEXT (c);
      if (c != f)
        return (count);
      ++fmt;
    }
    else
    {
      assign = 1; 
      width = INT_MAX;
      char_ptr = NULL;
      ++fmt;
      if (*fmt == '*')
      {
        assign = 0;
        ++fmt;
      }
      if (isdigit (*fmt))
      {
        width = 0;
        while (isdigit (*fmt))
          width = width * 10 + (*fmt++ - '0');
        if (width == 0) 
          width = INT_MAX;
      }
      size = 0;
      if (*fmt == 'h' || *fmt == 'l')
        size = *fmt++;
      f = *fmt;
      switch (f)
      {
        case 'c':
          if (width == INT_MAX)
            width = 1;
          if (assign)
            char_ptr = va_arg (arg_ptr, char *);
          while (width > 0)
          {
            --width;
            NEXT (c);
            if (assign)
            {
              *char_ptr++ = (char)c;
              ++count;
            }
          }
          break;

        case '[':
          (void)memset (map, 0, 256);
          end = 0;
          ++fmt;
          if (*fmt == '^')
          {
            ++fmt; 
            end = 1;
          }
          i = 0;
          for (;;)
          {
            f = (unsigned char)*fmt;
            switch (f)
            {
              case 0:
                --fmt;       // avoid skipping past 0
                NEXT (c);
                goto string;
              case ']':
                if (i > 0)
                {
                  NEXT (c);
                  goto string;
                }
                // no break
              default:
                if (fmt[1] == '-' && fmt[2] != 0 && f < (unsigned char)fmt[2])
                {
                  (void)memset (map+f, 1, (unsigned char)fmt[2]-f);
                  fmt += 2;
                }
                else
                  map[f] = 1;
                break;
            }
            ++fmt; ++i;
          }

        case 's':
          (void)memset (map, 0, 256);
          map[' '] = 1;
          map['\n'] = 1;
          map['\r'] = 1;
          map['\t'] = 1;
          end = 1;
          do
          {
            NEXT (c);
          } while (WHITE (c));
string:
          if (assign)
            char_ptr = va_arg (arg_ptr, char *);
          while (width > 0 && map[(unsigned char)c] != end)
          {
            --width;
            if (assign)
              *char_ptr++ = (char)c;
            c = *buf++;
            if (c == '\0')
              break;
            else
              ++chars;
          }
          if (assign)
          {
            *char_ptr = 0;
            ++count;
          }
          if (c == '\0')
            return (count);
          else
            UNGETC (c);
          break;

        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
          neg = ok = 0; dx = 0.0;
          do
          {
            NEXT (c);
          } while (WHITE (c));
          if (c == '+')
          {
            NEXT (c); 
            --width;
          }
          else if (c == '-')
          {
            neg = 1; 
            NEXT (c); 
            --width;
          }
          while (width > 0 && isdigit (c))
          {
            --width;
            dx = dx * 10.0 + (double)(c - '0');
            ok = 1;
            c = *buf++;
            if (c == '\0')
              break;
            else
              ++chars;
          }
          if (width > 0 && c == '.')
          {
            --width;
            dd = 10.0; 
            NEXT (c);
            while (width > 0 && isdigit (c))
            {
              --width;
              dx += (double)(c - '0') / dd;
              dd *= 10.0;
              ok = 1;
              c = *buf++;
              if (c == '\0')
                break;
              else
                ++chars;
            }
          }
          if (!ok)
            return (count);
          if (width > 0 && (c == 'e' || c == 'E'))
          {
            eneg = 0; 
            exp = 0; 
            NEXT (c); 
            --width;
            if (width > 0 && c == '+')
            {
              NEXT (c); 
              --width;
            }
            else if (width > 0 && c == '-')
            {
              eneg = 1; 
              NEXT (c); 
              --width;
            }
            if (!(width > 0 && isdigit (c)))
            {
              UNGETC (c);
              return (count);
            }
            while (width > 0 && isdigit (c))
            {
              --width;
              exp = exp * 10 + (c - '0');
              c = *buf++;
              if (c == '\0')
                break;
              else
                ++chars;
            }
            if (eneg) 
              exp = -exp;
            while (exp > 0)
            {
              dx *= 10.0;
              --exp;
            }
            while (exp < 0)
            {
              dx /= 10.0;
              ++exp;
            }
          }
          if (assign)
          {
            if (neg) 
              dx = -dx;
            if (size == 'l')
            {
              dbl_ptr = va_arg (arg_ptr, double *);
              *dbl_ptr = dx;
            }
            else
            {
              flt_ptr = va_arg (arg_ptr, float *);
              *flt_ptr = (float)dx;
            }
            ++count;
          }
          if (c == '\0')
            return (count);
          else
            UNGETC (c);
          break;

        case 'i':
          neg = 0; 
          radix = 10;
          do
          {
            NEXT (c);
          } while (WHITE (c));
          if (!(width > 0 && c == '0'))
            goto scan_complete_number;
          NEXT (c); 
          --width;
          if (width > 0 && (c == 'x' || c == 'X'))
          {
            NEXT (c); 
            radix = 16; 
            --width;
          }
          else if (width > 0 && (c >= '0' && c <= '7'))
            radix = 8;
          goto scan_unsigned_number;

        case 'd':
        case 'u':
        case 'o':
        case 'x':
        case 'X':
          do
          {
            NEXT (c);
          } while (WHITE (c));
          switch (f)
          {
            case 'o':           
              radix = 8; 
              break;
            case 'x': 
            case 'X': 
              radix = 16; 
              break;
            default:            
              radix = 10; 
              break;
          }
scan_complete_number:
          neg = 0;
          if (width > 0 && c == '+')
          {
            NEXT (c); 
            --width;
          }
          else if (width > 0 && c == '-' && radix == 10)
          {
            neg = 1; 
            NEXT (c); 
            --width;
          }
scan_unsigned_number:
          n = 0; 
          ok = 0;
          while (width > 0)
          {
            --width;
            if (isdigit (c))
              d = c - '0';
            else if (isupper (c))
              d = c - 'A' + 10;
            else if (islower (c))
              d = c - 'a' + 10;
            else
              break;
            if (d < 0 || d >= radix)
              break;
            ok = 1;
            n = n * radix + d;
            c = *buf++;
            if (c == '\0')
              break;
            else
              ++chars;
          }
          if (!ok)
            return (count);
          if (assign)
          {
            if (neg) 
              n = -n;
            switch(size)
            {
              case 'h':
                short_ptr = va_arg (arg_ptr, short *);
                *short_ptr = (short)n;
                break;
              case 'l':
                long_ptr = va_arg (arg_ptr, long *);
                *long_ptr = (long)n;
                break;
              default:
                int_ptr = va_arg (arg_ptr, int *);
                *int_ptr = (int)n;
            }
            ++count;
          }
          if (c == '\0')
            return (count);
          else
            UNGETC (c);
          break;

        case 'n':
          if (assign)
          {
            int_ptr = va_arg (arg_ptr, int *);
            *int_ptr = chars;
            ++count;
          }
          break;

        default:
          if (f == 0)                 // % at end of string
            return (count);
          NEXT (c);
          if (c != f)
            return (count);
          break;
      }
      ++fmt;
    }
  }

  return (count);
}

