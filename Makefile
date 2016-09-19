#
# Description:  Makefile for evio2hex
#   Unwrap data from a specific bank tag/num and print data to standard out
#

CODA	= /daqfs/coda/3.06
EVIO_INC = ${CODA}/Linux/include
EVIO_LIB = ${CODA}/Linux/lib

CC	= g++
CFLAGS	= -I. -I${EVIO_INC} \
		-L. -L${EVIO_LIB} \
		-levio -levioxx -lrt -lexpat
CFLAGS	+= -g -Wall
AR	= ar
RANLIB	= ranlib

PROGS	= evio2hex

all: echoarch $(PROGS)

%: %.cc
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(PROGS) *~

echoarch:
	echo "Make for $(ARCH)"
