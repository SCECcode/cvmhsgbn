/*
 * @file cvmhsgbn_ucvm_validate.c
 * @brief test with a full set of validation points in depth
 * @author - SCEC
 * @version 1.0
 *
 * Tests the CVMHLABN library by loading it and executing the code as
 * UCVM would.
 *
 *
 *  ./cvmhsgbn_ucvm_validate -f validate_api_good.txt
 *
 *  test 2 mode: query-by-elevation and query-by-depth
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "ucvm.h"
#include "ucvm_utils.h"

#include "cvmhsgbn.h"

int validate_debug = 0;

/*********************************************/
typedef struct dat_entry_t 
{
  int x_idx;
  int y_idx;
  int z_idx;
  int vp_idx;
  int vs_idx;
  int rho_idx;
  int depth_idx;
} dat_entry_t;

typedef struct dat_data_t 
{
  double x;
  double y;
  double z;
  double vp;
  double vs;
  double rho;
  double depth;
} dat_data_t;

dat_entry_t dat_entry;

/*
X,Y,Z,depth,vp63_basin,vs63_basin
383000.000000,3744000.000000,-15000.000000,0.000000,-99999.000000,-99999.000000
384000.000000,3744000.000000,-15000.000000,0.000000,-99999.000000,-99999.000000
*/
FILE *_process_datfile(char *fname) {

  char dat_line[1028];
  FILE *fp = fopen(fname, "r");
  if (fp == NULL) {
    fprintf(stderr,"VALIDATE_UCVM: FAIL: Unable to open the validation data file %s\n", fname);
    exit(1);
  }
  /* read the title line */
  if (fgets(dat_line, 1028, fp) == NULL) {
    fprintf(stderr,"VALIDATE_UCVM: FAIL: Unable to extract validation data file %s\n", fname);
    fclose(fp);
    exit(1);
  }

  /* Strip terminating newline */
  int slen = strlen(dat_line);
  if ((slen > 0) && (dat_line[slen-1] == '\n')) {
    dat_line[slen-1] = '\0';
  }

  char delimiter[] = ",";
  char *p = strtok(dat_line, delimiter);
  int counter=0;

// X,Y,Z,depth,vp63_basin,vs63_basin
  while(p != NULL)
  {
    if(validate_debug) { printf("VALIDATE_UCVM:'%s'\n", p); }
    if(strcmp(p,"X")==0)
      dat_entry.x_idx=counter;
    else if(strcmp(p,"Y")==0)
      dat_entry.y_idx=counter;
    else if(strcmp(p,"Z")==0)
      dat_entry.z_idx=counter;
    else if(strcmp(p,"vp63_basin")==0)
      dat_entry.vp_idx=counter;
    else if(strcmp(p,"vs63_basin")==0)
      dat_entry.vs_idx=counter;
    else if(strcmp(p,"depth")==0)
      dat_entry.depth_idx=counter;
    p = strtok(NULL, delimiter);
    counter++;
  }
  return fp;
}

int _next_datfile(FILE *fp, dat_data_t *dat) {

  char dat_line[1028];
  if (fgets(dat_line, 1028, fp) == NULL) {
    return(1); 
  }

  char delimiter[] = ",";
  char *p = strtok(dat_line, delimiter);
  int counter=0;

//X,Y,Z,depth,vp63_basin,vs63_basin
//383000.000000,3744000.000000,-15000.000000,0.000000,-99999.000000,-99999.000000
  while(p != NULL) {
    double val = atof(p);
    if(counter == dat_entry.x_idx)
        dat->x=val;
      else if (counter == dat_entry.y_idx)
        dat->y=val;
      else if (counter == dat_entry.z_idx)
        dat->z=val;
      else if (counter == dat_entry.vs_idx)
        dat->vs=val;
      else if (counter == dat_entry.vp_idx)
        dat->vp=val;
      else if (counter == dat_entry.depth_idx)
        dat->depth=val;
    p = strtok(NULL, delimiter);
    counter++;
  }
  return(0);
}


/*********************************************/

int _compare_double(double f1, double f2) {
  double precision = 0.00001;
  if (((f1 - precision) < f2) && ((f1 + precision) > f2)) {
    return 0; // good
    } else {
      return 1; // bad
  }
}

/* Usage function */
void usage() {
  printf("     vx_cvmhsgbn_valiate - (c) SCEC\n");
  printf("Extract velocities from a simple GOCAD voxet. Accepts\n");
  printf("geographic coordinates and UTM Zone 11, NAD27 coordinates in\n");
  printf("X Y Z columns. Z is expressed as elevation offset by default.\n\n");
  printf("\tusage: vx_cvmhsgbn_validate [-d] [-z dep/elev/off] -f file.dat\n\n");
  printf("Flags:\n");
  printf("\t-f point.dat\n\n");
  printf("\t-d enable debug/verbose mode\n\n");
  printf("\t-z directs use of dep/elev/off for Z column (default is dep).\n\n");
  printf("Output format is:\n");
  printf("\tvp vs rho\n\n");
  exit (0);
}

extern char *optarg;
extern int optind, opterr, optopt;

/**
 * Initializes CVMHLABN in standalone mode as ucvm plugin 
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
        int rc;
        int opt;

        char datfile[100]="";
        char configfile[100]="";
        ucvm_ctype_t cmode=XXX;

        dat_data_t dat;
        int tcount=0;  // total entry
        int mcount=0;  // real mismatch
        int mmcount=0; // fake mismatch -- no data
        int okcount=0;
        FILE *ofp= fopen("validate_ucvm_bad.txt","w");
        FILE *oofp= fopen("validate_ucvm_good.txt","w");
        fprintf(ofp,"X,Y,Z,depth,vp63_basin,vs63_basin\n");
        fprintf(oofp,"X,Y,Z,depth,vp63_basin,vs63_basin\n");

        /* Parse options */
        while ((opt = getopt(argc, argv, "df:c:h")) != -1) {
          switch (opt) {
          case 'c':
            strcpy(configfile, optarg);
            break;
          case 'f':
            strcpy(datfile, optarg);
            break;
XXX
          case 'z':
            if (strcasecmp(optarg, "dep") == 0) {
              zmode = UCVM_COORD_GEO_DEPTH;
            } else if (strcasecmp(optarg, "elev") == 0) {
              zmode = UCVM_COORD_GEO_ELEV;
            } else {
              fprintf(stderr, "VALIDATE_UCVM: Invalid coord type %s", optarg);
              usage();
              exit(0);
            }
            break;
          case 'd':
            validate_debug=1;
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

      
        FILE *fp=_process_datfile(datfile);

	// Initialize the model. 
        // try to use Use UCVM_INSTALL_PATH
        char *envstr=getenv("UCVM_INSTALL_PATH");
        if(envstr != NULL) {
	   assert(cvmhsgbn_init(envstr, "cvmhsgbn") == 0);
           } else {
	     assert(cvmhsgbn_init("..", "cvmhsgbn") == 0);
        }
	printf("VALIDATE_UCVM: Loaded the model successfully.\n");

        assert(cvmhsgbn_setparam(0, UCVM_PARAM_QUERY_MODE, zmode) == 0);
	printf("VALIDATE_UCVM: Set model zmode successfully.\n");

        rc=_next_datfile(fp, &dat); // do it one by one with ucvm_query.. 
        while(rc==0) {
XXX

              tcount++;
              pt.longitude = dat.x;
              pt.latitude = dat.y;
              pt.depth = dat.depth;

	      rc=cvmhsgbn_query(&pt, &ret, 1); // rc 0 is okay

              if(validate_debug) {fprintf(stderr, "VALIDATE_UCVM:   with.. %lf %lf %lf\n",pt.longitude, pt.latitude, pt.depth); }
              if(rc == 0) {

                if(validate_debug) {
                   fprintf(stderr,"VALIDATE_UCVM:     vs:%lf vp:%lf rho:%lf\n\n",ret.vs, ret.vp, ret.rho);
                }

                // is result matching ?
                if(_compare_double(ret.vs, dat.vs) ||
                             _compare_double(ret.vp, dat.vp)) { 

/*** special case.. only in lr

VALIDATE_UCVM:356000.000000,3754000.000000,-100.000114
VALIDATE_UCVM: dat.vs(-99999.000000),dat.vp(1480.000000)
VALIDATE_UCVM:   ret vs:(-1.000000) ret vp:(-1.000000)

**/
                     // okay if ( dat.vp == -99999, dat.vs== -99999 ) and (ret.vs == -1, ret.vp == -1) 
                     if (!_compare_double(ret.vs, -1.0) && !_compare_double(ret.vp, -1.0) &&
                              !_compare_double(dat.vs, -99999.0) && !_compare_double(dat.vp, -99999.0)) {
                       mmcount++;  // just -1 vs -99999
                       fprintf(oofp,"%lf,%lf,%lf,%lf,%lf,%lf\n",dat.x,dat.y,dat.z,dat.depth,dat.vp,dat.vs);
                       } else {
                         fprintf(stderr,"\nVALIDATE_UCVM:Mismatching -\n");
                         fprintf(stderr,"VALIDATE_UCVM:%lf,%lf,%lf\n",dat.x, dat.y, dat.z);
                         fprintf(stderr,"VALIDATE_UCVM: dat.vs(%lf),dat.vp(%lf)\n",dat.vs, dat.vp);
                         fprintf(stderr,"VALIDATE_UCVM:   ret vs:(%lf) ret vp:(%lf)\n",ret.vs, ret.vp);
                         mcount++;  // real mismatch
                         fprintf(ofp,"%lf,%lf,%lf,%lf,%lf,%lf\n",dat.x,dat.y,dat.z,dat.depth,dat.vp,dat.vs);
                      }
                    } else {
                         okcount++;
                  }
                } else { // rc=1 
                   if(validate_debug) printf("VALIDATE_UCVM: BAD,  %lf %lf %lf\n",pt.longitude, pt.latitude, pt.depth);
              }
          rc=_next_datfile(fp, &dat);
        }

        fprintf(stderr,"VALIDATE_UCVM: %d mismatch out of %d \n", mcount, tcount);
        fprintf(stderr,"VALIDATE_UCVM: good with matching values(%d) mmcount(%d) \n",okcount, mmcount );
	assert(cvmhsgbn_finalize() == 0);
	printf("VALIDATE_UCVM:Model closed successfully.\n");


        fclose(fp);
        fclose(ofp);
        fclose(oofp);

	return 0;
}




#define NUM_POINTS 20000
char *configfile;
ucvm_ctype_t cmode;



  cmode = UCVM_COORD_GEO_DEPTH;
  snprintf(configfile, UCVM_MAX_PATH_LEN, "%s", "./ucvm.conf");
  snprintf(modellist, UCVM_MAX_MODELLIST_LEN, "%s", "1d");
  snprintf(map_label, UCVM_MAX_LABEL_LEN, "%s", UCVM_MAP_UCVM);

 /* Initialize interface */
  if (ucvm_init(configfile) != UCVM_CODE_SUCCESS) {
    fprintf(stderr, "Failed to initialize UCVM API\n");
    return(1);
  }

  /* Add models */
  if (ucvm_add_model_list("cvmhsgbn") != UCVM_CODE_SUCCESS) {
    fprintf(stderr, "Failed to enable model list %s\n", modellist);
    return(1);
  }

  /* Set z mode for depth*/
  if (ucvm_setparam(UCVM_PARAM_QUERY_MODE, cmode) != UCVM_CODE_SUCCESS) {
    fprintf(stderr, "Failed to set z mode\n");
    fprintf(stderr, "Failed to set z mode\n");
    return(1);
  }

  /* Allocate buffers */
  pnts = malloc(NUM_POINTS * sizeof(ucvm_point_t));
  qpnts = malloc((int)(max_depth/z_inter) * sizeof(ucvm_point_t));
  qprops = malloc((int)(max_depth/z_inter) * sizeof(ucvm_data_t));

  /* Read in coords */
  while (!feof(stdin)) {
    memset(&(pnts[numread]), 0, sizeof(ucvm_point_t));
    if (


fscanf(stdin,"%lf %lf", &(pnts[numread].coord[0]),

      /* Check for scan failure */
      if ((pnts[numread].coord[0] == 0.0) ||
          (pnts[numread].coord[1] == 0.0)) { continue; }


      numread++;
      if (numread == NUM_POINTS) {

      qpnts[i].coord[0] = pnts[p].coord[0];
      qpnts[i].coord[1] = pnts[p].coord[1];
      qpnts[i].coord[2] = (double)i * z_inter;
    }

    /* Query the UCVM */
    if (ucvm_query(numz, qpnts, qprops) != UCVM_CODE_SUCCESS) {
      fprintf(stderr, "Query CVM failed\n");
      return(1);
    }
kkkk
XXX
        if (extract_basins(numread, pnts, qpnts, qprops,
                           max_depth, z_inter,
                           vs_thresh) != UCVM_CODE_SUCCESS) {
          fprintf(stderr, "Query basins failed\n");


          return(1);
        }
        numread = 0;
      }
    }
  }

  if (numread > 0) {
XXX
    if (extract_basins(numread, pnts, qpnts, qprops,
                       max_depth, z_inter,
                       vs_thresh) != UCVM_CODE_SUCCESS) {
      fprintf(stderr, "Query basins failed\n");
      return(1);
    }
    numread = 0;
  }

  ucvm_finalize();
  free(pnts);
  free(qpnts);
  free(qprops);

  return(0);
}



/* Extract basin values for the specified points */
int extract_properties(int n, ucvm_point_t *pnts, \
                   ucvm_point_t *qpnts, ucvm_data_t *qprops,
                   double max_depth, double z_inter, double vs_thresh)
{
  int i, p, dnum, numz;
  double vs_prev;
  double depths[3];
 
  numz = (int)(max_depth / z_inter);
  for (p = 0; p < n; p++) {
    /* Setup query points */
    for (i = 0; i < numz; i++) {
      qpnts[i].coord[0] = pnts[p].coord[0];
      qpnts[i].coord[1] = pnts[p].coord[1];
      qpnts[i].coord[2] = (double)i * z_inter;
    }

    /* Query the UCVM */
    if (ucvm_query(numz, qpnts, qprops) != UCVM_CODE_SUCCESS) {
      fprintf(stderr, "Query CVM failed\n");
      return(1);
    }

    /* Check for threshold crossing */
    vs_prev = DEFAULT_ZERO_DEPTH;
    dnum = 0;
    depths[0] = DEFAULT_NULL_DEPTH;
    depths[1] = DEFAULT_NULL_DEPTH;
    depths[2] = DEFAULT_NULL_DEPTH;
    for (i = 0; i < numz; i++) {
      /* Compare the Vs if it is valid */
      if (qprops[i].cmb.vs > DEFAULT_ZERO_DEPTH) {
        if ((vs_prev < vs_thresh) && (qprops[i].cmb.vs >= vs_thresh)) {
          depths[dnum] = (double)i * z_inter;
          if (dnum == 0) {
            depths[2] = depths[1] = depths[0];
            dnum++;
            } else {
              if(dnum == 1) {
                 depths[2] = depths[1];
                 dnum++;
              }
          }

        }

        /* Save current vs value */
        vs_prev = qprops[i].cmb.vs;
      }
    }

    /* Display output */
    printf(OUTPUT_FMT, pnts[p].coord[0], pnts[p].coord[1],
           depths[0], depths[1], depths[2]);

  }

  return(0);
}


