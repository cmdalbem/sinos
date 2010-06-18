// constants.h
//
// General purpose macros and constants.
//

#pragma once


// Types
struct point
{
	float x,y,z;
};

// Macros
#define RGB(x) (float)(x)/(255)
#define VARNAME(x) #x
#define RAD(x) ((x)*(M_PI))/(180)

// Default values for Materials Attributes
const float default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
const float default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const float default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float default_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float default_shininess = 0;

// Global variables
#define NSTARS 1000
#define STATEUPDATES_PER_SEC 50
#define TOPBOUND 2.500
#define BOTBOUND -2.05
#define LEFTBOUND -4.550
#define RIGHTBOUND 4.500

// keyboard keys
enum{
	K_ESC		= 27,
	K_SPACE		= 32
};
	
	



