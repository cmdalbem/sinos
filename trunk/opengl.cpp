#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <string.h>

#include "lib/glm.h"
#include "lib/imageloader.h"
#include "Object.h"
#include "ObjectModel.h"
#include "ObjectBall.h"
#include "constants.h"

#define VFACTOR 100
#define DFACTOR 5
#define SUN_MOVEMENT_EQUATION sin(posit/VFACTOR)*DFACTOR, 7 , cos(posit/VFACTOR)*DFACTOR
#define SPIRAL_MOVEMENT_EQUATION cos(posit/VFACTOR)*(100000/posit), 10 , sin(posit/VFACTOR)*(100000/posit)
								 

// Objects
point star[NSTARS];
vector<Object*> objects; /* The ideal is that, in the end, there 
						  * will be no drawable object that isn't in this vector.
						  */
						  
						  

// !! The following globals aren't in their proper place. !!

//camera attributes
float 	xpos = 5, 
		ypos = 7,
		zpos = 5, 
		xrot = 45, 
		yrot = -45,
		zoom = 5;

//mouse
static int	xold, yold;		
static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;
		
GLuint tigerTexture;
		
bool l1=true, l2=false, l3=true;

float posit=0; //iterational position for the SUN

long int 	frameCounter, fps = 0;	//frames per second counter and register
char 		osd[1024]; 				//text buffer for on-screen output

/* TODO: create Camera Class */
#define NCAMERAS 3
int cameraMode=0;
char cameraModes[][30] = { "Cinematic",
						   "Manual",
						   "Ball centered"
						  };



// OSD means On-Screen Display
void drawOsd()
/* Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/bitfont.c */
{
  glDisable(GL_LIGHTING);
  
  sprintf(osd,"FPS: %li   camera mode: %s   l1/l2/l3: %i %i %i",fps,cameraModes[cameraMode],l1,l2,l3);
  
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

void initObjects () {

	// TODO: organize resolution constants so that the game may
	//       have configuration of graphics performance.
	static ObjectModel tableTop("obj/pooltable_table16x.obj");
	static ObjectModel tableStruct("obj/pooltable_struct.obj");
	static ObjectModel stick("obj/taco4x.obj");
	static ObjectModel wall1("obj/wall.obj");
	static ObjectModel wall2("obj/wall.obj");
	static ObjectModel wall3("obj/wall.obj");
	static ObjectModel wall4("obj/wall.obj");
	static ObjectBall  ball(0.1,100,100);	

	// table
	tableStruct.material.setDiffuse(0.25,0.09,0.07);
	tableStruct.material.setSpecular(0.3,0.3,0.3);
	tableStruct.material.setShininess(120);
	tableTop.material.setDiffuse(0.078 *0.75, 0.66 *0.75, 0.078 *0.75);
	tableTop.material.setSpecular(0.1,0.1,0.1);
	tableTop.material.setShininess(40);
	tableStruct.setSize(10,10,10);
	tableTop.setSize(10,10,10);
	objects.push_back(&tableStruct);
	objects.push_back(&tableTop);
	
	// the stick
	stick.material.setDiffuse(RGB(238),RGB(221),RGB(195));
	stick.material.setSpecular(0.3,0.3,0.3);
	stick.material.setShininess(80);
	stick.setPos(0.2,3,0);
	stick.setRot(30,330,0); // aehoo não consigo fazer isso aqui funcionar!!
	stick.setSize(0.5,0.5,0.5);
	objects.push_back(&stick);
	
	// the ball
	ball.setPos(0,2.95,0);
	ball.material.setShininess(120);
	ball.material.setDiffuse(0.6, 0.6, 0.6);
	ball.material.setSpecular(0.9, 0.9, 0.9);
	objects.push_back(&ball);
	
	// walls
	Material wallMaterial;
	wallMaterial.setDiffuse(0.4,0.4,0.4);
	wallMaterial.setSpecular(0.7,0.7,0.7);
	wallMaterial.setShininess(120);
	wall1.loadMaterial(wallMaterial);
	wall2.loadMaterial(wallMaterial);
	wall3.loadMaterial(wallMaterial);
	wall4.loadMaterial(wallMaterial);
	int wallH = 20;
	wall1.setPos(0,0,20);
	wall1.setSize(20,wallH,20);
	wall2.setPos(0,0,-20);
	wall2.setSize(20,wallH,20);
	wall2.rotate(0,180,0);
	wall3.setPos(-20,0,0);
	wall3.setSize(20,wallH,20);
	wall3.rotate(0,-90,0);
	wall4.setPos(0,-20,0);
	wall4.setSize(20,20,20);
	wall4.rotate(90,0,0);
	objects.push_back(&wall1);
	objects.push_back(&wall2);
	objects.push_back(&wall3);
	objects.push_back(&wall4); //floor
	

    for (int i=0;i<NSTARS;i++)
    {
    	star[i].x = rand()%100 - 50;
    	star[i].y = rand()%100 - 50;
    	star[i].z = rand()%100 - 50;
    }
}

void drawObjects () {
    	
    glDisable(GL_LIGHTING); //stars drawing of not-lighted objects
    // stars
    for (int i=0;i<NSTARS;i++)
    {
    	glPushMatrix();
    	glTranslated(star[i].x *20, star[i].y *20 , star[i].z *20);
	    glutSolidSphere(0.5,10,10);
    	glPopMatrix();
    }
    // sun
	glPushMatrix();
    glTranslated( SUN_MOVEMENT_EQUATION );
	glutSolidSphere(0.5,10,10);
    glPopMatrix(); 

    // axis
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
    
    glEnable(GL_LIGHTING); //ends drawing of not-lighted objects
    
    
    
    //// FLOOR
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tigerTexture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
	
	
	// iterational plan drawing test (work in progress)
	Material wallMaterial;
	wallMaterial.setDiffuse(0.4,0.4,0.4);
	wallMaterial.setSpecular(0.7,0.7,0.7);
	wallMaterial.setShininess(120);
    wallMaterial.apply();
	glPushMatrix();
	glTranslated(-20, 20, -20);
	glRotatef(90,1,0,0);
	glScalef(40,40,40);
	glTranslated(0,0,0);
	drawPlane(50,50, 0,0,1);
	wallMaterial.unapply();
	glPopMatrix();
	
	// draw objects
	vector<Object*>::iterator ob_it;
	for( ob_it=objects.begin(); ob_it<objects.end(); ob_it++ )
		(*ob_it)->draw();
}

void camera () {

	//gluLookAt(0, 3, -8, 0, 0, 0, 0, 1, 0);
	
	// orbital movement (remember to disable drawing of the sun)
	//gluLookAt( SUN_MOVEMENT_EQUATION, 0,0,0, 0,1,0);
	
	switch( cameraMode )
	{
		case 0:
			// cinematic dramatic camera
			gluLookAt( SPIRAL_MOVEMENT_EQUATION,
					   objects[3]->getPosX(), objects[3]->getPosY(), objects[3]->getPosZ(), /*ball position*/
					   0,1,0);
			break;
		case 1:
			// controlled movement
			glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
			glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
			glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
			break;
		
		case 2:
			// ball centered camera
			float orig[3] = { objects[3]->getPosX(), objects[3]->getPosY(), objects[3]->getPosZ() }; //ball positions
			
			gluLookAt( orig[0] + sin(xrot*0.1)*sin(-yrot*0.1)*zoom, orig[1] + cos(xrot*0.1)*zoom, orig[2] + cos(-yrot*0.1)*sin(xrot*0.1)*zoom,
					   orig[0], orig[1], orig[2], /*ball position*/
					   //SUN_MOVEMENT_EQUATION, /*sun position*/					   
					   0,1,0);
			break;
	}

}

void lights () {
	
	// ambient light
	GLfloat ambientColor[] = {0, 0, 0, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	// position light (sun)
	GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0f };
	GLfloat diffuseLight[] = { 1.0f, 0.6f, 0.6f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	
	// directional light
	GLfloat lampColor[] = {RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4, 1.0f};
	GLfloat direction[] = {0.0f, 1.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lampColor);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lampColor);
	glLightfv(GL_LIGHT2, GL_POSITION, direction);
	
	// spotlight
	GLfloat light1_position[] = { 0.0, 10.0, 0.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lampColor);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.7);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);	
}

void display () {

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // SUN
	posit++;
	GLfloat position[] = { SUN_MOVEMENT_EQUATION, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);    
    
	glLoadIdentity();  
    // IMPORTANT: these calls aren't in arbitrary order.
		drawOsd();
		camera();
		drawObjects();
		lights();    	   
    
    glutSwapBuffers();
    frameCounter++;
}

void init () {
	
	initObjects();
	
    glClearColor(0, 0, 0, 1.0f); //background color
    // Limpa a janela e habilita o teste para eliminar faces ocultas por outras
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_NORMALIZE); //normalizes all normals
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0); 	// sun
    glEnable (GL_LIGHT1);   // spotlight
    glEnable (GL_LIGHT2); 	// directional
    glShadeModel (GL_SMOOTH);
    
    // TODO: find better place for this
    Image* image = loadBMP("textures/tiger.bmp");
	tigerTexture = loadTexture(image);
	delete image;
}

void reshape (int w, int h) {
    glMatrixMode (GL_PROJECTION); //set the matrix to projection
    
	glLoadIdentity ();
	
    glViewport (0, 0, (GLsizei)w, (GLfloat)h);
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 2000.0);
    					
    glMatrixMode (GL_MODELVIEW);  //set the matrix back to model
}

void keyboardFunc (unsigned char key, int x, int y) {
   
    if( key=='c' )
		cameraMode = (cameraMode+1)%NCAMERAS;
    
    // move camera up
    if( key=='w')
		ypos += 0.2;
	
	// move camera down
	if( key=='s')
		ypos -= 0.2;
    
	// turn on/off light0
	if (key == '1') {
		if(!l1)
			glEnable(GL_LIGHT0);
		else
			glDisable(GL_LIGHT0);
		l1=!l1;
	}
	
	// turn on/off light1
	if (key == '2') {
		if(!l2)
			glEnable(GL_LIGHT1);
		else
			glDisable(GL_LIGHT1);
		l2=!l2;
	}
	
    // turn on/off light2
    if (key == '3') {
		if(!l3)
			glEnable(GL_LIGHT2);
		else
			glDisable(GL_LIGHT2);
		l3=!l3;
	}
    
    if (key==K_ESC)
    {
	    exit(0);
    }
}



void mouseFunc(int button, int state, int x, int y) {
 
  if (GLUT_LEFT_BUTTON == button)
    left_click = state;
  if (GLUT_RIGHT_BUTTON == button)
    right_click = state;
  xold = x;
  yold = y;
}

void mouseMotionFunc(int x, int y) {
	
	if (GLUT_DOWN == left_click)
    {
		xrot = xrot + (y - yold) / 5.f;
		yrot = yrot + (x - xold) / 5.f;
		glutPostRedisplay();
    }
	
	if (GLUT_DOWN == right_click)
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * M_PI);
		xrotrad = (xrot / 180 * M_PI);
		xpos += (yold - y)/10.0 * float(sin(yrotrad)) ;
		zpos -= (yold - y)/10.0 * float(cos(yrotrad)) ;
		ypos -= (yold - y)/10.0 * float(sin(xrotrad)) ;
		glutPostRedisplay();
		
		zoom += (y - yold) / 5.f;
		if(zoom<1) zoom=1;		// do not let reverse the camera
	}

	xold = x;
	yold = y;
}

void timerFunc(int value) {
	fps = frameCounter;
	frameCounter = 0;
	
	glutTimerFunc(1000/*1sec*/, timerFunc, 0);
}






int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); //set the display to Double buffer, with depth
    glutInitWindowSize (1280, 720);                  //set the window size
    glutInitWindowPosition (400, 0);              //set the position of the window
    glutCreateWindow ("SiNoS - 1/2/3: lights  mouse/w/s: camera  c: mode");     //the caption of the window
    init();
    glutDisplayFunc (display); 						//use the display function to draw everything
    
    
    glutIdleFunc (display);							// update any variables in display, display can be changed to anyhing,
                            						// as long as you move the variables to be updated, in this case, angle++;
    glutReshapeFunc (reshape);
    
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	glutTimerFunc(1000/*1sec*/, timerFunc, 0);
  
    glutMainLoop(); 
    
    
    return 0;
} 
