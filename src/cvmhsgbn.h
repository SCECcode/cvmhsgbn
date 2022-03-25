#ifndef CVMHSGBN_H
#define CVMHSGBN_H

/**
 * @file cvmhsgbn.h
 * @brief Main header file for CVMHSGBN library.
 * @author - SCEC 
 * @version 1.0
 *
 * Delivers CVMH San Gabriel Basin Velocity Model
 *
 */

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>

#include "ucvm_model_dtypes.h"
#include "vx_sub.h"

// Constants
#ifndef M_PI
	/** Defines pi */
	#define M_PI 3.14159265358979323846
#endif

#define VX_NO_DATA -99999.0

// Structures
/** Defines a point (latitude, longitude, and depth) in WGS84 format */
typedef struct cvmhsgbn_point_t {
	/** Longitude member of the point */
	double longitude;
	/** Latitude member of the point */
	double latitude;
	/** Depth member of the point */
	double depth;
} cvmhsgbn_point_t;

/** Defines the material properties this model will retrieve. */
typedef struct cvmhsgbn_properties_t {
	/** P-wave velocity in meters per second */
	double vp;
	/** S-wave velocity in meters per second */
	double vs;
	/** Density in g/m^3 */
	double rho;
        /** NOT USED from basic_property_t */
        double qp;
        /** NOT USED from basic_property_t */
        double qs;
} cvmhsgbn_properties_t;

/** The CVMHSGBN configuration structure. */
typedef struct cvmhsgbn_configuration_t {
	/** The zone of UTM projection */
	int utm_zone;
	/** The model directory */
	char model_dir[1000];

} cvmhsgbn_configuration_t;

/** The model structure which points to available portions of the model. */
typedef struct cvmhsgbn_model_t {
	/** A pointer to the Vp data either in memory or disk. Null if does not exist. */
	void *vp;
	/** Vp status: 0 = not found, 1 = found and not in memory, 2 = found and in memory */
	int vp_status;
} cvmhsgbn_model_t;

// Constants
/** The version of the model. */
extern const char *cvmhsgbn_version_string;

// Variables
/** Set to 1 when the model is ready for query. */
extern int cvmhsgbn_is_initialized;

/** Location of the binary data files. */
extern char cvmhsgbn_data_directory[2000];

/** Configuration parameters. */
extern cvmhsgbn_configuration_t *cvmhsgbn_configuration;
/** Holds pointers to the velocity model data OR indicates it can be read from file. */
extern cvmhsgbn_model_t *cvmhsgbn_velocity_model;

/** The height of this model's region, in meters. */
extern double cvmhsgbn_total_height_m;
/** The width of this model's region, in meters. */
extern double cvmhsgbn_total_width_m;

// UCVM API Required Functions

#ifdef DYNAMIC_LIBRARY

/** Initializes the model */
int model_init(const char *dir, const char *label);
/** Cleans up the model (frees memory, etc.) */
int model_finalize();
/** Returns version information */
int model_version(char *ver, int len);
/** Queries the model */
int model_query(cvmhsgbn_point_t *points, cvmhsgbn_properties_t *data, int numpts);
/** Setparam */
int model_setparam(int, int, int);

#endif

// CVMHSGBN Related Functions

/** Initializes the model */
int cvmhsgbn_init(const char *dir, const char *label);
/** Cleans up the model (frees memory, etc.) */
int cvmhsgbn_finalize();
/** Returns version information */
int cvmhsgbn_version(char *ver, int len);
/** Queries the model */
int cvmhsgbn_query(cvmhsgbn_point_t *points, cvmhsgbn_properties_t *data, int numpts);
/** Setparam*/
int cvmhsgbn_setparam(int, int, ...);

// Non-UCVM Helper Functions
/** Reads the configuration file. */
int cvmhsgbn_read_configuration(char *file, cvmhsgbn_configuration_t *config);
void cvmhsgbn_print_error(char *err);
int cvmhsgbn_setzmode(char* z);

#endif