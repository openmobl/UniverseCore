
OUTDIR = build/

CC = gcc

CFLAGS  = -g -O2 -Wall

INCLUDE = -I../../../include/ -I../../../platform/include/ -I../../../platform/linux/include/
INCLUDE += -I../../../css/ -I../../../dom/ -I../../../history/ -I../../../html/ -I../../../loader/
INCLUDE += -I../../../page/ -I../../../platform/ -I../../../platform/image-decoders/ -I../../../rendering/
INCLUDE += -I../../../platform/linux/ -I../../../loader/linux/
INCLUDE += -I../../../parser/ -I../../../parser/html/ -I../../../parser/sgml/ -I../../../parser/smil/
INCLUDE += -I../../../parser/wbxml/ -I../../../parser/wml/ -I../../../parser/xml/

CONFIGFLAGS =

LINKFLAGS = -L../../../ -lUniverseCore



all: Tester.c
	@echo "Compiling..." && \
	$(CC) $(CFLAGS) $(INCLUDE) Tester.c $(LINKFLAGS) -o tester

clean:
	@echo "Cleaning up..." && \
	cd $(OUTDIR) && \
	rm -f *.* && \
	cd ../



