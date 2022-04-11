##
##  validate using UCVM api
##
rm -rf validate_ucvm_bad.out
rm -rf validate_ucvm_other.out

if [ ! -f ./validate_lite_foo.out  ]; then 
  echo "need to run run_vxlite_validate.sh first!!!"
  exit 1 
fi

if [ "$UCVM_INSTALL_PATH" ] ; then
  SCRIPT_DIR="$UCVM_INSTALL_PATH"/bin
  else
    SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
fi

source ${SCRIPT_DIR}/../conf/ucvm_env.sh

./vx_cvmhsgbn_validate -f validate_lite_foo.out

