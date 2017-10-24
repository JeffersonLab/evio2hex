#
# Description:  Makefile for evio2hex
#   Unwrap data from a specific bank tag/num and print data to standard out
#

# ARCH	= Linux-x86_64
ARCH	= Linux-i686

CODA	= /daqfs/coda/3.06
EVIO_INC = ${CODA}/$(ARCH)/include
EVIO_LIB = ${CODA}/$(ARCH)/lib

CC	= g++
CFLAGS	= -I. -I${EVIO_INC} \
		-L. -L${EVIO_LIB} \
		-levio -levioxx -lrt -lexpat
CFLAGS	+= -g -Wall

PROGS	= evio2hex

all: echoarch $(PROGS)

%: %.cc
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(PROGS) *~

echoarch:
	echo "Make for $(ARCH)"
