# Autoconf/automake file

lib_LIBRARIES = libvxapi_cvmhsgbn.a libcvmhsgbn.a 
bin_PROGRAMS = vx_lite_cvmhsgbn vx_cvmhsgbn
include_HEADERS = vx_sub_cvmhsgbn.h cvmhsgbn.h
 
# General compiler/linker flags
AM_CFLAGS = -Wall -O3 -std=c99 -D_LARGEFILE_SOURCE \
            -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -fPIC
AM_LDFLAGS = -L../gctpc/source -lgctpc -lm

# Dist sources
libcvmhsgbn_a_SOURCES = vx_sub_cvmhsgbn.c vx_io.c 
vx_lite_cvmhsgbn_SOURCES = vx_lite_cvmhsgbn.c
vx_cvmhsgbn_SOURCES = cvmhsgbn.c vx_cvmhsgbn.c

TARGETS = vx_lite_cvmhsgbn vx_cvmhsgbn libvxapi_cvmhsgbn.a libcvmhsgbn.a libcvmhsgbn.so

all: $(TARGETS)

install:
	mkdir -p ${prefix}
	mkdir -p ${prefix}/lib
	mkdir -p ${prefix}/include
	cp libcvmhsgbn.so ${prefix}/lib
	cp libcvmhsgbn.a ${prefix}/lib
	cp cvmhsgbn.h ${prefix}/include

cvmhsgbn.h: ../cvmhbn/src/cvmhbn.h vx_sub_cvmhsgbn.h
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/src/cvmhbn.h > cvmhsgbn.h

cvmhsgbn.c: ../cvmhbn/src/cvmhbn.c cvmhsgbn.h
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/src/cvmhbn.c > cvmhsgbn.c

vx_lite_cvmhsgbn.c: ../cvmhbn/src/vx_lite_cvmhbn.c vx_sub_cvmhsgbn.h
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/src/vx_lite_cvmhbn.c > vx_lite_cvmhsgbn.c

vx_cvmhsgbn.c: ../cvmhbn/src/vx_cvmhbn.c cvmhsgbn.h
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/src/vx_cvmhbn.c > vx_cvmhsgbn.c

vx_sub_cvmhsgbn.c: ../cvmhbn/src/vx_sub_cvmhbn.c vx_sub_cvmhsgbn.h
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/src/vx_sub_cvmhbn.c > vx_sub_cvmhsgbn.c

vx_sub_cvmhsgbn.h: ../cvmhbn/src/vx_sub_cvmhbn.h 
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/src/vx_sub_cvmhbn.h > vx_sub_cvmhsgbn.h

libcvmhsgbn.a: vx_sub_cvmhsgbn.o vx_io.o utils.o cvmhsgbn_static.o 
	$(AR) rcs $@ $^

cvmhsgbn_static.o: cvmhsgbn.c
	$(CC) -o $@ -c $^ $(AM_CFLAGS)

libcvmhsgbn.so: vx_sub_cvmhsgbn.o vx_io.o utils.o cvmhsgbn.o
	$(CC) -shared $(AM_CFLAGS) -o libcvmhsgbn.so $^ $(AM_LDFLAGS)

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

clean:
	rm -rf $(TARGETS)
	rm -rf *.o 
	rm -rf cvmhsgbn.h cvmhsgbn.c vx_lite_cvmhsgbn.c vx_cvmhsgbn.c vx_sub_cvmhsgbn.c vx_sub_cvmhsgbn.h
