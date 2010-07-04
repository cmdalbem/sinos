// utils.h
//
// General use functions collected from the Internet and addapted for SiNoS.
//

#pragma once

#include <iostream>
using namespace std;
#include <math.h>
#include <string.h>
#include <vector>

#include <GL/gl.h>
#include "lib/Texture.h"
#include "ObjectBall.h"
#include "Camera.h"
#include "constants.h"


/* getDistance()
 * Returns the euclidian norma of the vector starting on object1 and 
 * ending on object2.
 */
double getDistance( double x1, double y1, double z1, double x2, double y2, double z2 );
double getDistance( Object &object1, Object &object2 );

/* drawGuideLine()
 * Draws a stipped line which represents the trajectory of the ball if
 * the player attacks with the present configuration.
 */
void drawGuideLine( double posx, double posy, double posz, double stickAngle);
void makeGuideLine( vector<ObjectBall> balls, double stickAngle);

/* getRandBetween()
 * Returns a random number in the closed interval.
 */
double getRandBetween(int a, int b);

double normalizeVector( double v[3] );

double getVectorNorma( const double v[3] );
double getVectorNorma( const double x, const double z );

double getVectorAngle( const double v[3] );
double getVectorAngle( double x, double z );

double dotProduct( const double v[3], const double u[3] );

/* glShadowProjection()
 * Original documentation follows:
 * 
 *   This is where the "magic" is done:
 *
 *   Multiply the current ModelView-Matrix with a shadow-projetion
 *   matrix.
 *
 *   l is the position of the light source
 *   e is a point on within the plane on which the shadow is to be
 *     projected.  
 *   n is the normal vector of the plane.
 *
 *   Everything that is drawn after this call is "squashed" down
 *   to the plane. Hint: Gray or black color and no lighting 
 *   looks good for shadows *g* 
 *
 * This function was adapted from the Internet. There as so many sources that I can't tell the original author of this.
 */
void glShadowProjection(float * l, float * e, float * n);

/* glCircle3f()
 * Draws a circle contorn with constant y coordinate.
 * 	Adapted from http://forum.gamedeception.net/threads/4723-Drawing-a-circle-in-OpenGL-%28simple!%29
 */
void glCircle3f ( GLfloat x, GLfloat y, GLfloat z, GLfloat radius );

/* loadTexture()
 * 	Adapted from http://www.3dcodingtutorial.com/Textures/Loading-Textures.html
 */
void loadTexture(Texture *texVar, string texFile, bool makeMipmap=false);
	
/* drawPlane()
 * 	Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/spots.c
 */
void drawPlane ( int w, int h, float nx, float ny, float nz );
