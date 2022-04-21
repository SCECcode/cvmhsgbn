#ifndef VX_SUB_CVMHSGBN_H
#define VX_SUB_CVMHSGBN_H

extern char *VX_SRC_NAMES[7];

typedef enum { VX_SRC_NR = 0, 
	       VX_SRC_HR, 
	       VX_SRC_LR, 
	       VX_SRC_CM,
	       VX_SRC_TO,
	       VX_SRC_BK,
               VX_SRC_GT} vx_src_t;

// query point coming from UCVM's ucvm_query  
// already processed as (ucvm surface - elevation)
// so it is a depth
typedef enum { VX_ZMODE_DEPTH = 0, 
	       VX_ZMODE_ELEV, 
               VX_ZMODE_OTHER_ELEV } vx_zmode_t;

typedef enum { VX_COORD_GEO = 0, VX_COORD_UTM } vx_coord_t;


typedef enum { VX_REQUEST_ALL = 0, 
	       VX_REQUEST_TOPO, 
	       VX_REQUEST_VSVPRHO } vx_request_t;


typedef enum { VX_PROV_NONE = 0,
	       VX_PROV_MANTLE,
	       VX_PROV_TOMO,
	       VX_PROV_BASIN,
	       VX_PROV_AIR,
	       VX_PROV_BASIN_GTL,
	       VX_PROV_EXTRAP_TOMO,
	       VX_PROV_WATER,
	       VX_PROV_BASEMENT_GTL,
	       VX_PROV_BB_TRANS_OUTER,
	       VX_PROV_AIR_OUTER,
	       VX_PROV_FILLED_MANTLE,
	       VX_PROV_FILLED_CRUST,
	       VX_PROV_EXTRAP_MANTLE,
	       VX_PROV_BACKGND,
               VX_PROV_GTL} vx_prov_t;


typedef struct vx_entry_t 
{
  double coor[3];
  vx_coord_t coor_type;
  /*  vx_request_t request; */
  double coor_utm[3];
  float elev_cell[2];
  float topo;
  float mtop;
  float base;
  float moho;
  vx_src_t data_src;
  float vel_cell[3];
  float provenance;
  float vp;
  float vs;
  double rho;
  double depth;
} vx_entry_t;


typedef struct vx_voxel_t 
{
  int coor[3];
  vx_src_t data_src;
  float elev_cell[2];
  float topo;
  float mtop;
  float base;
  float moho;
  float vel_cell[3];
  float provenance;
  float vp;
  float vs;
  double rho;
} vx_voxel_t;


/* 
  Main API functions
*/


/* Initializer */
int vx_setup(const char* data_dir);

/* Cleanup function to free resources and restore state */
int vx_cleanup();

/* Set Z mode to either elevation, depth, or elev offset */
int vx_setzmode(vx_zmode_t m);

/* Retrieve data point in LatLon or UTM */
int vx_getcoord(vx_entry_t *entry);

/* Initialize data structures */
void vx_init_entry(vx_entry_t *entry);
void vx_init_voxel(vx_voxel_t *voxel);


/* 
  Tools for user background handlers 
*/

/* Retrieve data point by referencing voxel index position */
void vx_getvoxel(vx_voxel_t *voxel);

/* Retrieve true surface elev at data point */
void vx_getsurface(double *coor, vx_coord_t coor_type, float *surface);

/* Retrieve data point in LatLon or UTM */
int vx_getcoord_private(vx_entry_t *entry, int enhanced);
int vx_getsurface_private(double *coor, vx_coord_t coor_type, 
			  float *surface);
void vx_model_top(double *coor, vx_coord_t coor_type, 
		  float *surface);
void vx_voxel_at_coord(vx_entry_t *entry, vx_voxel_t *voxel);
void vx_closest_voxel_to_coord(vx_entry_t *entry, vx_voxel_t *voxel);
void vx_dist_point_to_voxel(vx_entry_t *entry, 
			    vx_voxel_t *voxel, 
			    float *dist_2d, float *dist_3d);

#endif
