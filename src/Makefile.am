# Autoconf/automake file

lib_LIBRARIES = libvxapi.a libvxapi_cvmhsgbn.a libcvmhsgbn.a 
bin_PROGRAMS = vx vx_lite vx_slice vx_lite_cvmhsgbn vx_cvmhsgbn run_vx.sh run_vx_lite.sh
include_HEADERS = vx_sub.h

# General compiler/linker flags
AM_CFLAGS = ${CFLAGS} -Wall -O3 -std=c99 -D_LARGEFILE_SOURCE \
            -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -fPIC
AM_LDFLAGS = ${LDFLAGS} -L../gctpc/source -lgctpc

# Dist sources
libcvmhsgbn_a_SOURCES = vx_sub_cvmhsgbn.c vx_io.c 
libvxapi_a_SOURCES = vx_sub.c scec1d.c vs30_gtl.c vx_io.c utils.c 
vx_lite_cvmhsgbn_SOURCES = vx_lite_cvmhsgbn.c
vx_cvmhsgbn_SOURCES = vx_cvmhsgbn.c
vx_slice_SOURCES = vx_slice.c
vx_lite_SOURCES = vx_lite.c
run_vx_sh_SOURCES = run_vx.sh
run_vx_lite_sh_SOURCES = run_vx_lite.sh

TARGETS = vx_lite_cvmhsgbn vx_cvmhsgbn libvxapi_cvmhsgbn.a libvxapi.a libcvmhsgbn.a libcvmhsgbn.so

all: $(TARGETS)

install:
	mkdir -p ${prefix}
	mkdir -p ${prefix}/lib
	mkdir -p ${prefix}/include
	cp libcvmhsgbn.so ${prefix}/lib
	cp libcvmhsgbn.a ${prefix}/lib
	cp cvmhsgbn.h ${prefix}/include

libcvmhsgbn.a: vx_sub_cvmhsgbn.o vx_io.o utils.o cvmhsgbn_static.o 
	$(AR) rcs $@ $^

cvmhsgbn_static.o: cvmhsgbn.c
	$(CC) -o $@ -c $^ $(AM_CFLAGS)

libcvmhsgbn.so: vx_sub_cvmhsgbn.o vx_io.o utils.o cvmhsgbn.o
	$(CC) -shared $(AM_CFLAGS) -o libcvmhsgbn.so $^ $(AM_LDFLAGS)

libvxapi.a: vx_sub.o scec1d.o vs30_gtl.o vx_io.o utils.o *.h
	$(AR) rcs $@ $^

libvxapi_cvmhsgbn.a: vx_sub_cvmhsgbn.o vx_io.o utils.o *.h
	$(AR) rcs $@ $^

cvmhsgbn.o: cvmhsgbn.c
	$(CC) -fPIC -DDYNAMIC_LIBRARY -o $@ -c $^ $(AM_CFLAGS)

vx_lite_cvmhsgbn.o : vx_lite_cvmhsgbn.c
	$(CC) -o $@ -c $^ $(AM_CFLAGS)

vx_lite_cvmhsgbn : vx_lite_cvmhsgbn.o libvxapi_cvmhsgbn.a
	$(CC) -o $@ $^ $(AM_LDFLAGS)

vx_cvmhsgbn.o : vx_cvmhsgbn.c
	$(CC) -o $@ -c $^ $(AM_CFLAGS)

vx_cvmhsgbn : vx_cvmhsgbn.o libcvmhsgbn.a
	$(CC) -o $@ $^ $(AM_LDFLAGS)

vx: vx.o
	$(CC) -o $@ $^ $(AM_LDFLAGS)

vx_lite: vx_lite.o libvxapi.a
	$(CC) -o $@ $^ $(AM_LDFLAGS)

vx_slice: vx_slice.o libvxapi.a
	$(CC) -o $@ $^ $(AM_LDFLAGS)

run_vx.sh:
run_vx_lite.sh:


clean:
	rm -rf $(TARGETS)
	rm -rf *.o 

