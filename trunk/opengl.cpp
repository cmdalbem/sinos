#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "lib/glm.h"
#include "lib/imageloader.h"
#include "Object.h"
#include "ObjectModel.h"
#include "constants.h"


float 	xpos = 0, 
		ypos = 0,
		zpos = 0, 
		xrot = 0, 
		yrot = 0, 
		angle= 0.0;
		
		
GLuint	_textureId;
		
GLfloat posit=0; //iterational position for the SUN

struct point
{
	float x,y,z;
};



// Objects
point star[NSTARS];
ObjectModel tableTop("obj/pooltable_table.obj");
ObjectModel tableStruct("obj/pooltable_struct.obj");
ObjectModel stick("obj/taco.obj");


void initObjects (void)
{
	tableStruct.setMaterialDiffuse(0.25,0.09,0.07);
	tableStruct.setMaterialSpecular(0.1,0.1,0.1);
	tableStruct.setSize(5,5,5);
	
	tableTop.setMaterialDiffuse(0.078, 0.66, 0.078);
	tableTop.setSize(5,5,5);
	
	stick.setMaterialDiffuse(RGB(238),RGB(221),RGB(195));
	stick.setMaterialSpecular(0.1,0.1,0.1);
	stick.setPos(0,1.675,0);
	stick.setRot(0,90,0);
	stick.setSize(0.5,0.5,0.5);
	

    for (int i=0;i<NSTARS;i++)
    {
    	star[i].x = rand()%100 - 50;
    	star[i].y = rand()%100 - 50;
    	star[i].z = rand()%100 - 50;
    }
}

void drawObjects (void) {
    float mcolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	
    
    //Object ball(0,0,0, 0,0,0, 1,1,1);
    
    // stars
	glMaterialfv(GL_FRONT, GL_EMISSION, mcolor);
    
    for (int i=0;i<NSTARS;i++)
    {
    	glPushMatrix();
    	glTranslated(star[i].x *20, star[i].y *20 , star[i].z *20);
	    glutSolidSphere(0.5,10,10);
    	glPopMatrix();
    }
    
    // sun
	glPushMatrix();
    glTranslated(cos(posit/VFACTOR)*DFACTOR -DFACTOR/2, sin(posit/VFACTOR)*DFACTOR -DFACTOR/2, cos(posit/VFACTOR)*DFACTOR -DFACTOR/2);
	glutSolidSphere(0.5,10,10);
    glPopMatrix(); 
    
    glMaterialfv(GL_FRONT, GL_EMISSION, default_emission);
    
    
    
    // FLOOR
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	
    glBegin(GL_POLYGON);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.5f);
		glVertex3i(6,  0, 0);
		glTexCoord2f(0.66666666, 1.0f);
		glVertex3i(2,  0, 3);
		glTexCoord2f(0.33333333, 1.0f);
		glVertex3i(-2, 0, 3);
		glTexCoord2f(0.0f, 0.5f);
		glVertex3i(-6, 0, 0);
		glTexCoord2f(0.33333333, 0.0f);
		glVertex3i(-2, 0, -3);
		glTexCoord2f(0.66666666, 0.0f);
		glVertex3i(2,  0, -3);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
    
    
    
	tableStruct.draw();
	tableTop.draw();
	
	stick.draw();

	

	// ball
	glPushMatrix();
	glTranslated(0,1.475,0);
	//glutSolidTeapot(0.5);
	GLfloat spec[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat dif[] = {0.6, 0.6, 0.6, 1.0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glutSolidSphere(0.05,100,100);
	glPopMatrix(); 
	glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
}

void init (void) {
	initObjects();
	
    glClearColor(0, 0, 0, 1.0f); //background color
    // Limpa a janela e habilita o teste para eliminar faces ocultas por outras
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
	
	// ambient light
	GLfloat ambientColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	// lamp
	GLfloat lightColor1[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat lightPos1[] = {0.0f, 2.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	
	// position light (sun)
	GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0f };
	GLfloat diffuseLight[] = { 1.0f, 0.6f, 0.6f, 1.0f };
	GLfloat specularLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glEnable(GL_NORMALIZE);

	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_LIGHT1);
    glShadeModel (GL_SMOOTH);
    glEnable (GL_NORMALIZE);
    
    
    Image* image = loadBMP("tiger.bmp");
	_textureId = loadTexture(image);
	delete image;
}

void camera (void) {
	
	//gluLookAt(cos(posit/VFACTOR)*DFACTOR -DFACTOR/2, sin(posit/VFACTOR)*DFACTOR -DFACTOR/2, cos(posit/VFACTOR)*DFACTOR -DFACTOR/2, 0, 0, 0, 0, 1, 0);
    
    //gluLookAt(0, 3, -8, 0, 0, 0, 0, 1, 0);
	
	glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera

}

void display (void) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    
    
    // SUN
	posit++;
	GLfloat position[] = { cos(posit/VFACTOR)*DFACTOR -DFACTOR/2, sin(posit/VFACTOR)*DFACTOR -DFACTOR/2, cos(posit/VFACTOR)*DFACTOR -DFACTOR/2, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);    
    
    // AXIS
    glBegin(GL_LINES);
       // x = read, y = green, z = blue
       glColor3f(1.0f, 0.0f, 0.0f);
       glVertex3i(-100,0,0);
       glVertex3i(100,0,0);
       
       glColor3f(0.0f, 1.0f, 0.0f);
       glVertex3i(0, -100,0);
       glVertex3i(0, 100,0);
       
       glColor3f(0.0f, 0.0f, 1.0f);
       glVertex3i(0,0,-100);
       glVertex3i(0,0,100);
    glEnd();
    

    glColor3f(1.0f, 1.0f, 1.0f); //white
    
	glLoadIdentity();  
    
    camera();
   
    drawObjects();
    
    
    
    glutSwapBuffers(); //swap the buffers
    angle++; //increase the angle
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h); 	//set the viewport to the current window specifications
    glMatrixMode (GL_PROJECTION);				//set the matrix to projection
    
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 2000.0); 	//set the perspective
    															//(angle of sight, width, height, depth)
    
    //glOrtho(-5, 5, -5, 5, 0.1f, 200);		//test to check if it works =D
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model

}

void keyboard (unsigned char key, int x, int y) {
    if (key=='z')
    {
	    xrot += 3;
	    if (xrot >360)
	    	xrot -= 360;
    }

    if (key=='q')
    {
	    xrot -= 3;
    	if (xrot < -360)
    		xrot += 360;
    }

    if (key=='w')
    {
	    float xrotrad, yrotrad;
    	yrotrad = (yrot / 180 * 3.141592654f);
	    xrotrad = (xrot / 180 * 3.141592654f);
	    xpos += float(sin(yrotrad)) ;
	    zpos -= float(cos(yrotrad)) ;
	    ypos -= float(sin(xrotrad)) ;
    }

    if (key=='s')
    {
	    float xrotrad, yrotrad;
	    yrotrad = (yrot / 180 * 3.141592654f);
	    xrotrad = (xrot / 180 * 3.141592654f);
	    xpos -= float(sin(yrotrad));
	    zpos += float(cos(yrotrad)) ;
	    ypos += float(sin(xrotrad));
    }
    
    if( key=='e')
    {
		ypos += 2;
	}
	
	if( key=='c')
    {
		ypos -= 2;
	}

    if (key=='d')
    {
	    yrot += 3;
	    if (yrot >360)
	    	yrot -= 360;
    }

    if (key=='a')
    {
	    yrot -= 3;
    	if (yrot < -360)
    		yrot += 360;
    }
    
    if (key=='l')
    {
		static bool off=false;
		if(off)
			glEnable(GL_LIGHT1);
		else
			glDisable(GL_LIGHT1);
		off=!off;
	}
    
    if (key==27)
    {
	    exit(0);
    }
}







int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); //set the display to Double buffer, with depth
    glutInitWindowSize (1000, 1000);                  //set the window size
    glutInitWindowPosition (400, 0);              //set the position of the window
    glutCreateWindow ("SiNoS");     //the caption of the window
    init();
    glutDisplayFunc (display); 						//use the display function to draw everything
    
    
    glutIdleFunc (display);							// update any variables in display, display can be changed to anyhing,
                            						// as long as you move the variables to be updated, in this case, angle++;
    glutReshapeFunc (reshape);
    
    glutKeyboardFunc( keyboard );
    
    glutMainLoop(); 
    
    
    return 0;
} 
