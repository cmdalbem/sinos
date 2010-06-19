#include <iostream>
using namespace std;
#include <math.h>
#include <string.h>

#include <GL/gl.h>
#include "lib/Texture.h"
#include "Camera.h"
#include "constants.h"
#include "utils.h"



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
        
        glEnable(GL_TEXTURE_2D);
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

void drawOsd( char osd[], Camera camera, long int fps )
/* Draws On-Screen Display */
/* Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/bitfont.c */
{
  glDisable(GL_LIGHTING);
  
  sprintf(osd,"FPS: %li   camera mode: %s",fps,camera.getMode());
  
  glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos3f(-1, 0.5, -1);
  for (int i = 0; i < (int)strlen(osd); i++) 
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, osd[i]);
   
  glEnable(GL_LIGHTING);
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
