include ../Makefile.conf

C68KFLAGS = 


INCLUDES  += -I./ -I./include/

OBJS      = implementation.o
LIBC_OBJS = ctype/ctype.o \
            errno/errno.o \
            math/tan.o math/tanh.o math/atanh.o math/atan2.o math/atan.o math/acosh.o math/asinh.o \
            math/cosh.o math/sinh.o math/acos.o math/asin.o math/ldexp.o math/significand.o \
            math/remainder.o math/pow.o math/sqrt.o math/sin.o math/rem_pio2.o math/scalb.o math/scalbn.o \
            math/floor.o math/copysign.o math/fabs.o math/ceil.o math/logb.o math/fabs.o math/exp.o \
            math/log10.o math/log.o math/fmod.o math/modf.o math/finite.o math/isnan.o math/rint.o \
            math/nextafter.o math/ilogb.o math/frexp.o math/cbrt.o math/expm1.o math/log1p.o \
            stdio/printf.o stdio/fopen.o stdio/fread.o stdio/fclose.o stdio/fwrite.o stdio/fseek.o \
            stdio/ferror.o stdio/ftell.o stdio/feof.o stdio/fgetc.o stdio/fgets.o stdio/fprintf.o \
            stdio/remove.o stdio/sscanf.o stdio/fflush.o \
            stdlib/calloc.o stdlib/realloc.o stdlib/malloc.o stdlib/environ.o stdlib/getenv.o \
            stdlib/setenv.o stdlib/putenv.o stdlib/qsort.o stdlib/div.o stdlib/ldiv.o stdlib/rand.o \
            stdlib/strtod.o stdlib/atoi.o stdlib/bsearch.o stdlib/abs.o stdlib/system.o stdlib/getcwd.o \
            stdlib/abort.o stdlib/atexit.o stdlib/exit.o stdlib/stat.o stdlib/select.o \
            string/bcmp.o string/bzero.o string/index.o string/rindex.o \
            string/strcasecmp.o string/strncasecmp.o string/strcspn.o string/strlwr.o string/strupr.o \
            string/strcpy.o string/strcat.o string/strchr.o string/strtok.o \
            string/strdup.o string/bcopy.o string/strerror.o \
            string/strpbrk.o string/strrchr.o string/strspn.o string/strstr.o string/memchr.o \
            string/memmove.o string/memcpy.o string/memset.o string/memcmp.o string/_memcpy.o \
            string/strcmp.o string/strncmp.o string/strlen.o \
            time/time.o time/localtime.o time/mktime.o time/asctime.o time/ctime.o time/gmtime.o \
            time/gettimeofday.o time/settimeofday.o \
            setjmp/setjmp.o setjmp/longjmp.o \
            network/socket.o network/inet.o network/netdb.o \
            unistd/read.o unistd/write.o unistd/close.o \


all: libarmc.a

libarmc.a: $(OBJS) $(LIBC_OBJS)
	$(AR) cur libarmc.a $(LIBC_OBJS) $(OBJS) 
	$(RANLIB) libarmc.a


#####################################################
## Compile armlet code
#####################################################

implementation.o: implementation.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c implementation.c -o implementation.o


###################### assert.h #####################

###################### ctype.h ######################
ctype/ctype.o: ctype/ctype.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c ctype/ctype.c -o ctype/ctype.o

###################### errno.h ######################
errno/errno.o: errno/errno.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c errno/errno.c -o errno/errno.o

###################### locale.h #####################

###################### math.h #######################
math/tan.o: math/tan.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/tan.c -o math/tan.o
math/tanh.o: math/tanh.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/tanh.c -o math/tanh.o
math/atanh.o: math/atanh.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/atanh.c -o math/atanh.o
math/atan.o: math/atan.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/atan.c -o math/atan.o
math/atan2.o: math/atan2.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/atan2.c -o math/atan2.o
math/acosh.o: math/acosh.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/acosh.c -o math/acosh.o
math/asinh.o: math/asinh.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/asinh.c -o math/asinh.o
math/cosh.o: math/cosh.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/cosh.c -o math/cosh.o
math/sinh.o: math/sinh.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/sinh.c -o math/sinh.o
math/acos.o: math/acos.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/acos.c -o math/acos.o
math/asin.o: math/asin.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/asin.c -o math/asin.o
math/ldexp.o: math/ldexp.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/ldexp.c -o math/ldexp.o
math/significand.o: math/significand.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/significand.c -o math/significand.o
math/remainder.o: math/remainder.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/remainder.c -o math/remainder.o
math/pow.o: math/pow.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/pow.c -o math/pow.o
math/sqrt.o: math/sqrt.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/sqrt.c -o math/sqrt.o
math/sin.o: math/sin.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/sin.c -o math/sin.o
math/rem_pio2.o: math/rem_pio2.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/rem_pio2.c -o math/rem_pio2.o
math/scalb.o: math/scalb.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/scalb.c -o math/scalb.o
math/scalbn.o: math/scalbn.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/scalbn.c -o math/scalbn.o
math/floor.o: math/floor.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/floor.c -o math/floor.o
math/copysign.o: math/copysign.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/copysign.c -o math/copysign.o
math/logb.o: math/logb.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/logb.c -o math/logb.o
math/fabs.o: math/fabs.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/fabs.c -o math/fabs.o
math/ceil.o: math/ceil.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/ceil.c -o math/ceil.o
math/exp.o: math/exp.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/exp.c -o math/exp.o
math/log10.o: math/log10.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/log10.c -o math/log10.o
math/log.o: math/log.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/log.c -o math/log.o
math/fmod.o: math/fmod.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/fmod.c -o math/fmod.o
math/modf.o: math/modf.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/modf.c -o math/modf.o
math/finite.o: math/finite.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/finite.c -o math/finite.o
math/isnan.o: math/isnan.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/isnan.c -o math/isnan.o
math/rint.o: math/rint.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/rint.c -o math/rint.o
math/nextafter.o: math/nextafter.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/nextafter.c -o math/nextafter.o
math/ilogb.o: math/ilogb.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/ilogb.c -o math/ilogb.o
math/frexp.o: math/frexp.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/frexp.c -o math/frexp.o
math/cbrt.o: math/cbrt.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/cbrt.c -o math/cbrt.o
math/expm1.o: math/expm1.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/expm1.c -o math/expm1.o
math/log1p.o: math/log1p.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c math/log1p.c -o math/log1p.o

###################### setjmp.h #####################
setjmp/setjmp.o: setjmp/setjmp.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c setjmp/setjmp.S -o setjmp/setjmp.o
setjmp/longjmp.o: setjmp/longjmp.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c setjmp/longjmp.S -o setjmp/longjmp.o

###################### signal.h #####################

###################### stdio.h ######################
stdio/fopen.o: stdio/fopen.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/fopen.c -o stdio/fopen.o
stdio/fread.o: stdio/fread.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/fread.c -o stdio/fread.o
stdio/fclose.o: stdio/fclose.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/fclose.c -o stdio/fclose.o
stdio/fwrite.o: stdio/fwrite.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/fwrite.c -o stdio/fwrite.o
stdio/fseek.o: stdio/fseek.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/fseek.c -o stdio/fseek.o
stdio/ftell.o: stdio/ftell.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/ftell.c -o stdio/ftell.o
stdio/feof.o: stdio/feof.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/feof.c -o stdio/feof.o
stdio/ferror.o: stdio/ferror.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/ferror.c -o stdio/ferror.o
stdio/fgetc.o: stdio/fgetc.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/fgetc.c -o stdio/fgetc.o
stdio/fgets.o: stdio/fgets.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/fgets.c -o stdio/fgets.o
stdio/fprintf.o: stdio/fprintf.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/fprintf.c -o stdio/fprintf.o
stdio/remove.o: stdio/remove.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/remove.c -o stdio/remove.o
stdio/printf.o: stdio/printf.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/printf.c -o stdio/printf.o
stdio/sscanf.o: stdio/sscanf.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/sscanf.c -o stdio/sscanf.o
stdio/fflush.o: stdio/fflush.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdio/fflush.c -o stdio/fflush.o

###################### stdlib.h #####################
stdlib/strtod.o: stdlib/strtod.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/strtod.c -o stdlib/strtod.o
stdlib/atoi.o: stdlib/atoi.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/atoi.c -o stdlib/atoi.o
stdlib/realloc.o: stdlib/realloc.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/realloc.c -o stdlib/realloc.o
stdlib/calloc.o: stdlib/calloc.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/calloc.c -o stdlib/calloc.o
stdlib/malloc.o: stdlib/malloc.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/malloc.c -o stdlib/malloc.o
stdlib/putenv.o: stdlib/putenv.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/putenv.c -o stdlib/putenv.o
stdlib/setenv.o: stdlib/setenv.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/setenv.c -o stdlib/setenv.o
stdlib/getenv.o: stdlib/getenv.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/getenv.c -o stdlib/getenv.o
stdlib/environ.o: stdlib/environ.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/environ.c -o stdlib/environ.o
stdlib/qsort.o: stdlib/qsort.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/qsort.c -o stdlib/qsort.o
stdlib/bsearch.o: stdlib/bsearch.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/bsearch.c -o stdlib/bsearch.o
stdlib/rand.o: stdlib/rand.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/rand.c -o stdlib/rand.o
stdlib/div.o: stdlib/div.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/div.c -o stdlib/div.o
stdlib/ldiv.o: stdlib/ldiv.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/ldiv.c -o stdlib/ldiv.o
stdlib/abs.o: stdlib/abs.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/abs.c -o stdlib/abs.o
stdlib/system.o: stdlib/system.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/system.c -o stdlib/system.o
stdlib/getcwd.o: stdlib/getcwd.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/getcwd.c -o stdlib/getcwd.o
stdlib/abort.o: stdlib/abort.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/abort.c -o stdlib/abort.o
stdlib/atexit.o: stdlib/atexit.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/atexit.c -o stdlib/atexit.o
stdlib/exit.o: stdlib/exit.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/exit.c -o stdlib/exit.o
stdlib/stat.o: stdlib/stat.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/stat.c -o stdlib/stat.o
stdlib/select.o: stdlib/select.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c stdlib/select.c -o stdlib/select.o

###################### string.h #####################
string/memmove.o: string/memmove.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/memmove.S -o string/memmove.o
string/memcpy.o: string/memcpy.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/memcpy.S -o string/memcpy.o
string/bcopy.o: string/bcopy.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/bcopy.S -o string/bcopy.o
string/_memcpy.o: string/_memcpy.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/_memcpy.S -o string/_memcpy.o
string/bcmp.o: string/bcmp.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/bcmp.c -o string/bcmp.o
string/memcmp.o: string/memcmp.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/memcmp.S -o string/memcmp.o
string/bzero.o: string/bzero.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/bzero.S -o string/bzero.o
string/memset.o: string/memset.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/memset.S -o string/memset.o
string/strcmp.o: string/strcmp.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strcmp.S -o string/strcmp.o
string/strncmp.o: string/strncmp.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strncmp.S -o string/strncmp.o
string/strlen.o: string/strlen.S
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strlen.S -o string/strlen.o
string/strcpy.o: string/strcpy.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strcpy.c -o string/strcpy.o
string/strcat.o: string/strcat.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strcat.c -o string/strcat.o
string/strchr.o: string/strchr.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strchr.c -o string/strchr.o
string/strtok.o: string/strtok.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strtok.c -o string/strtok.o
string/strcspn.o: string/strcspn.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strcspn.c -o string/strcspn.o
string/strpbrk.o: string/strpbrk.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strpbrk.c -o string/strpbrk.o
string/strrchr.o: string/strrchr.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strrchr.c -o string/strrchr.o
string/strspn.o: string/strspn.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strspn.c -o string/strspn.o
string/strstr.o: string/strstr.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strstr.c -o string/strstr.o
string/memchr.o: string/memchr.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/memchr.c -o string/memchr.o
string/index.o: string/index.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/index.c -o string/index.o
string/rindex.o: string/rindex.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/rindex.c -o string/rindex.o
string/strcasecmp.o: string/strcasecmp.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strcasecmp.c -o string/strcasecmp.o
string/strncasecmp.o: string/strncasecmp.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strncasecmp.c -o string/strncasecmp.o
string/strerror.o: string/strerror.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strerror.c -o string/strerror.o
string/strlwr.o: string/strlwr.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strlwr.c -o string/strlwr.o
string/strupr.o: string/strupr.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strupr.c -o string/strupr.o
string/strdup.o: string/strdup.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c string/strdup.c -o string/strdup.o

###################### time.h #######################
time/time.o: time/time.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c time/time.c -o time/time.o
time/localtime.o: time/localtime.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c time/localtime.c -o time/localtime.o
time/mktime.o: time/mktime.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c time/mktime.c -o time/mktime.o
time/asctime.o: time/asctime.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c time/asctime.c -o time/asctime.o
time/ctime.o: time/ctime.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c time/ctime.c -o time/ctime.o
time/gmtime.o: time/gmtime.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c time/gmtime.c -o time/gmtime.o
time/gettimeofday.o: time/gettimeofday.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c time/gettimeofday.c -o time/gettimeofday.o
time/settimeofday.o: time/settimeofday.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c time/settimeofday.c -o time/settimeofday.o

################### sys/socket.h ####################
network/socket.o: network/socket.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c network/socket.c -o network/socket.o

###################### netdb.h ######################
network/netdb.o: network/netdb.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c network/netdb.c -o network/netdb.o

#################### arpa/inet.h ####################
network/inet.o: network/inet.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c network/inet.c -o network/inet.o

###################### unistd.h #####################
unistd/read.o: unistd/read.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c unistd/read.c -o unistd/read.o
unistd/write.o: unistd/write.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c unistd/write.c -o unistd/write.o
unistd/close.o: unistd/close.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c unistd/close.c -o unistd/close.o
unistd/gethostname.o: unistd/gethostname.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c unistd/gethostname.c -o unistd/gethostname.o
unistd/sethostname.o: unistd/sethostname.c
	$(CXX) $(INCLUDES) $(CFLAGS) -c unistd/sethostname.c -o unistd/sethostname.o


clean:
	rm -f *.o *.a *.o ctype/*.o errno/*.o math/*.o setjmp/*.o stdio/*.o stdlib/*.o string/*.o time/*.o network/*.o unistd/*.o
	

