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
#include "LightInfo.h"
#include "Level.h"
#include "constants.h"
#include "utils.h"
						 

// Objects
point star[NSTARS];
vector<Object*> objects;
vector<LightInfo*> theLights;

Level level(&objects, &theLights);

Camera camera, camera2(2);

//texturized objects 
static ObjectModel tableStruct("obj/pooltable_struct_noframe.obj");
static ObjectModel tableTop("obj/pooltable_table.obj");
static ObjectModel tableFrame("obj/pooltable_frame.obj");
static ObjectModel scenario("obj/crypt.obj");
static ObjectModel globe("obj/globe.obj");
//static ObjectModel ball("obj/poolball.obj");
static ObjectBall  ball(1,100,100);
static ObjectBall  cursor(1,100,100);
static ObjectStick stick("obj/taco.obj", &ball);


static ObjectModel light("obj/light1.obj");
static ObjectModel wall("obj/wall.obj");	
static ObjectModel tableStruct2("obj/pooltable_struct.obj");
static ObjectModel tableTop2("obj/pooltable_table.obj");
static ObjectModel tableStruct3("obj/pooltable_struct.obj");
static ObjectModel tableTop3("obj/pooltable_table.obj");
static ObjectModel tableStruct4("obj/pooltable_struct.obj");
static ObjectModel tableTop4("obj/pooltable_table.obj");

// Texture files
Texture tigerTex, woodTex, tableTex, rockTex, starsTex, stickTex, ballTex;

						  
// mouse-keyboard
static int	xold, yold;		
static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;
static bool holdCtrl = false;
static bool invertViewports = false;
						  

// !! The following globals aren't in their proper place. !!

float 		posit=0;
long int 	frameCounter, fps = 0;	//frames per second counter and register
char 		osd[1024]; 				//text buffer for on-screen output
int 		lensAngle = 60;
float 		radius=2.3;



//--------------------------------------------------------------------//
//----------------------------- OPENGL -------------------------------//
//--------------------------------------------------------------------//

void initObjects () {

	// the ball
	ball.setPos(0,BALL_O_Y,0);
	ball.material.setShininess(120); 
	ball.material.setDiffuse(0.6, 0.6, 0.6);
	ball.material.setSpecular(0.9, 0.9, 0.9);
	ball.setTexture(&ballTex);
		objects.push_back(&ball);		// objects[0]
	
	cursor.setPos(0,BALL_O_Y,2);
	
	// the stick
	stick.material.setDiffuse(RGB(238),RGB(221),RGB(195));
	stick.material.setSpecular(0.3,0.3,0.3);
	stick.material.setShininess(80);
	stick.setTexture(&stickTex);
	stick.setSize(7,7,8);
	stick.rotate(-5);
		objects.push_back(&stick);		//objects[1]
	
	// crypt scenario
	scenario.setPos(0,-30,0);
	scenario.setSize(250,350,250);
	scenario.setTexture(&rockTex);
	scenario.material.setDiffuse(0.4,0.4,0.4);
	//scenario.material.setSpecular(0.2,0.2,0.2);
	//scenario.material.setShininess(80);
		objects.push_back(&scenario);	//objects[2]
	
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
	tableFrame.setMaterial(tableTopMat);
	tableStruct.setTexture(&woodTex);
	tableTop.setTexture(&tableTex);
	tableFrame.setTexture(&tableTex);
	tableStruct.setSize(100,100,100); 
	tableTop.setSize(100,100,100);
	tableFrame.setSize(100,100,100);
		objects.push_back(&tableTop);	//objects[3]
		objects.push_back(&tableStruct);//objects[4]

	// table2
	tableStructMat.setDiffuse(0.25,0.09,0.07);
	tableStructMat.setSpecular(0,0,0);
	tableTopMat.setDiffuse(0.078 *0.5, 0.66 *0.5, 0.078 *0.5);	
	tableStruct2.setMaterial(tableStructMat);
	tableTop2.setMaterial(tableTopMat);
	tableStruct2.setSize(100,100,100);
	tableTop2.setSize(100,100,100);
	tableStruct2.setPos(0,0,-300);
	tableTop2.setPos(0,0,-300);
	// table3
	tableStruct3.setMaterial(tableStructMat);
	tableTop3.setMaterial(tableTopMat);
	tableStruct3.setSize(100,100,100);
	tableTop3.setSize(100,100,100);
	tableStruct3.setPos(0,0,300);
	tableTop3.setPos(0,0,300);
	// table4
	tableStruct4.setMaterial(tableStructMat);
	tableTop4.setMaterial(tableTopMat);
	tableStruct4.setSize(100,100,100);
	tableTop4.setSize(100,100,100);
	tableStruct4.setPos(0,0,600);
	tableTop4.setPos(0,0,600);
		objects.push_back(&tableStruct2);//objects[5]
		objects.push_back(&tableTop2);	 //objects[6]
		objects.push_back(&tableStruct3);//objects[7]
		objects.push_back(&tableTop3);	 //objects[8]
		objects.push_back(&tableStruct4);//objects[9]
		objects.push_back(&tableTop4);	 //objects[10]
	
	// ceiling lamp
	light.setPos(0,120,0);
	light.setSize(50,50,50);
	light.material.setDiffuse(0.5,0.5,0.5);
	light.material.setSpecular(1,1,1);
	light.material.setShininess(120);
	//light.material.setEmission(RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4);
		objects.push_back(&light);		 //objects[11]

	
	// infinite scenario globe
	globe.setSize(500,500,500);
	globe.setTexture(&starsTex);
		objects.push_back(&globe);		 //objects[12]
	
	#ifdef SHOW_TABLE_FRAME
		objects.push_back(&tableFrame);  //objects[#]
	#endif

    for (int i=0;i<NSTARS;i++)
    {
    	star[i].x = rand()%500 - 250;
    	star[i].y = rand()%500 - 250;
    	star[i].z = rand()%500 - 250;
    }
}

void drawObjects_partial () {
	
	objects[0]->draw();
	stick.draw();
	tableStruct.draw();
	tableTop.draw();
	tableFrame.draw();
}

void castShadows() {
	float lightSource[] = { 0, 100, 0, 1 };
	float tablePlane [] = { 0, BALL_O_Y-1, 0 };
	float floorPlane [] = { 0, 0, 0 };
	float planeNormal[] = { 0, -1, 0 };

	glDisable(GL_LIGHTING); 					//lights don't affect shadows
	glEnable (GL_BLEND); 						//enable transparency
	glEnable(GL_STENCIL_TEST); 					//enable stencil testing when drawing polygones
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR); 	/* sets operations with stencil buffer when: 
												 * stencil_test = false
												 * stencil_test = true && depth_test = false
												 * stencil_test = true
												 */
	glColor4f(0,0,0,0.5); 						//black shadow with 50% transparency

	// Cast shadows on the ground
	glPushMatrix();
	glShadowProjection(lightSource,floorPlane,planeNormal); //manipulates the matrix so everything will be projected in the FLOORPLANE
		glClear( GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_EQUAL, 0, 1); 						//will let only one vertex be drawn on each position each time shadow
			tableStruct.draw();
			tableTop.draw();
			stick.draw();
	glPopMatrix();
	
	// Cast shadows on the table
	glPushMatrix();
	glShadowProjection(lightSource,tablePlane,planeNormal); //manipulates the matrix so everything will be projected in the TABLEPLANE
		glClear( GL_STENCIL_BUFFER_BIT);
		glColorMask(false, false, false, false); 			//disables Color Buffer
		glDepthMask(false); 								//disables Depth Buffer
		glStencilFunc(GL_ALWAYS, 1, 1); 					//creates the mask that will define where shadow will be cast
			tableTop.draw();
		
		glColorMask(true,true,true,true);
		glDepthMask(true); 
		glStencilFunc(GL_EQUAL, 1, 1); 						//now the shadow is cast only where stencil buffer==1, i.e. the table
			stick.draw();
			ball.draw();
			#ifdef SHOW_TABLE_FRAME
				tableFrame.draw();
			#endif
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
}

void drawObjects () {
    // drawing of not-lit objects
    glDisable(GL_LIGHTING);
	    //// Sun
		//glPushMatrix();
	    //glTranslated( SUN_MOVEMENT_EQUATION );
		//glutSolidSphere(0.5,10,10);
	    //glPopMatrix();
		
		globe.draw();
		
		// Holes delimiters
		for(int i=0; i<NHOLES; i++)
			glCircle3f(HC[i][0],BALL_O_Y,HC[i][1],HC[i][2]);
		
		/*printf("%.3f,%.3f,%.3f %.3f\n",cursor.getPosX(),cursor.getPosY(),cursor.getPosZ(),radius);
		glCircle3f(cursor.getPosX(),BALL_O_Y,cursor.getPosZ(),radius);
		//cursor.draw();
		glBegin(GL_LINES);
			glVertex3f(cursor.getPosX()-10, cursor.getPosY(),cursor.getPosZ());
			glVertex3f(cursor.getPosX()+10, cursor.getPosY(),cursor.getPosZ());
			glVertex3f(cursor.getPosX(), cursor.getPosY(),cursor.getPosZ()+10);
			glVertex3f(cursor.getPosX(), cursor.getPosY(),cursor.getPosZ()-10);
		glEnd();*/

	glEnable(GL_LIGHTING); //ends drawing of not-lit objects
	
	// draw all objects
	for( int it=0; it<objects.size(); it++ )
		objects[it]->draw();
}

void lights ()
{
	// position light (sun)
	//GLfloat position[] = { 0 , 100 , 0 , 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, position);    
	//GLfloat spot_direction[] = { 0.0 , -1.0 , 0.0 , 0.0};
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	
	
	// spotlight
	//GLfloat light1_position[] = { 0.0, 100.0, 0.0, 1.0 };
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	
	// spotlight 2
	//GLfloat light2_position[] = { 0.0, 100.0, -300.0, 1.0 };
	//glLightfv(GL_LIGHT3, GL_POSITION, light2_position);
	//glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	
	// spotlight 3
	//GLfloat light3_position[] = { 0.0, 100.0, 300.0, 1.0 };
	//glLightfv(GL_LIGHT4, GL_POSITION, light3_position);
	//glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
	
	
	// spotlight 4
	//GLfloat light4_position[] = { 0.0, 100.0, 600.0, 1.0 };
	//glLightfv(GL_LIGHT5, GL_POSITION, light4_position);
	//glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction);
	
	// directional light
	//GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	//glLightfv(GL_LIGHT2, GL_POSITION, direction);
	
	
}

void perspectiveViewport( int width, int height ) {
	glMatrixMode (GL_PROJECTION); //set the matrix to projection
	glLoadIdentity ();
	
    glViewport (0, 0, (GLsizei)width, (GLfloat)height);
    glScissor(0, 0, width, height);
    
    if( invertViewports )
		glOrtho(-65, 65, -35, 35, 0.1, 10000);
	else
		gluPerspective (lensAngle, (GLfloat)width / (GLfloat)height, 0.1, 10000.0);
    					
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  
    // IMPORTANT: don't change the order of these calls
		drawOsd(osd,camera,fps);
		if( invertViewports )
			camera2.apply();
		else
			camera.apply();
		level.lights();
		level.drawObjects();
		castShadows();
}	

void orthoViewport( int width, int height ) {
	glViewport ((3*width)/4, (3*height)/4, (GLsizei)width/4, (GLfloat)height/4);
	glScissor((3*width)/4, (3*height)/4, width/4, height/4);
	
	glEnable(GL_SCISSOR_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
    if( invertViewports )
		gluPerspective (lensAngle, (GLfloat)width / (GLfloat)height, 0.1, 500.0);
	else
		glOrtho(-65, 65, -35, 35, 5, 500);
    					
    glMatrixMode (GL_MODELVIEW);  //set the matrix back to model
    glLoadIdentity();
	// IMPORTANT: don't change the order of these calls
		if( invertViewports )
			camera.apply();
		else
			camera2.apply();
		level.lights();
		level.drawObjects_partial();
		
	glDisable(GL_SCISSOR_TEST);
}	

void display () {

	int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);
	
		orthoViewport(width,height);
		perspectiveViewport(width,height);
				
	glDisable(GL_SCISSOR_TEST);
		
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
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.002);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00005);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 160);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
	
	// spotlight
	GLfloat lampColor[] = {RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4, 1.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0);
	
	// spotlight 2
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 15.0);
	// spotlight 3
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 15.0);
	// spotlight 4
	glLightfv(GL_LIGHT5, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 15.0);	
	
	// directional light
	GLfloat lampColor2[] = {RGB(252), RGB(234), RGB(186), 1.0f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lampColor2);
	
	// Creating LightInfo vector to the level class
	
	// position light (sun)
	//GLfloat position[] = { 0 , 100 , 0 , 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, position);    
	//GLfloat spot_direction[] = { 0.0 , -1.0 , 0.0 , 0.0};
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	GLfloat position[] = { 0 , 100 , 0 , 1.0f };
	GLfloat spot_direction[] = { 0.0 , -1.0 , 0.0 , 0.0};
	LightInfo light0(true, position, true, spot_direction, GL_LIGHT0);
	theLights.push_back(&light0);
	
	// spotlight
	//GLfloat light1_position[] = { 0.0, 100.0, 0.0, 1.0 };
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	GLfloat light1_position[] = { 0.0, 100.0, 0.0, 1.0 };
	LightInfo light1(true, light1_position, true, spot_direction, GL_LIGHT1);
	theLights.push_back(&light1);
	
	// spotlight 2
	//GLfloat light2_position[] = { 0.0, 100.0, -300.0, 1.0 };
	//glLightfv(GL_LIGHT3, GL_POSITION, light2_position);
	//glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	
	GLfloat light2_position[] = { 0.0, 100.0, -300.0, 1.0 };
	LightInfo light3(true, light2_position, true, spot_direction, GL_LIGHT3);
	theLights.push_back(&light3);
	
	// spotlight 3
	//GLfloat light3_position[] = { 0.0, 100.0, 300.0, 1.0 };
	//glLightfv(GL_LIGHT4, GL_POSITION, light3_position);
	//glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
	
	GLfloat light3_position[] = { 0.0, 100.0, 300.0, 1.0 };
	LightInfo light4(true, light3_position, true, spot_direction, GL_LIGHT4);
	theLights.push_back(&light4);
	
	// spotlight 4
	//GLfloat light4_position[] = { 0.0, 100.0, 600.0, 1.0 };
	//glLightfv(GL_LIGHT5, GL_POSITION, light4_position);
	//glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction);
	
	GLfloat light4_position[] = { 0.0, 100.0, 600.0, 1.0 };
	LightInfo light5(true, light4_position, true, spot_direction, GL_LIGHT5);
	theLights.push_back(&light5);
	
	// directional light
	//GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	//glLightfv(GL_LIGHT2, GL_POSITION, direction);
	
	GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	LightInfo light2(true, direction, false, direction, GL_LIGHT2);
	theLights.push_back(&light2);
}

void init ()
{
	cout << "Loading models...";
	initObjects();
	cout << "Done.\n";
	
    glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_NORMALIZE);		//normalizes all normals
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glShadeModel (GL_SMOOTH);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	initLights();
	glEnable (GL_LIGHT0); 	// sun
	glEnable (GL_LIGHT1);   // spotlight
	glEnable (GL_LIGHT2); 	// directional
	glEnable (GL_LIGHT3); // extra spotlight1
	glEnable (GL_LIGHT4); // extra spotlight2
	glEnable (GL_LIGHT5); // extra spotlight3
    
    cout << "Loading textures..."; 
    loadTexture(&ballTex, "textures/poolball.tga");
    loadTexture(&woodTex, "textures/wood.tga");
    loadTexture(&tableTex, "textures/table.tga", true);//set true to use mipmapping
    loadTexture(&rockTex, "textures/rock.tga");//set true to use mipmapping
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
   
    if( key=='w' )
		cursor.setPosZ( cursor.getPosZ()-0.3 );
	if( key=='s' )
		cursor.setPosZ( cursor.getPosZ()+0.3 );
	if( key=='a' )
		cursor.setPosX( cursor.getPosX()-0.3 );
	if( key=='d' )
		cursor.setPosX( cursor.getPosX()+0.3 );		
    if( key=='q' )
		radius-=0.1;
	if( key=='e' )
		radius+=0.1;
    
    if ( key==K_SPACE) {
		ball.applyForce(stick.getAttackStrenght()*10,stick.getAngleInXZ()+90);  //some naughty magic numbers here
		stick.setAttackStrenght(0);
	}
		
    // chances lens focal distance
    /*if ( key=='s' )
		lensAngle+=2;
	if ( key=='w' )
		lensAngle-=2;
		* */
		
	if( key=='v')
		invertViewports = !invertViewports;
    
    if( key=='c' )
		camera.nextCameraMode(objects[0]);
	
	if ( key=='1' )
		glDisable(GL_LIGHT1);
	if ( key=='2' )
		glEnable(GL_LIGHT1); 
	
    if ( key==K_ESC )
    {
	    exit(0);
    }
}

void specialFunc(int key, int x, int y)
{
	if ( key == GLUT_KEY_LEFT )
	{
		stick.rotate( -5 );
	}
	
	if ( key == GLUT_KEY_RIGHT )
	{
		stick.rotate( 5 );
	}
	
	if ( key == GLUT_KEY_DOWN )
	{
		stick.changePower(0.1);
	}
	
	if ( key == GLUT_KEY_UP )
	{
		stick.changePower(-0.1);
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
	
	holdCtrl = (glutGetModifiers() == GLUT_ACTIVE_CTRL);
}

void mouseMotionFunc(int x, int y) {
	
	if (GLUT_DOWN == left_click)
    {
		if( holdCtrl )
			stick.rotate( (x-xold)/5. );
		else
			camera.action1(x - xold, y - yold);
    }
	
	if (GLUT_DOWN == right_click)
	{
		if( holdCtrl )
			stick.changePower( (y-yold)/2. );
		else		
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
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL); //set the display to Double buffer, with depth
    glutInitWindowSize (1280, 720);                  //set the window size
    glutInitWindowPosition (400, 0);              //set the position of the window
    glutCreateWindow ("SiNoS - mouse/c: camera  ctrl+mouse: stick  spacebar: attack");     //the caption of the window
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
