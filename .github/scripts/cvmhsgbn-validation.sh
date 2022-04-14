#!/bin/bash

## run testsgbn validation

cd test_validation

make run_validate | tee result_validate.txt

p=`grep -c FAIL result_validate.txt` 
if [ $p != 0 ]; then
   echo "something wrong.."
   exit 1 
fi

exit 0 

