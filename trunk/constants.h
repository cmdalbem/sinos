// constants.h
//
// General purpose macros and constants.
//

#pragma once


// Macros

#define RGB(x) (float)(x)/(255)


// default values for materials attributes
const float default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
const float default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const float default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float default_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float default_shininess = 0;


// global variables
#define NSTARS 1000
#define VFACTOR 150
#define DFACTOR 10


// keyboard keys
enum{
	K_ESC		= 27,
	K_F1		= 241,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_F11,
	K_F12
};
	
	



