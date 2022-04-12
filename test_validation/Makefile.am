# Autoconf/automake file

bin_PROGRAMS = cvmhsgbn_api_validate cvmhsgbn_vxlite_validate 

# General compiler/linker flags
AM_CFLAGS = -DDYNAMIC_LIBRARY -Wall -O3 -std=c99 -D_LARGEFILE_SOURCE \
        -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 ${CFLAGS} -I../src
AM_LDFLAGS = ${LDFLAGS} -L../src -lcvmhsgbn -L../gctpc/source -lgctpc -lm

# Dist sources
cvmhsgbn_api_validate_SOURCES = cvmhsgbn_api_validate.c
cvmhsgbn_vxlite_validate_SOURCES = cvmhsgbn_vxlite_validate.c

.PHONY = run_validate

all: $(bin_PROGRAMS)

############################################
# Executables
############################################

cvmhsgbn_vxlite_validate : cvmhsgbn_vxlite_validate.o 
	$(CC) -o $@ $^ $(AM_LDFLAGS)

cvmhsgbn_api_validate : cvmhsgbn_api_validate.o 
	$(CC) -o $@ $^ $(AM_LDFLAGS)

############################################
# Special build targets
############################################

run_validate:
	./run_vxlite_validate.sh
	./run_api_validate.sh

clean:
	rm -rf *~ *.o $(bin_PROGRAMS) *.txt

