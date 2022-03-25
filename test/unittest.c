#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "unittest_defs.h"
#include "test_vx_lite_cvmhsgbn_exec.h"
#include "test_vx_cvmhsgbn_exec.h"
#include "test_cvmhsgbn_exec.h"


int main (int argc, char *argv[])
{
  char *xmldir;

  if (argc == 2) {  
    xmldir = argv[1];
  } else {
    xmldir = NULL;
  }

  /* Run test suites */
  suite_cvmhsgbn_exec(xmldir);
  suite_vx_cvmhsgbn_exec(xmldir);
  suite_vx_lite_cvmhsgbn_exec(xmldir);

  return 0;
}
