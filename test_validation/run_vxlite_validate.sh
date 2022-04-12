#!/bin/bash
##
## validate with cvmhsgbn vxlite api
## 
rm -rf validate_vxlite_bad.txt 
rm -rf validate_vxlite_good.txt

if [ ! -f ../data/cvmhsgbn/CVMHB-San-Gabriel-Basin.dat ]; then 
  echo "need to retrieve CVMHB-San-Gabriel-Basin.dat first!!!"
  exit 1
fi

if [ "$UCVM_INSTALL_PATH" ] ; then
  SCRIPT_DIR="$UCVM_INSTALL_PATH"/bin
  source ${SCRIPT_DIR}/../conf/ucvm_env.sh
  ./cvmhsgbn_vxlite_validate -m ../data/cvmhsgbn -f ../data/cvmhsgbn/CVMHB-San-Gabriel-Basin.dat
  else
    SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
    env DYLD_LIBRARY_PATH=${SCRIPT_DIR}/../src LD_LIBRARY_PATH=${SCRIPT_DIR}/../src ./cvmhsgbn_vxlite_validate -m ../data/cvmhsgbn -f ../data/cvmhsgbn/CVMHB-San-Gabriel-Basin.dat
fi
