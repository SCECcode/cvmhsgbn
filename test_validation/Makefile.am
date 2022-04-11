# Autoconf/automake file

bin_PROGRAMS = vx_cvmhsgbn_validate vx_cvmhsgbn_lite_validate 

# General compiler/linker flags
AM_CFLAGS = -DDYNAMIC_LIBRARY -Wall -O3 -std=c99 -D_LARGEFILE_SOURCE \
        -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 ${CFLAGS} -I../src
AM_LDFLAGS = ${LDFLAGS} -L../src -lcvmhsgbn -L../gctpc/source -lgctpc -lm

# Dist sources
vx_cvmhsgbn_validate_SOURCES = vx_cvmhsgbn_validate.c
vx_cvmhsgbn_lite_validate_SOURCES = vx_cvmhsgbn_lite_validate.c

.PHONY = run_validate

all: $(bin_PROGRAMS)

############################################
# Executables
############################################

vx_cvmhsgbn_lite_validate : vx_cvmhsgbn_lite_validate.o libcvmhsgbn.a
	$(CC) -o $@ $^ $(AM_LDFLAGS)

vx_cvmhsgbn_validate : vx_cvmhsgbn_validate.o libcvmhsgbn.a
	$(CC) -o $@ $^ $(AM_LDFLAGS)

############################################
# Special build targets
############################################

run_validate:
	./run_vxlite_validate.sh
	./run_ucvm_validate.sh

clean:
	rm -rf *~ *.o $(bin_PROGRAMS)

