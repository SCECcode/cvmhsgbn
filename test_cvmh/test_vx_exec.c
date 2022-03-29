#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include "vx_sub.h"
#include "unittest_defs.h"
#include "test_helper.h"
#include "test_vx_exec.h"


int test_vx_points()
{
  char infile[128];
  char outfile[128];
  char reffile[128];
  char currentdir[128];

  printf("Test: vx executable\n");

  /* Save current directory */
  getcwd(currentdir, 128);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test.in");
  sprintf(outfile, "%s/%s", currentdir, "test-8-point-vx-extract-elev.out");
  sprintf(reffile, "%s/%s", currentdir, 
	  "./ref/test-8-point-vx-extract-elev.ref");

  if (test_assert_int(save_test_points(infile), 0) != 0) {
    return _failure("Fail to create test points");
  }

  if (test_assert_int(runVX(BIN_DIR, MODEL_DIR, infile, outfile), 0) != 0) {
    return _failure("vx call failed");
  }

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    return _failure("Mismatched result");
  }

  unlink(infile);
  unlink(outfile);

  return _success();
}


int suite_vx_exec(const char *xmldir)
{
  suite_t suite;
  char logfile[256];
  FILE *lf = NULL;

  /* Setup test suite */
  strcpy(suite.suite_name, "suite_vx_exec");
  suite.num_tests = 1;
  suite.tests = malloc(suite.num_tests * sizeof(test_t));
  if (suite.tests == NULL) {
    return _failure("Failed to alloc test structure");
  }
  test_get_time(&suite.exec_time);

  /* Setup test cases */
  strcpy(suite.tests[0].test_name, "test_vx_points");
  suite.tests[0].test_func = &test_vx_points;
  suite.tests[0].elapsed_time = 0.0;

  if (test_run_suite(&suite) != 0) {
    return _failure("Failed to execute tests");
  }

  if (xmldir != NULL) {
    sprintf(logfile, "%s/%s.xml", xmldir, suite.suite_name);
    lf = init_log(logfile);
    if (lf == NULL) {
      return _failure("Failed to initialize logfile");
    }
    
    if (write_log(lf, &suite) != 0) {
      return _failure("Failed to write test log");
    }
    
    close_log(lf);
  }

  free(suite.tests);

  return 0;
}
