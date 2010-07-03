// constants.h
//
// General purpose macros and constants.
//

#pragma once


#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_MIDDLE 1
#define GLUT_WHEEL_UP   3
#define GLUT_WHEEL_DOWN 4
#endif


// Types
struct point
{
	float x,y,z;
};

// Macros
#define RGB(x) (float)(x)/(255)
#define VARNAME(x) #x
#define RAD(x) ((x)*(M_PI))/(180.)
#define DEGREES(x) ((x)*(180.))/(M_PI)

// Default values for Materials Attributes
const float default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
const float default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const float default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float default_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float default_shininess = 0;


// Global constants
#define TABLE_PLANE_Y 28.5
#define NHOLES 6
#define NCAMERAMODES 3
#define OSD_LINES 3
#define BALL_RADIUS (1.)
#define BALL_MAX_RES 80
#define BALL_MIN_RES 5
#define NPLAYERS 2

// Global variables
#define STATEUPDATES_PER_SEC (4000.) /* the semantics of this constant is confuse without glutTimer
									  * actually, it works like the game clock. Increase to make the game slower.
									  */
#define UPDATE_PRIORITY_FACTOR 100    /* game will re-render again after N updates
									  * increase and rendering will get laggy.
									  */

#define TIME_STEP (1000.)/STATEUPDATES_PER_SEC
#define BALL_DECELERATION_R (0.05)   // table wall friction
#define BALL_DECELERATION_N (0.2)    // table plane friction
#define BALL_ACCELERATION_G (1.1)    // gravitational acceleration
#define BALL_MIN_SPEED (0.5)
#define BALL_MAX_SPEED BALL_RADIUS*STATEUPDATES_PER_SEC
#define STICK_MAX_STR 30
#define MAX_BACKTRACK 1000
#define NSTARS 1000
#define LOD_FACTOR (1500.) // increase and will increase the medium quality of balls
#define FULLSCREEN_ON 0
#define SHOW_TABLE_FRAME 1 //comment to hide

// Table boundaries
#ifdef SHOW_TABLE_FRAME
	#define TOPBOUND 25.4
	#define BOTBOUND -20.5
	#define LEFTBOUND -45.6
	#define RIGHTBOUND 45.2
#else
	#define TOPBOUND 27.2
	#define BOTBOUND -22.3
	#define LEFTBOUND -47.4
	#define RIGHTBOUND 47
#endif

// Holes
// holes entrances limits {x,y}
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
// holes centers {x,z,radius}
const float HC[6][3] ={{-48. ,-22.9, 2.7},
				       {-0.3 ,-24.4, 2.4},
				  	   { 47.4,-22.6, 2.7},
				 	   {-48. , 27.8, 2.7},
					   {-0.3 , 29.3, 2.4},
					   { 47.4, 27.5, 2.7}};

// Keyboard keys
enum{
	K_ESC		= 27,
	K_SPACE		= 32
};
	
	



