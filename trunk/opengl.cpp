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
#include "Camera.h"
#include "constants.h"

						 

#define VFACTOR 100
#define DFACTOR 5
#define SUN_MOVEMENT_EQUATION sin(posit/VFACTOR)*DFACTOR, 7 , cos(posit/VFACTOR)*DFACTOR


// Objects
point star[NSTARS];
vector<Object*> objects; /* The ideal is that, in the end, there 
						  * will be no drawable object that isn't in this vector.
						  */
Camera camera;
						  
						  
// !! The following globals aren't in their proper place. !!

//mouse
static int	xold, yold;		
static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;
		
GLuint tigerTexture;
		
bool light1=true, light2=true, light3=true;

float posit=0; //iterational position for the SUN

long int 	frameCounter, fps = 0;	//frames per second counter and register
char 		osd[1024]; 				//text buffer for on-screen output

int lensAngle = 60;




void drawOsd()
/* Draws On-Screen Display */
/* Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/bitfont.c */
{
  glDisable(GL_LIGHTING);
  
  sprintf(osd,"FPS: %li   camera mode: %s   l1/l2/l3: %i %i %i",fps,camera.getMode(),light1,light2,light3);
  
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
	static ObjectModel tableStruct("obj/pooltable_struct2x.obj");
	static ObjectModel stick("obj/taco4x.obj");
	static ObjectModel wall("obj/wall.obj");
	static ObjectBall  ball(0.1,100,100);	
	static ObjectModel light("obj/light1.obj");
	static ObjectModel crypt("obj/crypt.obj");
	
	// crypt scenario
	crypt.setPos(0,-2,0);
	crypt.setSize(25,35,25);
	crypt.material.setDiffuse(0.4,0.4,0.4);
	//crypt.material.setSpecular(0.2,0.2,0.2);
	crypt.material.setShininess(80);
	objects.push_back(&crypt);
	

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
	
	// ceiling lamp
	light.setPos(0,10,0);
	light.setSize(5,5,5);
	light.material.setDiffuse(0.5,0.5,0.5);
	light.material.setSpecular(1,1,1);
	light.material.setShininess(120);
	//light.material.setEmission(RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4);
	objects.push_back(&light);
	
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
	wall.material.setDiffuse(0.4,0.4,0.4);
	wall.material.setSpecular(0.7,0.7,0.7);
	wall.material.setShininess(120);
	wall.setPos(0,20,0);
	wall.setSize(20,20,20);
	wall.rotate(90,0,0);
	//objects.push_back(&wall);
	

    for (int i=0;i<NSTARS;i++)
    {
    	star[i].x = rand()%100 - 50;
    	star[i].y = rand()%100 - 50;
    	star[i].z = rand()%100 - 50;
    }
}

void drawAxis()
{
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

    
    //drawAxis();  
    
    glEnable(GL_LIGHTING); //ends drawing of not-lighted objects
    
    
    
    // tiger carpet
    //glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, tigerTexture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glBegin(GL_POLYGON);
		//glColor3f(1.0f, 1.0f, 1.0f);
		//glTexCoord2f(1.0f, 0.5f);
		//glVertex3i(6,  0, 0);
		//glTexCoord2f(0.66666666, 1.0f);
		//glVertex3i(2,  0, 3);
		//glTexCoord2f(0.33333333, 1.0f);
		//glVertex3i(-2, 0, 3);
		//glTexCoord2f(0.0f, 0.5f);
		//glVertex3i(-6, 0, 0);
		//glTexCoord2f(0.33333333, 0.0f);
		//glVertex3i(-2, 0, -3);
		//glTexCoord2f(0.66666666, 0.0f);
		//glVertex3i(2,  0, -3);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
	
	
	// iterational plan drawing test (work in progress)
	//Material wallMaterial;
	//wallMaterial.setDiffuse(0.4,0.4,0.4);
	//wallMaterial.setSpecular(0.7,0.7,0.7);
	//wallMaterial.setShininess(120);
    //wallMaterial.apply();
	//glPushMatrix();
	//glTranslated(-15, 0, -15);
	//glRotatef(90,1,0,0);
	//glScalef(30,30,30);
	////glTranslated(0,0,0);
	//drawPlane(100,100, 0,0,-1);
	//wallMaterial.unapply();
	//glPopMatrix();
	
	// draw objects
	vector<Object*>::iterator ob_it;
	for( ob_it=objects.begin(); ob_it<objects.end(); ob_it++ )
		(*ob_it)->draw();
}


void lights () {
	
	// ambient light
	GLfloat ambientColor[] = {0.0, 0.0, 0.0, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	// position light (sun)
	GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0f };
	GLfloat diffuseLight[] = { 0.9f, 0.5f, 0.5f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.002);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.004);
	
	// spotlight
	GLfloat lampColor[] = {RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4, 1.0f};
	GLfloat light1_position[] = { 0.0, 10.0, 0.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lampColor);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.2);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.04);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 12.0);
	
	// directional light
	GLfloat lampColor2[] = {RGB(252), RGB(234), RGB(186), 1.0f};
	GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lampColor2);
	glLightfv(GL_LIGHT2, GL_POSITION, direction);
	
	// spotlight 2
	//GLfloat light2_position[] = { 0.0, 4.0, 0.0, 1.0 };
	//GLfloat spot2_direction[] = { 0.0, 1.0, 0.0 };
	//GLfloat shin = 120;
	//glLightfv(GL_LIGHT3, GL_DIFFUSE, lampColor2);
	//glLightfv(GL_LIGHT3, GL_SPECULAR, lampColor2);
	//glLightfv(GL_LIGHT3, GL_SHININESS, &shin);
	//glLightfv(GL_LIGHT3, GL_POSITION, light2_position);
	//glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 13);
	//glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot2_direction);
	//glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0);	
}


void display () {

	int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode (GL_PROJECTION); //set the matrix to projection
	glLoadIdentity ();
	
    glViewport (0, 0, (GLsizei)width, (GLfloat)height);
    gluPerspective (lensAngle, (GLfloat)width / (GLfloat)height, 0.1, 2000.0);
    					
    glMatrixMode (GL_MODELVIEW);  //set the matrix back to model

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // SUN
	posit++;
	GLfloat position[] = { SUN_MOVEMENT_EQUATION, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);    
    
	glLoadIdentity();  
    // IMPORTANT: these calls aren't in arbitrary order.
		drawOsd();
		camera.apply(objects[5]);
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
    if(light1)
		glEnable (GL_LIGHT0); 	// sun
    if(light2)
		glEnable (GL_LIGHT1);   // spotlight
    if(light3)
		glEnable (GL_LIGHT2); 	// directional
	glEnable (GL_LIGHT3);
    glShadeModel (GL_SMOOTH);
    
    // TODO: find better place for this
    Image* image = loadBMP("textures/tiger.bmp");
	tigerTexture = loadTexture(image);
	delete image;
}

void reshape (int w, int h) {
    /*
    glMatrixMode (GL_PROJECTION); //set the matrix to projection
    
	glLoadIdentity ();
	
    glViewport (0, 0, (GLsizei)w, (GLfloat)h);
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 2000.0);
    					
    glMatrixMode (GL_MODELVIEW);  //set the matrix back to model
    */
}

void keyboardFunc (unsigned char key, int x, int y) {
   
    if ( key=='s' )
		lensAngle+=2;
	if ( key=='w' )
		lensAngle-=2;
    
    if( key=='c' )
		camera.nextCameraMode();
    
	// turn on/off light0
	if (key == '1') {
		if(!light1)
			glEnable(GL_LIGHT0);
		else
			glDisable(GL_LIGHT0);
		light1 = !light1;
	}
	
	// turn on/off light1
	if (key == '2') {
		if(!light2)
			glEnable(GL_LIGHT1);
		else
			glDisable(GL_LIGHT1);
		light2 = !light2;
	}
	
    // turn on/off light2
    if (key == '3') {
		if(!light3)
			glEnable(GL_LIGHT2);
		else
			glDisable(GL_LIGHT2);
		light3 = !light3;
	}
    
    if (key==K_ESC)
    {
	    exit(0);
    }
}



void mouseFunc(int button, int state, int x, int y) {
/* This funciont only updates click states and positions */
 
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
		camera.action1(x - xold, y - yold);
    }
	
	if (GLUT_DOWN == right_click)
	{
		camera.action2(x - xold, y - yold);
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
