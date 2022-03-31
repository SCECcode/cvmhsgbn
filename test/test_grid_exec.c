/**
   test_grid_exec.c

   invokes src/run_vx_cvmhsgbn.sh/vx_cvmhsgbn
   invokes src/run_vx_lite_cvmhsgbn.sh/vx_lite_cvmhsgbn
**/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include "unittest_defs.h"
#include "test_helper.h"
#include "test_grid_exec.h"


// test with elevation offset by cvmh's digital elevation
int test_cvmhsgbn_grid_elev()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: model with large grid in elevation mode\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-grid-elev.in");
  sprintf(outfile, "%s/%s", currentdir, "test-grid-extract-cvmhsgbn-elev.out");
  sprintf(reffile, "%s/%s", currentdir, "./ref/test-grid-extract-cvmhsgbn-elev.ref");

  if (test_assert_int(runCVMHSGBN(BIN_DIR, MODEL_DIR,infile, outfile,
				MODE_ELEVATION), 0) != 0) {
    return _failure("cvmhsgbn failure");
  }

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    printf("unmatched result\n");
    printf("%s\n",outfile);
    printf("%s\n",reffile);
    return _failure("diff file");
  }

  unlink(outfile);

  return _success();
}

int test_cvmhsgbn_grid_depth()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: model with large grid in depth mode\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-grid-depth.in");
  sprintf(outfile, "%s/%s", currentdir, "test-grid-extract-cvmhsgbn-depth.out");
  sprintf(reffile, "%s/%s", currentdir, "./ref/test-grid-extract-cvmhsgbn-depth.ref");

  if (test_assert_int(runCVMHSGBN(BIN_DIR, MODEL_DIR,infile, outfile,
				MODE_DEPTH), 0) != 0) {
    return _failure("cvmhsgbn failure");
  }

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    printf("unmatched result\n");
    printf("%s\n",outfile);
    printf("%s\n",reffile);
    return _failure("diff file");
  }

  unlink(outfile);

  return _success();
}

int test_cvmhsgbn_grid_offset()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: model with large grid in offset mode\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-grid-offset.in");
  sprintf(outfile, "%s/%s", currentdir, "test-grid-extract-cvmhsgbn-offset.out");
  sprintf(reffile, "%s/%s", currentdir, "./ref/test-grid-extract-cvmhsgbn-offset.ref");

  if (test_assert_int(runCVMHSGBN(BIN_DIR, MODEL_DIR,infile, outfile,
				MODE_NONE), 0) != 0) {
    return _failure("cvmhsgbn failure");
  }

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    printf("unmatched result\n");
    printf("%s\n",outfile);
    printf("%s\n",reffile);
    return _failure("diff file");
  }

  unlink(outfile);

  return _success();
}


int test_vx_lite_cvmhsgbn_grid_elev()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: vx_lite_cvmhsgbn executable with large grid in elevation mode\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-grid-elev.in");
  sprintf(outfile, "%s/%s", currentdir, "test-grid-extract-vx-lite-cvmhsgbn-elev.out");
  sprintf(reffile, "%s/%s", currentdir, "./ref/test-grid-extract-vx-lite-cvmhsgbn-elev.ref");

  if (test_assert_int(runVXLiteCVMHSGBN(BIN_DIR, MODEL_DIR, infile, outfile, 
				MODE_ELEVATION), 0) != 0) {
    return _failure("vx_lite_cvmhsgbn failure");
  }  

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    printf("unmatched result\n");
    printf("%s\n",outfile);
    printf("%s\n",reffile);
    return _failure("diff file");
  }

  unlink(outfile);

  return _success();
}


int test_vx_lite_cvmhsgbn_grid_depth()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: vx_lite_cvmhsgbn executable with large grid in depth mode\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-grid-depth.in");
  sprintf(outfile, "%s/%s", currentdir, "test-grid-extract-vx-lite-cvmhsgbn-depth.out");
  sprintf(reffile, "%s/%s", currentdir, "./ref/test-grid-extract-vx-lite-cvmhsgbn-depth.ref");

  if (test_assert_int(runVXLiteCVMHSGBN(BIN_DIR, MODEL_DIR, infile, outfile, 
				MODE_DEPTH), 0) != 0) {
    return _failure("vx_lite_cvmhsgbn failure");
  }  

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    printf("unmatched result\n");
    printf("%s\n",outfile);
    printf("%s\n",reffile);
    return _failure("diff file");
  }

  unlink(outfile);

  return _success();
}


int test_vx_lite_cvmhsgbn_grid_offset()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: vx_lite_cvmhsgbn executable with large grid in elev offset mode\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-grid-offset.in");
  sprintf(outfile, "%s/%s", currentdir, 
	  "test-grid-extract-vx-lite-cvmhsgbn-offset.out");
  /* Use same reference file as depth */
  sprintf(reffile, "%s/%s", currentdir, 
	  "./ref/test-grid-extract-vx-lite-cvmhsgbn-offset.ref");

  if (test_assert_int(runVXLiteCVMHSGBN(BIN_DIR, MODEL_DIR, infile, outfile, 
				MODE_NONE), 0) != 0) {
    return _failure("vx_lite_cvmhsgbn failure");
  }  

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    printf("unmatched result\n");
    printf("%s\n",outfile);
    printf("%s\n",reffile);
    return _failure("diff file");
  }

  unlink(outfile);

  return _success();
}

int suite_grid_exec(const char *xmldir)
{
  suite_t suite;
  char logfile[256];
  FILE *lf = NULL;

  /* Setup test suite */
  strcpy(suite.suite_name, "suite_grid_exec");
  suite.num_tests = 6;
  suite.tests = malloc(suite.num_tests * sizeof(test_t));
  if (suite.tests == NULL) {
    fprintf(stderr, "Failed to alloc test structure\n");
    return(1);
  }
  test_get_time(&suite.exec_time);

  /* Setup test cases */
  strcpy(suite.tests[0].test_name, "test_cvmhsgbn_grid_elev");
  suite.tests[0].test_func = &test_cvmhsgbn_grid_elev;
  suite.tests[0].elapsed_time = 0.0;

  strcpy(suite.tests[1].test_name, "test_cvmhsgbn_grid_depth");
  suite.tests[1].test_func = &test_cvmhsgbn_grid_depth;
  suite.tests[1].elapsed_time = 0.0;

  strcpy(suite.tests[2].test_name, "test_cvmhsgbn_grid_offset");
  suite.tests[2].test_func = &test_cvmhsgbn_grid_offset;
  suite.tests[2].elapsed_time = 0.0;

  strcpy(suite.tests[3].test_name, "test_vx_lite_cvmhsgbn_grid_elev");
  suite.tests[3].test_func = &test_vx_lite_cvmhsgbn_grid_elev;
  suite.tests[3].elapsed_time = 0.0;

  strcpy(suite.tests[4].test_name, "test_vx_lite_cvmhsgbn_grid_depth");
  suite.tests[4].test_func = &test_vx_lite_cvmhsgbn_grid_depth;
  suite.tests[4].elapsed_time = 0.0;

  strcpy(suite.tests[5].test_name, "test_vx_lite_cvmhsgbn_grid_offset");
  suite.tests[5].test_func = &test_vx_lite_cvmhsgbn_grid_offset;
  suite.tests[5].elapsed_time = 0.0;

  if (test_run_suite(&suite) != 0) {
    fprintf(stderr, "Failed to execute tests\n");
    return(1);
  }

  if (xmldir != NULL) {
    sprintf(logfile, "%s/%s.xml", xmldir, suite.suite_name);
    lf = init_log(logfile);
    if (lf == NULL) {
      fprintf(stderr, "Failed to initialize logfile\n");
      return(1);
    }
    
    if (write_log(lf, &suite) != 0) {
      fprintf(stderr, "Failed to write test log\n");
      return(1);
    }
    
    close_log(lf);
  }

  free(suite.tests);

  return 0;
}
