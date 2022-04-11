#!/bin/bash
##
## validate with vx_lite_cvmhsgbn api
## 
rm -rf validate_lite_bad.out 
rm -rf validate_lite_foo.out

if [ ! -f ../data/cvmhsgbn/CVMHB-San-Gabriel-Basin.dat ]; then 
  echo "need to retrieve CVMHB-San-Gabriel-Basin.dat first!!!"
  exit 1
fi

if [ "$UCVM_INSTALL_PATH" ] ; then
  SCRIPT_DIR="$UCVM_INSTALL_PATH"/bin
  else
    SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
fi

source ${SCRIPT_DIR}/../conf/ucvm_env.sh

./vx_cvmhsgbn_lite_validate -m ../data/cvmhsgbn -f ../data/cvmhsgbn/CVMHB-San-Gabriel-Basin.dat

