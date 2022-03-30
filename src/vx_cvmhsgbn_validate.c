/*
 * @file vx_cvmhsgbn_validate.c
 * @brief test with a full set of validation points
 * @author - SCEC
 * @version 1.0
 *
 * Tests the CVMHSGBN library by loading it and executing the code as
 * UCVM would.
 *
 *
 *  ./vx_cvmhsgbn_validate -z elev -f CVMHB-San-Gabriel-Basin.dat
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
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
} dat_entry_t;

typedef struct dat_data_t 
{
  double x;
  double y;
  double z;
  double vp;
  double vs;
  double rho;
} dat_data_t;

dat_entry_t dat_entry;

/*
X,Y,Z,tag61_basin,vp63_basin,vs63_basin
383000.000000,3744000.000000,-15000.000000,-99999.000000,-99999.000000,-99999.000000
384000.000000,3744000.000000,-15000.000000,-99999.000000,-99999.000000,-99999.000000
*/
FILE *_process_datfile(char *fname) {

  char dat_line[1028];
  FILE *fp = fopen(fname, "r");
  if (fp == NULL) {
    fprintf(stderr,"FAIL: Unable to open the validation data file %s\n", fname);
    exit(1);
  }
  /* read the title line */
  if (fgets(dat_line, 1028, fp) == NULL) {
    fprintf(stderr,"FAIL: Unable to extract validation data file %s\n", fname);
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

// X,Y,Z,tag61_basin,vp63_basin,vs63_basin
  while(p != NULL)
  {
//    printf("'%s'\n", p);
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

//X,Y,Z,tag61_basin,vp63_basin,vs63_basin
//383000.000000,3744000.000000,-15000.000000,-99999.000000,-99999.000000,-99999.000000
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
        char datfile[100]="";
        dat_data_t dat;


        /* Parse options */
        while ((opt = getopt(argc, argv, "df:z:h")) != -1) {
          switch (opt) {
          case 'f':
            strcpy(datfile, optarg);
            break;
          case 'z':
            if (strcasecmp(optarg, "dep") == 0) {
              zmode = UCVM_COORD_GEO_DEPTH;
            } else if (strcasecmp(optarg, "elev") == 0) {
              zmode = UCVM_COORD_GEO_ELEV;
            } else if (strcasecmp(optarg, "off") == 0) {
              zmode = UCVM_COORD_GEO_ELEVOFF;
            } else {
              fprintf(stderr, "Invalid coord type %s", optarg);
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
	printf("Loaded the model successfully.\n");

        assert(cvmhsgbn_setparam(0, UCVM_PARAM_QUERY_MODE, zmode) == 0);
	printf("Set model zmode successfully.\n");

        rc=_next_datfile(fp, &dat);
        while(rc==0) {
              pt.longitude = dat.x;
              pt.latitude = dat.y;
              pt.depth = dat.z;

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

                if(validate_debug) {
                  fprintf(stderr, "  calling vx_getsurface: surface is %f, initial elevation %f > depth(%f)\n",
                         surface, elev, pt.depth);
                }
              }

	      rc=cvmhsgbn_query(&pt, &ret, 1);
              if(validate_debug) {fprintf(stderr, ">>>> with.. %lf %lf %lf\n",pt.longitude, pt.latitude, pt.depth); }
              if(rc == 0) {
                if(validate_debug) {
                   printf("     vs:%lf vp:%lf rho:%lf\n\n",ret.vs, ret.vp, ret.rho);
                }
                if(_compare_double(ret.vs, dat.vs) ||
                             _compare_double(ret.vp, dat.vp)) { 
                   fprintf(stderr,"Mismatching -\n");
                   fprintf(stderr,"%lf %lf %lf\n",pt.longitude, pt.latitude, pt.depth);
                   fprintf(stderr,"    vs: %lf vp:%lf\n",ret.vs, ret.vp);
                   fprintf(stderr,"    dat_vs: %lf dat_vp: %lf\n\n",dat.vs, dat.vp);
                }
                } else {
                   printf("BAD: %lf %lf %lf\n",pt.longitude, pt.latitude, pt.depth);
              }
          rc=_next_datfile(fp, &dat);
        }

	assert(cvmhsgbn_finalize() == 0);
	printf("Model closed successfully.\n");

        fclose(fp);

	return 0;
}
