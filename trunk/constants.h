#pragma once


// Macros

#define RGB(x) (float)(x)/(255)



// default values for materials attributes

const float default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

const float default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };

const float default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };

const float default_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };


// global variables

#define NSTARS 1000
#define VFACTOR 150
#define DFACTOR 10
