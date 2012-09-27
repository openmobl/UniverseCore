
OUTDIR = build/

CC = m68k-palmos-gcc

CFLAGS  = -palmos5r3 -g -O2 -Wall

INCLUDE = -I../../../include/ -I../../../platform/include/ -I../../../platform/palmos/include/
INCLUDE += -I../../../css/ -I../../../dom/ -I../../../history/ -I../../../html/ -I../../../loader/
INCLUDE += -I../../../page/ -I../../../platform/ -I../../../platform/image-decoders/ -I../../../rendering/
INCLUDE += -I../../../platform/palmos/ -I../../../loader/palmos/
INCLUDE += -I../../../parser/ -I../../../parser/html/ -I../../../parser/sgml/ -I../../../parser/smil/
INCLUDE += -I../../../parser/wbxml/ -I../../../parser/wml/ -I../../../parser/xml/

CONFIGFLAGS = -DPALMOS

LINKFLAGS = -L../../../ -lUniverseCore -lPalmOSGlue



all: $(OUTDIR)Tester_linked.o $(OUTDIR)Tester.ro
	@echo "Building Tester..." && \
	build-prc Tester.def $(OUTDIR)Tester_linked.o $(OUTDIR)Tester.ro -o WBXMLTester.prc

$(OUTDIR)Tester.ro: Tester.rcp TesterRes.h
	@echo "Building resources..." && \
	pilrc -ro Tester.rcp $(OUTDIR)Tester.ro

$(OUTDIR)Tester.o: Tester.c
	@echo "Compiling..." && \
	$(CC) $(CFLAGS) $(INCLUDE) -c Tester.c -o $(OUTDIR)Tester.o

$(OUTDIR)Tester_linked.o: $(OUTDIR)Tester.o
	@echo "Linking Tester..." && \
	$(CC) $(CFLAGS) $(OUTDIR)Tester.o -o $(OUTDIR)Tester_linked.o $(LINKFLAGS)

clean:
	@echo "Cleaning up..." && \
	cd $(OUTDIR) && \
	rm -f *.* && \
	cd ../



