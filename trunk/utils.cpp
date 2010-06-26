#include <iostream>
using namespace std;
#include <math.h>
#include <string.h>

#include <GL/gl.h>
#include "lib/Texture.h"
#include "Camera.h"
#include "constants.h"
#include "utils.h"
#include "time.h"


void drawGuideLine( double posx, double posy, double posz, double stickAngle)
{
	glColor4f(1.,1.,1.,1); 
	glLineStipple(2, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
		glVertex3f( posx, posy, posz);
		glVertex3f( posx+100*cos(RAD(stickAngle+90)), posy, posz+100*sin(RAD(-(stickAngle+90))) );
	glEnd();
	glDisable(GL_LINE_STIPPLE);	
}
double getRandBetween(int a, int b)
{
	srand(time(0));
	int d = abs(a) + abs(b) + 1;
	return (rand() % d) + a;
}

double normalizeVector( double v[3] )
{
	double norma = getVectorNorma(v);
	
	v[0] = v[0] / norma;
	v[1] = v[1] / norma;
	v[2] = v[2] / norma;
}

double getVectorNorma( const double v[3] )
{
	return sqrt( pow(v[0],2) + pow(v[1],2) + pow(v[2],2) );	
}

double getVectorAngle( const double v[3] )
{
	if( getVectorNorma(v) ) {
		float direction = DEGREES(acosf(v[0]/getVectorNorma(v))); //angle of move vector = arc cos x/hypotenuse
		
		if( (v[2]/getVectorNorma(v)) > 0 )
			direction += 2*(180-direction);
			
		return direction;
	}
	else
		return 0;
}	

void glShadowProjection(float * l, float * e, float * n)
/* This function was adapted from the Internet. There as so many sources that I can't tell the original author of this.*/
{
  float d, c;
  float mat[16];

  // These are c and d (corresponding to the tutorial)
  
  d = n[0]*l[0] + n[1]*l[1] + n[2]*l[2];
  c = e[0]*n[0] + e[1]*n[1] + e[2]*n[2] - d;

  // Create the matrix. OpenGL uses column by column
  // ordering

  mat[0]  = l[0]*n[0]+c; 
  mat[4]  = n[1]*l[0]; 
  mat[8]  = n[2]*l[0]; 
  mat[12] = -l[0]*c-l[0]*d;
  
  mat[1]  = n[0]*l[1];        
  mat[5]  = l[1]*n[1]+c;
  mat[9]  = n[2]*l[1]; 
  mat[13] = -l[1]*c-l[1]*d;
  
  mat[2]  = n[0]*l[2];        
  mat[6]  = n[1]*l[2]; 
  mat[10] = l[2]*n[2]+c; 
  mat[14] = -l[2]*c-l[2]*d;
  
  mat[3]  = n[0];        
  mat[7]  = n[1]; 
  mat[11] = n[2]; 
  mat[15] = -d;

  // Finally multiply the matrices together *plonk*
  glMultMatrixf(mat);
}

void glCircle3f(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) {
/* Adapted from http://forum.gamedeception.net/threads/4723-Drawing-a-circle-in-OpenGL-%28simple!%29*/
    float angle;
    glPushMatrix();
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 100; i++) {
			angle = i*2*M_PI/100;
			glVertex3f(x + (cos(angle) * radius), y, z + (sin(angle) * radius));
		}
    glEnd();
    glPopMatrix();
}  

void loadTexture(Texture *texVar, string texFile, bool makeMipmap)
/* Adapted from http://www.3dcodingtutorial.com/Textures/Loading-Textures.html */
{
    if (LoadTGA(texVar, (char*)texFile.c_str()))
    {

        // This tells opengl to create 1 texture and put it's ID in the given integer variable
        // OpenGL keeps a track of loaded textures by numbering them: the first one you load is 1, second is 2, ...and so on.
        glGenTextures(1, &texVar->texID);
        // Binding the texture to GL_TEXTURE_2D is like telling OpenGL that the texture with this ID is now the current 2D texture in use
        // If you draw anything the used texture will be the last binded texture
        glBindTexture(GL_TEXTURE_2D, texVar->texID);
        // This call will actualy load the image data into OpenGL and your video card's memory. The texture is allways loaded into the current texture
        // you have selected with the last glBindTexture call
        // It asks for the width, height, type of image (determins the format of the data you are giveing to it) and the pointer to the actual data
        glTexImage2D(GL_TEXTURE_2D, 0, texVar->bpp / 8, texVar->width, texVar->height, 0, texVar->type, GL_UNSIGNED_BYTE, texVar->imageData);

        if( makeMipmap ) {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			gluBuild2DMipmaps( GL_TEXTURE_2D, 3, texVar->width, texVar->height, GL_RGB, GL_UNSIGNED_BYTE, texVar->imageData ); 
		}
		else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);			
		}
        
        if (texVar->imageData)
        {
            // You can now free the image data that was allocated by LoadTGA
            // You don't want to keep a few Mb of worthless data on heap. It's worthless because OpenGL stores the image someware else after
            // you call glTexImage2D (usualy in you video card)
            free(texVar->imageData); 
        }
        else
			cout << "Couldn't set texture " << texFile << " on " << VARNAME(texVar) << "." << endl;
    }
    else
		cout << "Couldn't open TGA texture " << texFile << "." << endl;

}

void drawPlane(int w, int h, float nx, float ny, float nz)
/* Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/spots.c */
{
  int i, j;
  float dw = 1.0 / w;
  float dh = 1.0 / h;

  glNormal3f(nx,ny,nz);
  for (j = 0; j < h; ++j) {
    glBegin(GL_TRIANGLE_STRIP);
    for (i = 0; i <= w; ++i) {
      glVertex2f(dw * i, dh * (j + 1));
      glVertex2f(dw * i, dh * j);
    }
    glEnd();
  }
}
