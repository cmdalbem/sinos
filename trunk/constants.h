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
#define BALL_O_Y 29.5

// with frame
/*#define TOPBOUND 25.4
#define BOTBOUND -20.5
#define LEFTBOUND -45.6
#define RIGHTBOUND 45.2*/

// without frame
#define TOPBOUND 27.2
#define BOTBOUND -22.3
#define LEFTBOUND -47.4
#define RIGHTBOUND 47

// positions of the holes
const float B1P1[] = {-48.3, -19.9};
const float B1P2[] = {-45  , -23.5};
const float B2P1[] = {-2.4 , -23.5};
const float B2P2[] = { 2.1 , -23.5};
const float B3P1[] = { 44.7, -23.5};
const float B3P2[] = { 48  , -19.9};
const float B4P1[] = {-48.3,  24.8};
const float B4P2[] = {-45  ,  28.1};
const float B5P1[] = {-2.4 ,  28.1};
const float B5P2[] = { 1.8 ,  28.1};
const float B6P1[] = { 48  ,  24.5};
const float B6P2[] = { 44.4,  28.1};

// keyboard keys
enum{
	K_ESC		= 27,
	K_SPACE		= 32
};
	
	



