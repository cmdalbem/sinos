// utils.h
//
// General use functions collected from the Internet and addapted for SiNoS.
//

#pragma once

#include <iostream>
using namespace std;
#include <math.h>
#include <string.h>

#include <GL/gl.h>
#include "lib/Texture.h"
#include "Camera.h"
#include "constants.h"


/* glCircle3f()
 * Draws a circle contorn with constant y coordinate.
 * 	Adapted from http://forum.gamedeception.net/threads/4723-Drawing-a-circle-in-OpenGL-%28simple!%29
 */
void glCircle3f ( GLfloat x, GLfloat y, GLfloat z, GLfloat radius );

/* loadTexture()
 * 	Adapted from http://www.3dcodingtutorial.com/Textures/Loading-Textures.html
 */
void loadTexture(Texture *texVar, string texFile, bool makeMipmap=false);

/* drawOsd()
 * Draws On-Screen Display
 * 	Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/bitfont.c
 */
void drawOsd ( char osd[], Camera camera, long int fps );

/* drawPlane()
 * 	Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/spots.c
 */
void drawPlane ( int w, int h, float nx, float ny, float nz );
