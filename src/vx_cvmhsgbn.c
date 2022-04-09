/*
 * @file vx_cvmhsgbn.c
 * @brief Bootstraps the test framework for the CVMHSGBN library.
 * @author - SCEC
 * @version 1.0
 *
 * Tests the CVMHSGBN library by loading it and executing the code as
 * UCVM would.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "cvmhsgbn.h"

extern int cvmhsgbn_debug;

int _compare_double(double f1, double f2) {
  double precision = 0.00001;
  if (((f1 - precision) < f2) && ((f1 + precision) > f2)) {
    return 1;
    } else {
      return 0;
  }
}

/* Usage function */
void usage() {
  printf("     vx_cvmhsgbn - (c) SCEC\n");
  printf("Extract velocities from a simple GOCAD voxet. Accepts\n");
  printf("geographic coordinates and UTM Zone 11, NAD27 coordinates in\n");
  printf("X Y Z columns. Z is expressed as elevation offset by default.\n\n");
  printf("\tusage: vx_cvmhsgbn [-d] [-z dep/elev/off] < file.in\n\n");
  printf("Flags:\n");
  printf("\t-d enable debug/verbose mode\n\n");
  printf("\t-z directs use of dep/elev/off for Z column (default is dep).\n\n");
  printf("Output format is:\n");
  printf("\tvp vs rho\n\n");
  exit (0);
}

extern char *optarg;
extern int optind, opterr, optopt;

/**
 * Initializes CVMHSGBN in standalone mode as ucvm plugin 
 * api.
 *
 * @param argc The number of arguments.
 * @param argv The argument strings.
 * @return A zero value indicating success.
 */
int main(int argc, char* const argv[]) {

	// Declare the structures.
	cvmhsgbn_point_t pt;
	cvmhsgbn_properties_t ret;
        int zmode=UCVM_COORD_GEO_DEPTH;
        int rc;
        int opt;


        /* Parse options */
        while ((opt = getopt(argc, argv, "z:h")) != -1) {
          switch (opt) {
          case 'z':
            if (strcasecmp(optarg, "dep") == 0) {
              zmode = UCVM_COORD_GEO_DEPTH;
            } else if (strcasecmp(optarg, "elev") == 0) {
              zmode = UCVM_COORD_GEO_ELEV;
            } else {
              fprintf(stderr, "Invalid coord type %s", optarg);
              usage();
              exit(0);
            }
            break;
          case 'h':
            usage();
            exit(0);
            break;
          default: /* '?' */
            usage();
            exit(1);
          }
        }

	// Initialize the model. 
        // try to use Use UCVM_INSTALL_PATH
        char *envstr=getenv("UCVM_INSTALL_PATH");
        if(envstr != NULL) {
	   assert(cvmhsgbn_init(envstr, "cvmhsgbn") == 0);
           } else {
	     assert(cvmhsgbn_init("..", "cvmhsgbn") == 0);
        }
	printf("Loaded the model successfully.\n");

        assert(cvmhsgbn_setparam(0, UCVM_PARAM_QUERY_MODE, zmode) == 0);
	printf("Set model zmode successfully.\n");

        char line[2001];
        while (fgets(line, 2000, stdin) != NULL) {
           if(line[0]=='#') continue; // comment line
           if (sscanf(line,"%lf %lf %lf",
               &pt.longitude,&pt.latitude,&pt.depth) == 3) {

// using cvmhsgbn -- everything is depth so need to convert here..
              if(zmode == UCVM_COORD_GEO_ELEV ) {
                double elev=pt.depth;
                float surface;
                double coor[3]; 
                coor[0]=pt.longitude;
                coor[1]=pt.latitude;
                coor[2]=pt.depth;
                vx_coord_t coor_type;
                if ((coor[0]<360.) && (fabs(coor[1])<90)) {
                   coor_type = VX_COORD_GEO;
                   } else {
                      coor_type = VX_COORD_UTM;
                }
                vx_getsurface(coor, coor_type, &surface);
                pt.depth = surface - elev;

                if(cvmhsgbn_debug) {
                  fprintf(stderr, "  calling vx_getsurface: surface is %f, initial elevation %f > depth(%f)\n",
                         surface, elev, pt.depth);
                }
              }

	      rc=cvmhsgbn_query(&pt, &ret, 1);
              if(cvmhsgbn_debug) {fprintf(stderr, " >>>> with.. %lf %lf %lf\n\n",pt.longitude, pt.latitude, pt.depth); }
              if(rc == 0) {
                printf("vs: %lf vp: %lf rho: %lf\n",ret.vs, ret.vp, ret.rho);
                } else {
                   printf("BAD: %lf %lf %lf\n",pt.longitude, pt.latitude, pt.depth);
              }
              } else {
                 break;
           }
        }

	assert(cvmhsgbn_finalize() == 0);
	printf("Model closed successfully.\n");

	return 0;
}
