# Autoconf/automake file

bin_PROGRAMS = cvmhsgbn_api_validate cvmhsgbn_vxlite_validate

# General compiler/linker flags
AM_CFLAGS = -DDYNAMIC_LIBRARY -Wall -O3 -std=c99 -D_LARGEFILE_SOURCE \
        -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -I../src
AM_LDFLAGS = -L../src -lcvmhsgbn -L../gctpc/source -lgctpc -lm -ldl

# Dist sources
cvmhsgbn_api_validate_SOURCES = cvmhsgbn_api_validate.c
cvmhsgbn_vxlite_validate_SOURCES = cvmhsgbn_vxlite_validate.c

.PHONY = run_validate run_validate_with_ucvm

all: $(bin_PROGRAMS)

############################################
# Executables
############################################
cvmhsgbn_vxlite_validate.c : ../cvmhbn/test_validation/cvmhbn_vxlite_validate.c
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_vxlite_validate.c > cvmhsgbn_vxlite_validate.c

cvmhsgbn_api_validate.c : ../cvmhbn/test_validation/cvmhbn_api_validate.c
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_api_validate.c > cvmhsgbn_api_validate.c

cvmhsgbn_vxlite_validate : cvmhsgbn_vxlite_validate.o 
	$(CC) -o $@ $^ $(AM_LDFLAGS)

cvmhsgbn_api_validate : cvmhsgbn_api_validate.o 
	$(CC) -o $@ $^ $(AM_LDFLAGS)

### only run after ucvm is built and installed
if UCVM
cvmhsgbn_ucvm_validate.c : ../cvmhbn/test_validation/cvmhbn_ucvm_validate.c
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_ucvm_validate.c > cvmhsgbn_ucvm_validate.c
	$(call getLDFLAGS)

cvmhsgbn_ucvm_rerun.c : ../cvmhbn/test_validation/cvmhbn_ucvm_rerun.c
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_ucvm_rerun.c > cvmhsgbn_ucvm_rerun.c
	$(call getLDFLAGS)

cvmhsgbn_ucvm_retry.c : ../cvmhbn/test_validation/cvmhbn_ucvm_retry.c
	sed -f ../cvmhbn/setup/cvmhsgbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_ucvm_retry.c > cvmhsgbn_ucvm_retry.c
	$(call getLDFLAGS)

cvmhsgbn_ucvm_validate.o : cvmhsgbn_ucvm_validate.c
	$(CC) -c -o $@ $^ $(UCVM_CFLAGS) $(AM_CFLAGS) $(CFLAGS)

cvmhsgbn_ucvm_validate : cvmhsgbn_ucvm_validate.o 
	$(CC) -o $@ $^ $(UCVM_LDFLAGS) $(AM_LDFLAGS)

cvmhsgbn_ucvm_rerun.o : cvmhsgbn_ucvm_rerun.c
	$(CC) -c -o $@ $^ $(UCVM_CFLAGS) $(AM_CFLAGS) $(CFLAGS)

cvmhsgbn_ucvm_rerun : cvmhsgbn_ucvm_rerun.o 
	$(CC) -o $@ $^ $(UCVM_LDFLAGS) $(AM_LDFLAGS)

cvmhsgbn_ucvm_retry.o : cvmhsgbn_ucvm_retry.c
	$(CC) -c -o $@ $^ $(UCVM_CFLAGS) $(AM_CFLAGS) $(CFLAGS)

cvmhsgbn_ucvm_retry : cvmhsgbn_ucvm_retry.o 
	$(CC) -o $@ $^ $(UCVM_LDFLAGS) $(AM_LDFLAGS)

define getLDFLAGS =
    if [ -f '$(UCVM_INSTALL_PATH)/model/cvmh/include/vx_sub.h' ]; then
        CVMH_ldflags = -L$(UCVM_INSTALL_PATH)/model/cvmh/lib -lvxapi -L$(UCVM_INSTALL_PATH)/model/cvmh/lib -lgeo
    fi
    if [ -f '$(UCVM_INSTALL_PATH)/model/cencal/include/cencalvm/query/cvmerror.h' ]; then
        CENCAL_ldflags = '-L$UCVM_INSTALL_PATH/model/cencal/lib -lcencalvm -lstdc++'
    fi
    UCVM_LDFLAGS = -L$(UCVM_INSTALL_PATH)/lib -lucvm $(CVMH_ldflags) $(CENCAL_ldflags) -dynamic  -L$(UCVM_INSTALL_PATH)/lib/euclid3/lib -letree -L$(UCVM_INSTALL_PATH)/lib/proj-5/lib -lproj -lpthread -L$(UCVM_INSTALL_PATH)/lib/fftw/lib -lfftw3 -lm -ldl
    UCVM_CFLAGS= -I$(UCVM_INSTALL_PATH)/include
endef

endif

############################################
# Special build targets
############################################

run_validate: cvmhsgbn_api_validate cvmhsgbn_vxlite_validate
	./run_vxlite_validate.sh
	./run_api_validate.sh

run_validate_with_ucvm: cvmhsgbn_ucvm_validate cvmhsgbn_ucvm_rerun cvmhsgbn_ucvm_retry
	./run_ucvm_validate.sh

clean:
	rm -rf *~ *.o $(bin_PROGRAMS) cvmhsgbn_ucvm_validate cvmhsgbn_ucvm_rerun cvmhsgbn_ucvm_retry *.txt
	rm -rf cvmhsgbn_ucvm_rerun.c cvmhsgbn_ucvm_validate.c cvmhsgbn_vxlite_validate.c cvmhsgbn_api_validate.c cvmhsgbn_ucvm_retry.c


