/**  
   test_vx_cvmhsgbn_exec.c

   invokes src/run_vx_cvmhsgbn.sh/vx_cvmhsgbn
     which uses cvmhsgbn api,
       cvmhsgbn_init, cvmhsgbn_setparam, cvmhsgbn_query, cvmhsgbn_finalize
**/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include "vx_sub_cvmhsgbn.h"
#include "unittest_defs.h"
#include "test_helper.h"
#include "test_vx_cvmhsgbn_exec.h"

int VX_TESTS=6;

int test_vx_cvmhsgbn_points_elevation()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: vx_cvmhsgbn executable with elevation option\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-elev.in");
  sprintf(outfile, "%s/%s", currentdir, 
	  "test-10-point-vx-cvmhsgbn-extract-elev.out");
  sprintf(reffile, "%s/%s", currentdir, 
	  "./ref/test-10-point-vx-cvmhsgbn-extract-elev.ref");

  if (test_assert_int(save_elevation_test_points(infile), 0) != 0) {
    return _failure("save test points failed");
  }

  if (test_assert_int(runVXCVMHSGBN(BIN_DIR, MODEL_DIR, infile, outfile, 
				MODE_ELEVATION), 0) != 0) {
    return _failure("vx_cvmhsgbn failure");
  }

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    return _failure("unmatch result with reference");
  }

  unlink(outfile);

  return _success();
}


int test_vx_cvmhsgbn_points_depth()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: vx_cvmhsgbn executable with depth option\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-depth.in");
  sprintf(outfile, "%s/%s", currentdir, 
	  "test-10-point-vx-cvmhsgbn-extract-depth.out");
  sprintf(reffile, "%s/%s", currentdir, 
	  "./ref/test-10-point-vx-cvmhsgbn-extract-depth.ref");

  if (test_assert_int(save_depth_test_points(infile), 0) != 0) {
    return _failure("save test points failed");
  }

  if (test_assert_int(runVXCVMHSGBN(BIN_DIR, MODEL_DIR, infile, outfile, 
				MODE_DEPTH), 0) != 0) {
    return _failure("vx_cvmhsgbn failure");
  }  

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    return _failure("diff failure");
  }

  unlink(outfile);

  return _success();
}

int test_vx_cvmhsgbn_points_depth_cvmhsgbn()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: vx_cvmhsgbn executable with depth option (cvmhsgbn)\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-depth-cvmhsgbn.in");
  sprintf(outfile, "%s/%s", currentdir, 
	  "test-vx-cvmhsgbn-extract-depth-cvmhsgbn.out");
  sprintf(reffile, "%s/%s", currentdir, 
	  "./ref/test-vx-cvmhsgbn-extract-depth-cvmhsgbn.ref");

  if (test_assert_int(runVXCVMHSGBN(BIN_DIR, MODEL_DIR, infile, outfile, 
				MODE_DEPTH), 0) != 0) {
    return _failure("vx_cvmhsgbn failure");
  }  

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    return _failure("diff failure");
  }

  unlink(outfile);

  return _success();
}


int test_vx_cvmhsgbn_points_ge()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: vx_cvmhsgbn validate ge option\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

// ge part
  sprintf(infile, "%s/%s", currentdir, "./inputs/test_latlons_cvmh_ge.txt");
  sprintf(outfile, "%s/%s", currentdir, 
	  "test_latlons_cvmh_ge.out");
  sprintf(reffile, "%s/%s", currentdir, 
	  "./ref/test_latlons_cvmh_ge.ref");

  if (test_assert_file_exist(infile) != 0) {
    return _failure("data file not found");
  }

  if (test_assert_int(runVXCVMHSGBN(BIN_DIR, MODEL_DIR, infile, outfile, 
				MODE_ELEVATION), 0) != 0) {
    return _failure("vx_cvmhsgbn failure");
  }

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    return _failure("diff file");
  }

  unlink(outfile);

  return _success();
}

int test_vx_cvmhsgbn_points_gd()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: vx_cvmhsgbn validate gd option\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

// ge part
  sprintf(infile, "%s/%s", currentdir, "./inputs/test_latlons_gd.txt");
  sprintf(outfile, "%s/%s", currentdir, 
	  "test_latlons_gd.out");
  sprintf(reffile, "%s/%s", currentdir, 
	  "./ref/test_latlons_gd.ref");

  if (test_assert_file_exist(infile) != 0) {
    printf("file:%s not found\n",infile);
    return _failure("data file not found");
  }

  if (test_assert_int(runVXCVMHSGBN(BIN_DIR, MODEL_DIR, infile, outfile, 
				MODE_DEPTH), 0) != 0) {
    return _failure("vx_cvmhsgbn failure");
  }

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    return _failure("diff file");
  }

  unlink(outfile);

  return _success();
}

int test_vx_cvmhsgbn_points_elevation_cvmhsgbn()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: vx_cvmhsgbn executable with elevation option (cvmhsgbn)\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-elev-cvmhsgbn.in");
  sprintf(outfile, "%s/%s", currentdir, 
	  "test-vx-cvmhsgbn-extract-elev-cvmhsgbn.out");
  sprintf(reffile, "%s/%s", currentdir, 
	  "./ref/test-vx-cvmhsgbn-extract-elev-cvmhsgbn.ref");

  if (test_assert_int(runVXCVMHSGBN(BIN_DIR, MODEL_DIR, infile, outfile, 
				MODE_ELEVATION), 0) != 0) {
    return _failure("vx_cvmhsgbn failure");
  }

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    return _failure("unmatch result with reference");
  }

  unlink(outfile);

  return _success();
}

int suite_vx_cvmhsgbn_exec(const char *xmldir)
{
  suite_t suite;
  char logfile[1280];
  FILE *lf = NULL;

  /* Setup test suite */
  strcpy(suite.suite_name, "suite_vx_cvmhsgbn_exec");

  suite.num_tests = VX_TESTS;
  suite.tests = malloc(suite.num_tests * sizeof(test_t));
  if (suite.tests == NULL) {
    fprintf(stderr, "Failed to alloc test structure\n");
    return(1);
  }
  test_get_time(&suite.exec_time);

  /* Setup test cases */
  strcpy(suite.tests[0].test_name, "test_vx_cvmhsgbn_points_elevation");
  suite.tests[0].test_func = &test_vx_cvmhsgbn_points_elevation;
  suite.tests[0].elapsed_time = 0.0;

  strcpy(suite.tests[1].test_name, "test_vx_cvmhsgbn_points_depth");
  suite.tests[1].test_func = &test_vx_cvmhsgbn_points_depth;
  suite.tests[1].elapsed_time = 0.0;

  strcpy(suite.tests[2].test_name, "test_vx_cvmhsgbn_points_gd");
  suite.tests[2].test_func = &test_vx_cvmhsgbn_points_gd;
  suite.tests[2].elapsed_time = 0.0;

  strcpy(suite.tests[3].test_name, "test_vx_cvmhsgbn_points_ge");
  suite.tests[3].test_func = &test_vx_cvmhsgbn_points_ge;
  suite.tests[3].elapsed_time = 0.0;

  strcpy(suite.tests[4].test_name, "test_vx_cvmhsgbn_points_elevation_cvmhsgbn");
  suite.tests[4].test_func = &test_vx_cvmhsgbn_points_elevation_cvmhsgbn;
  suite.tests[4].elapsed_time = 0.0;

  strcpy(suite.tests[5].test_name, "test_vx_cvmhsgbn_points_depth_cvmhsgbn");
  suite.tests[5].test_func = &test_vx_cvmhsgbn_points_depth_cvmhsgbn;
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
