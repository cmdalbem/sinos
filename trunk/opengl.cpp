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
#include "lib/Texture.h"

#include "Object.h"
#include "ObjectModel.h"
#include "ObjectBall.h"
#include "ObjectStick.h"
#include "Camera.h"
#include "constants.h"

						 

#define VFACTOR 100
#define DFACTOR 5
#define SUN_MOVEMENT_EQUATION sin(posit/VFACTOR)*DFACTOR, 7 , cos(posit/VFACTOR)*DFACTOR


// Objects
point star[NSTARS];
vector<Object*> objects;

//texturized objects
static ObjectModel tableStruct("obj/pooltable_struct.obj");
static ObjectModel tableTop("obj/pooltable_table16x.obj");
static ObjectModel scenario("obj/crypt.obj");
static ObjectModel globe("obj/globe.obj");
//static ObjectModel ball("obj/poolball.obj");
static ObjectBall  ball(0.1,100,100);
static ObjectStick stick("obj/taco.obj", &ball);

// Texture files
Texture tigerTex, woodTex, tableTex, rockTex, starsTex, stickTex, ballTex;


Camera camera,
	   camera2(0, 5, 0, 0, 90, 0);
						  
// mouse
static int	xold, yold;		
static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;

bool invertViewports=true;

						  

// !! The following globals aren't in their proper place. !!

bool 		light1=true, light2=true, light3=true, light4=true;
float 		posit=0;
long int 	frameCounter, fps = 0;	//frames per second counter and register
char 		osd[1024]; 				//text buffer for on-screen output
int 		lensAngle = 60;
int 		cursor = 0;


//--------------------------------------------------------------------//
//----------------------------- UTILS --------------------------------//
//--------------------------------------------------------------------//

void loadTexture(Texture *texVar, string texFile, bool makeMipmap=false)
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



//--------------------------------------------------------------------//
//----------------------------- OPENGL -------------------------------//
//--------------------------------------------------------------------//

void initObjects () {

	// TODO: organize resolution variations so that the game may
	//       have configuration of graphics performance.
	static ObjectModel light("obj/light1.obj");
	static ObjectModel wall("obj/wall.obj");	
	static ObjectModel tableStruct2("obj/pooltable_struct.obj");
	static ObjectModel tableTop2("obj/pooltable_table16x.obj");
	static ObjectModel tableStruct3("obj/pooltable_struct.obj");
	static ObjectModel tableTop3("obj/pooltable_table16x.obj");
	static ObjectModel tableStruct4("obj/pooltable_struct.obj");
	static ObjectModel tableTop4("obj/pooltable_table16x.obj");
	
	// the ball
	ball.setPos(0,2.95,0);
	ball.material.setShininess(120);
	ball.material.setDiffuse(0.6, 0.6, 0.6);
	ball.material.setSpecular(0.9, 0.9, 0.9);
	ball.setTexture(&ballTex);
		objects.push_back(&ball);
	
	// the stick
	stick.material.setDiffuse(RGB(238),RGB(221),RGB(195));
	stick.material.setSpecular(0.3,0.3,0.3);
	stick.material.setShininess(80);
	stick.setTexture(&stickTex);
	stick.setSize(0.7,0.7,0.8);
	stick.rotateLeft();
	//	objects.push_back(&stick);
	
	// crypt scenario
	scenario.setPos(0,-3,0);
	scenario.setSize(25,35,25);
	scenario.setTexture(&rockTex);
	scenario.material.setDiffuse(0.4,0.4,0.4);
	//scenario.material.setSpecular(0.2,0.2,0.2);
	//scenario.material.setShininess(80);
		objects.push_back(&scenario);
	
	// tables material setup
	Material tableStructMat;
	tableStructMat.setDiffuse(0.5 *1.5, 0.18 *1.5, 0.14 *1.5);
	tableStructMat.setSpecular(0.3,0.3,0.3);
	tableStructMat.setShininess(120);
	Material tableTopMat;
	tableTopMat.setDiffuse(0.078 , 0.66 , 0.078 );
	tableTopMat.setSpecular(0.1,0.1,0.1);
	tableTopMat.setShininess(40);
	
	// main table
	tableStruct.setMaterial(tableStructMat);
	tableTop.setMaterial(tableTopMat);
	tableStruct.setTexture(&woodTex);
	tableTop.setTexture(&tableTex);
	tableStruct.setSize(10,10,10);
	tableTop.setSize(10,10,10);
		objects.push_back(&tableTop); objects.push_back(&tableStruct);

	// table2
	tableStructMat.setDiffuse(0.25,0.09,0.07);
	tableStructMat.setSpecular(0,0,0);
	tableTopMat.setDiffuse(0.078 *0.5, 0.66 *0.5, 0.078 *0.5);	
	tableStruct2.setMaterial(tableStructMat);
	tableTop2.setMaterial(tableTopMat);
	tableStruct2.setSize(10,10,10);
	tableTop2.setSize(10,10,10);
	tableStruct2.setPos(0,0,-30);
	tableTop2.setPos(0,0,-30);
	// table3
	tableStruct3.setMaterial(tableStructMat);
	tableTop3.setMaterial(tableTopMat);
	tableStruct3.setSize(10,10,10);
	tableTop3.setSize(10,10,10);
	tableStruct3.setPos(0,0,30);
	tableTop3.setPos(0,0,30);
	// table4
	tableStruct4.setMaterial(tableStructMat);
	tableTop4.setMaterial(tableTopMat);
	tableStruct4.setSize(10,10,10);
	tableTop4.setSize(10,10,10);
	tableStruct4.setPos(0,0,60);
	tableTop4.setPos(0,0,60);
		objects.push_back(&tableStruct2);objects.push_back(&tableTop2);
		objects.push_back(&tableStruct3);objects.push_back(&tableTop3);	
		objects.push_back(&tableStruct4);objects.push_back(&tableTop4);		
	
	// ceiling lamp
	light.setPos(0,11,0);
	light.setSize(5,5,5);
	light.material.setDiffuse(0.5,0.5,0.5);
	light.material.setSpecular(1,1,1);
	light.material.setShininess(120);
	//light.material.setEmission(RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4);
		objects.push_back(&light);

	
	// infinite scenario globe
	globe.setSize(50,50,50);
	globe.setTexture(&starsTex);
		objects.push_back(&globe);
	

    for (int i=0;i<NSTARS;i++)
    {
    	star[i].x = rand()%50 - 25;
    	star[i].y = rand()%50 - 25;
    	star[i].z = rand()%50 - 25;
    }
}

void drawObjects_partial () {
	
	objects[0]->draw();
	stick.draw();
	tableStruct.draw();
	tableTop.draw();
}

void drawObjects () {
    	
    glDisable(GL_LIGHTING); //stars drawing of not-lighted objects
	    // Stars
	    for (int i=0;i<NSTARS;i++) {
	    	glPushMatrix();
	    	glTranslated(star[i].x *20, star[i].y *20, star[i].z *20);
		    glutSolidSphere(0.5,10,10);
	    	glPopMatrix();
	    }
	    //// Sun
		//glPushMatrix();
	    //glTranslated( SUN_MOVEMENT_EQUATION );
		//glutSolidSphere(0.5,10,10);
	    //glPopMatrix();
		// Infinite Background
		globe.draw();
	glEnable(GL_LIGHTING); //ends drawing of not-lighted objects
	
	
	// ball cursor
	glPushMatrix();
    glTranslated( ball.getPosX() + cos(RAD(cursor))*0.3, ball.getPosY() , ball.getPosZ() + sin(RAD(cursor))*0.3 );
    glRotated( -cursor, 0,1,0);
	glScalef(0.1,0.1,0.1);
	glutSolidTetrahedron();
    glPopMatrix();	    
	cursor+=5;
    
    
	// draw all objects
	for( int it=0; it<objects.size(); it++ )
		objects[it]->draw();
		
	stick.draw();
}

void lights () {
	
	// position light (sun)
	//posit++;
	//GLfloat position[] = { SUN_MOVEMENT_EQUATION, 1.0f };
	GLfloat position[] = { 0,10,0, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);    
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	
	// spotlight
	GLfloat light1_position[] = { 0.0, 10.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	
	// spotlight 2
	GLfloat light2_position[] = { 0.0, 10.0, -30.0, 1.0 };
	glLightfv(GL_LIGHT3, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	// spotlight 3
	GLfloat light3_position[] = { 0.0, 10.0, 30.0, 1.0 };
	glLightfv(GL_LIGHT4, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
	// spotlight 4
	GLfloat light4_position[] = { 0.0, 10.0, 60.0, 1.0 };
	glLightfv(GL_LIGHT5, GL_POSITION, light4_position);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction);
	
	// directional light
	GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT2, GL_POSITION, direction);
}

void perspectiveViewport( int width, int height ) {
	glMatrixMode (GL_PROJECTION); //set the matrix to projection
	glLoadIdentity ();
	
    glViewport (0, 0, (GLsizei)width, (GLfloat)height);
    glScissor(0, 0, width, height);
    gluPerspective (lensAngle, (GLfloat)width / (GLfloat)height, 0.1, 1000.0);
    					
    glMatrixMode (GL_MODELVIEW);  //set the matrix back to model
	glLoadIdentity();  
    // IMPORTANT: don't change the order of these calls
		drawOsd();
		camera.apply();
		lights();
		drawObjects();
}	

void orthoViewport( int width, int height ) {
	glViewport ((3*width)/4, (3*height)/4, (GLsizei)width/4, (GLfloat)height/4);
	
	// This call isn't working as it would
	glScissor((3*width)/4, (3*height)/4, width/4, height/4);
	
	glEnable(GL_SCISSOR_TEST);
	
	glMatrixMode (GL_PROJECTION); //set the matrix to projection
	glLoadIdentity ();
    
    glOrtho(-7, 7, -4, 3.5, 0.1, 50);
    					
    glMatrixMode (GL_MODELVIEW);  //set the matrix back to model
    glLoadIdentity();
	// IMPORTANT: don't change the order of these calls
		camera2.apply();
		lights();
		drawObjects_partial();
		
	glDisable(GL_SCISSOR_TEST);
}	

void display () {

	int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);
	
		perspectiveViewport(width,height);
		orthoViewport(width,height);
				
	glDisable(GL_SCISSOR_TEST);
	
	//cout << "distance from crypt: " << camera.distanceFromObject(*objects[1]) << endl;
		
    glutSwapBuffers();
    frameCounter++;
}

void initLights () {
	
	// ambient light
	GLfloat ambientColor[] = {0.09, 0.05, 0.05, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	
	// position light (sun)
	GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0f };
	GLfloat diffuseLight[] = { 0.9f, 0.5f, 0.5f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.4);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 160);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
	
	// spotlight
	GLfloat lampColor[] = {RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4, 1.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.2);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.04);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 12.0);
	
	// spotlight 2
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.04);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 6.0);
	// spotlight 3
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.04);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 6.0);
	// spotlight 4
	glLightfv(GL_LIGHT5, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.04);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 6.0);	
	
	// directional light
	GLfloat lampColor2[] = {RGB(252), RGB(234), RGB(186), 1.0f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lampColor2);
}

void init ()
{
	cout << "Loading models...";
	initObjects();
	cout << "Done.\n";
	
    glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_NORMALIZE);		//normalizes all normals
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glShadeModel (GL_SMOOTH);
    
    if(light1)
		glEnable (GL_LIGHT0); 	// sun
    if(light2)
		glEnable (GL_LIGHT1);   // spotlight
    if(light3)
		glEnable (GL_LIGHT2); 	// directional
		
	initLights();
	
	glEnable (GL_LIGHT3); // extra spotlight1
	glEnable (GL_LIGHT4); // extra spotlight2
	glEnable (GL_LIGHT5); // extra spotlight3
    
    cout << "Loading textures..."; 
    loadTexture(&ballTex, "textures/poolball.tga");
    loadTexture(&woodTex, "textures/wood.tga");
    loadTexture(&tableTex, "textures/table.tga", true);//set true to use mipmapping
    loadTexture(&rockTex, "textures/rock.tga", true);//set true to use mipmapping
    loadTexture(&starsTex, "textures/stars3.tga");
    loadTexture(&stickTex, "textures/stick.tga");
    cout << "Done.\n"; 
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

//--------------------------- KEYBOARD ---------------------------//
void keyboardFunc (unsigned char key, int x, int y) {
   
    if ( key=='f')
		ball.applyForce(10,cursor);   
		
    // chances lens focal distance
    if ( key=='s' )
		lensAngle+=2;
	if ( key=='w' )
		lensAngle-=2;
    
    if( key=='c' )
		camera.nextCameraMode(objects[0]);
    
    if ( key=='v' )
		invertViewports = !invertViewports;
	
	// turn on/off light0
	if ( key == '1' ) {
		if(!light1)
			glEnable(GL_LIGHT0);
		else
			glDisable(GL_LIGHT0);
		light1 = !light1;
	}
	
	// turn on/off light1
	if ( key == '2' ) {
		if(!light2)
			glEnable(GL_LIGHT1);
		else
			glDisable(GL_LIGHT1);
		light2 = !light2;
	}
	
    // turn on/off light2
    if ( key == '3' ) {
		if(!light3)
			glEnable(GL_LIGHT2);
		else
			glDisable(GL_LIGHT2);
		light3 = !light3;
	}
	
    if ( key==K_ESC )
    {
	    exit(0);
    }
}

void specialFunc(int key, int x, int y)
{
	if ( key == GLUT_KEY_LEFT )
	{
		stick.rotateLeft();
	}
	
	if ( key == GLUT_KEY_RIGHT )
	{
		stick.rotateRight();
	}
	
	if ( key == GLUT_KEY_UP )
	{
		stick.strenghtUp();
	}
	
	if ( key == GLUT_KEY_DOWN )
	{
		stick.strenghtDown();
	}
}

//--------------------------- MOUSE ---------------------------//
void mouseFunc(int button, int state, int x, int y) {
/* This function only updates click states and positions */
 
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

void updateFPS(int value) {
	fps = frameCounter;
	frameCounter = 0;
	
	glutTimerFunc(1000/*1sec*/, updateFPS, 0);
}

void updateState(int value) {
	ball.updateState();
	glutTimerFunc(1000/STATEUPDATES_PER_SEC, updateState, 0);
}




int main (int argc, char **argv) {
    cout << "Initializing...\n";
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
    glutSpecialFunc(specialFunc);
    glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	glutTimerFunc(1000/*1sec*/, updateFPS, 0);    
	glutTimerFunc(1000/STATEUPDATES_PER_SEC , updateState, 0);
	
    glutMainLoop(); 
    
    
    return 0;
} 
