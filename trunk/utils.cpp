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


/* The euclidian norma is calculated as:
 * 
 * ((x-x0)² + (y-y0)² + (z-z0)²)^1/2,
 */

double getDistance( double x1, double y1, double z1, double x2, double y2, double z2 )
{
	return sqrt(  pow(x1-x2, 2)
				+ pow(y1-y2, 2)
				+ pow(z1-z2, 2));
}	

double getDistance( Object &object1, Object &object2 )
{
	return sqrt(  pow(object1.getPosX() - object2.getPosX(), 2)
				+ pow(object1.getPosY() - object2.getPosY(), 2)
				+ pow(object1.getPosZ() - object2.getPosZ(), 2));
}

void drawGuideLine( double xo, double yo, double zo, double stickAngle)
{
	//glColor4f(1.,1.,1.,1); 
	glLineStipple(2, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
		glVertex3f( xo, yo, zo);
		glVertex3f( xo+100*cos(RAD(stickAngle+90)), yo, zo+100*sin(RAD(-(stickAngle+90))) );
	glEnd();
	glDisable(GL_LINE_STIPPLE);	
}

void makeGuideLine( vector<ObjectBall> balls, double stickAngle)
{
	double x1 = balls[0].pos[0], 
		   y1 = TABLE_PLANE_Y, 
		   z1 = balls[0].pos[2];
	double vecx = 0.1 *  cos(RAD(stickAngle+90)),
		   vecz = 0.1 * -sin(RAD(stickAngle+90));
	double x2 = x1 + 20*vecx,
		   y2 = y1,
		   z2 = z1 + 20*vecz;		   	
	double x3, y3=y2, z3;
	bool rebound = false;
	
	// Find rebound point, if any
	for(int i=0; i<400 && !rebound; i++) {
		x2 += vecx;
		z2 += vecz;
		for( int b=0; b<balls.size(); b++ ) {
			if( getDistance(balls[b].pos[0],balls[b].pos[1],balls[b].pos[2], x2,y2,z2) < 2*BALL_RADIUS )
			{
				x3 = x2 + 10 * ( balls[b].pos[0]-x2 );
				z3 = z2 + 10 * ( balls[b].pos[2]-z2 );
				rebound = true;
			}
		}
	}	
	
	// Draw	
	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_STIPPLE);
	//glEnable(GL_LINE_SMOOTH);
		glLineStipple(2, 0xAAAA);
		glBegin(GL_LINES);
			glColor3f(1.,1.,1.); 
			glVertex3f( x1, y1, z1);
			glVertex3f( x2, y2, z2);
		glEnd();
		if( rebound ) {
			glBegin(GL_LINES);
				glVertex3f( x2, y2, z2);
				glVertex3f( x3, y3, z3);
			glEnd();	
		}
	glDisable(GL_LINE_STIPPLE);
	glDisable(GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);
}

double getRandBetween(int a, int b)
{
	static bool init=true;
	if(init){
		srand(time(0));
		init=false;
	}

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

double getVectorNorma( const double x, const double y )
{
	return sqrt( pow(x,2) + pow(y,2) );	
}

double getVectorAngle( const double v[3] )
{
	return getVectorAngle( v[0], v[2] );
}	

double getVectorAngle( const double x, const double y )
{
	double magnitude = getVectorNorma(x,y);
	if( magnitude ) {
		float direction = DEGREES(acosf(x/magnitude)); //angle of move vector = arc cos x/hypotenuse
		
		if( (y/magnitude) > 0 )
			direction += 2*(180-direction);
			
		return direction;
	}
	else
		return 0;
}

double dotProduct( const double v[3], const double u[3] )
{
       return v[0]*u[0] + v[2]*u[2];
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
