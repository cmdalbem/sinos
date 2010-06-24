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
Camera camera, camera2(2);

// Texture files
Texture tigerTex, woodTex, tableTex, rockTex, starsTex, ballTex, stickTex;

Level level(&objects, &theLights, &camera, &camera2, &ballTex, &stickTex);

//texturized objects 
static ObjectModel tableStruct("obj/pooltable_struct_noframe.obj");
static ObjectModel tableTop("obj/pooltable_table.obj");
static ObjectModel tableFrame("obj/pooltable_frame.obj");
static ObjectModel scenario("obj/crypt.obj");
static ObjectModel globe("obj/globe.obj");
static ObjectBall  cursor(1,100,100);


// Normal objects
static ObjectModel tableShadow("obj/pooltable_shadow.obj");
static ObjectModel light("obj/light1.obj");
static ObjectModel tableStruct2("obj/pooltable_struct.obj");
static ObjectModel tableTop2("obj/pooltable_table.obj");
static ObjectModel tableStruct3("obj/pooltable_struct.obj");
static ObjectModel tableTop3("obj/pooltable_table.obj");
static ObjectModel tableStruct4("obj/pooltable_struct.obj");
static ObjectModel tableTop4("obj/pooltable_table.obj");
						  
// mouse-keyboard
static int	xold, yold;		
static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;
static int	middle_click = GLUT_UP;
static bool heldCtrl = false;
static bool heldShift = false;
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

void initObjects ()
{
	cursor.setPos(0,TABLE_PLANE_Y,2);
	
	// crypt scenario
	scenario.setPos(0,-30,0);
	scenario.setSize(250,350,250);
	scenario.setTexture(&rockTex);
	scenario.material.setDiffuse(0.4,0.4,0.4);
	//scenario.material.setSpecular(0.2,0.2,0.2);
	//scenario.material.setShininess(80);
		objects.push_back(&scenario);	//objects[0]
	
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
		objects.push_back(&tableTop);		//objects[1]
		objects.push_back(&tableStruct);	//objects[2]
	tableShadow.setSize(100,100,100);

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
		objects.push_back(&tableStruct2);//objects[3]
		objects.push_back(&tableTop2);	 //objects[4]
		objects.push_back(&tableStruct3);//objects[5]
		objects.push_back(&tableTop3);	 //objects[6]
		objects.push_back(&tableStruct4);//objects[7]
		objects.push_back(&tableTop4);	 //objects[8]
	
	// ceiling lamp
	light.setPos(0,120,0);
	light.setSize(50,50,50);
	light.material.setDiffuse(0.5,0.5,0.5);
	light.material.setSpecular(1,1,1);
	light.material.setShininess(120);
	//light.material.setEmission(RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4);
		objects.push_back(&light);		 //objects[9]

	
	// infinite scenario globe
	globe.setSize(500,500,500);
	globe.setTexture(&starsTex);
		objects.push_back(&globe);		 //objects[10]
	
	#ifdef SHOW_TABLE_FRAME
		objects.push_back(&tableFrame);  //objects[11]
	#endif

    for (int i=0;i<NSTARS;i++)
    {
    	star[i].x = rand()%500 - 250;
    	star[i].y = rand()%500 - 250;
    	star[i].z = rand()%500 - 250;
    }
}

void castShadows() {
/* TO DO: modularize shadowing */
	
	float lightSource[] = { 0, 100, 0, 1 };
	float tablePlane [] = { 0, TABLE_PLANE_Y, 0 };
	float floorPlane [] = { 0, 0, 0 };
	float planeNormal[] = { 0, -1, 0 };

	glDisable(GL_LIGHTING); 					//lights don't affect shadows
	glEnable (GL_BLEND); 						//enable transparency
	glEnable(GL_STENCIL_TEST); 					//enable stencil testing when drawing polygones
	
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR); 	// sets operations with stencil buffer when: 
												// stencil_test = false
												// stencil_test = true && depth_test = false
												// stencil_test = true
												
	glColor4f(0,0,0,0.5); 						//black colored shadow with 50% transparency

	// Cast shadows on the ground
	glPushMatrix();
		glShadowProjection(lightSource,floorPlane,planeNormal); //manipulates the matrix so everything will be projected in the FLOORPLANE
			glClear( GL_STENCIL_BUFFER_BIT);
			glStencilFunc(GL_EQUAL, 0, 1); 						//will let only one vertex be drawn on each position each time shadow
				tableShadow.draw(); 
				level.stick.draw();
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
				level.stick.draw();
				level.ball.draw();
				#ifdef SHOW_TABLE_FRAME
					tableFrame.draw();
				#endif
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
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
	
	level.ball.updateRotateMatrix(); //this MUST be called HERE
	
    glLoadIdentity ();
    
    // IMPORTANT: don't change the order of these calls
		drawOsd(osd, camera,fps);
		if( invertViewports )
			level.camera2->apply();
		else
			level.camera->apply();
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
    					
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    
	// IMPORTANT: don't change the order of these calls
		if( invertViewports )
			level.camera->apply();
		else
			level.camera2->apply();
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
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0001);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,   0.0001);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION,0.0001);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0);
	
	// spotlight 2
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.001);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.005);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 15.0);
	// spotlight 3
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.001);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.005);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 15.0);
	// spotlight 4
	glLightfv(GL_LIGHT5, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.001);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.005);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 15.0);	
	
	// directional light
	GLfloat lampColor2[] = {RGB(252), RGB(234), RGB(186), 1.0f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lampColor2);
	
	
	/*
	// Creating LightInfo vector to the level class
	//
	// THERE IS SOMETHING EXTREMELY WRONG IN THIS PART OF THE CODE.
	// TRY PRINTING THE VALUE OF THE VARIABLES INSIDE THE LIGHTINFO
	// INSTANCES ON THE VECTOR AND YOU'RE GONNA SEE VERY STRANGE
	// VALUES INSTEAD OF THOSE YOU'VE JUST PUT THERE D=
	//
	// FOR NOW, THE LEVEL CLASS IS USING THE LIGHTS() FUNCTION INSTEAD
	// OF THESE LIGHTINFO INSTANCES.
	
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
	//theLights.push_back(&light1);
	
	// spotlight 2
	//GLfloat light2_position[] = { 0.0, 100.0, -300.0, 1.0 };
	//glLightfv(GL_LIGHT3, GL_POSITION, light2_position);
	//glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	
	GLfloat light2_position[] = { 0.0, 100.0, -300.0, 1.0 };
	LightInfo light3(true, light2_position, true, spot_direction, GL_LIGHT3);
	//theLights.push_back(&light3);
	
	// spotlight 3
	//GLfloat light3_position[] = { 0.0, 100.0, 300.0, 1.0 };
	//glLightfv(GL_LIGHT4, GL_POSITION, light3_position);
	//glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
	
	GLfloat light3_position[] = { 0.0, 100.0, 300.0, 1.0 };
	LightInfo light4(true, light3_position, true, spot_direction, GL_LIGHT4);
	//theLights.push_back(&light4);
	
	// spotlight 4
	//GLfloat light4_position[] = { 0.0, 100.0, 600.0, 1.0 };
	//glLightfv(GL_LIGHT5, GL_POSITION, light4_position);
	//glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction);
	
	GLfloat light4_position[] = { 0.0, 100.0, 600.0, 1.0 };
	LightInfo light5(true, light4_position, true, spot_direction, GL_LIGHT5);
	//theLights.push_back(&light5);
	
	// directional light
	//GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	//glLightfv(GL_LIGHT2, GL_POSITION, direction);
	
	GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	LightInfo light2(true, direction, false, direction, GL_LIGHT2);
	//theLights.push_back(&light2);
	*/
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
    loadTexture(&woodTex, "textures/wood.tga");
    loadTexture(&tableTex, "textures/table.tga", true);//set true to use mipmapping
    loadTexture(&rockTex, "textures/rock.tga");//set true to use mipmapping
    loadTexture(&starsTex, "textures/stars3.tga");
    loadTexture(&stickTex, "textures/stick.tga");
    loadTexture(&ballTex,  "textures/poolball.tga");
    cout << "Done.\n"; 
}

//--------------------------- KEYBOARD ---------------------------//
void keyboardFunc (unsigned char key, int x, int y) {
			
	if ( key==K_SPACE) {
		level.ball.applyForce(level.stick.getAttackStrenght()*10,level.stick.getAngleInXZ()+90);  //some naughty magic numbers here
		level.stick.attack();
	}
		
	if( key=='v')
		invertViewports = !invertViewports;
    
    if( key=='c' )
		level.camera->nextMode(&(level.ball));
	
    if ( key==K_ESC )
    {
	    exit(0);
    }
}

void specialFunc(int key, int x, int y)
{
	if( key == GLUT_KEY_F1 ) {
		level.camera->setMode(0);
	}
	if( key == GLUT_KEY_F2 ) {
		
		level.camera->setMode(1,&(level.ball));
	}
	if( key == GLUT_KEY_F3 ) {
		level.camera->setMode(2);
	}
	
	if( key == GLUT_KEY_LEFT ) {
		level.stick.rotate( -5 );
	}
	
	if( key == GLUT_KEY_RIGHT ) {
		level.stick.rotate( 5 );
	}
	
	if( key == GLUT_KEY_DOWN ) {
		level.stick.changePower(0.1);
	}
	
	if( key == GLUT_KEY_UP ) {
		level.stick.changePower(-0.1);
	}
}

//--------------------------- MOUSE ---------------------------//
void mouseFunc(int button, int state, int x, int y) {
/* This function only updates click states and positions */
 
	if( button == GLUT_LEFT_BUTTON )
		left_click = state;
	if( button == GLUT_RIGHT_BUTTON )
		right_click = state;
	if( button == GLUT_WHEEL_MIDDLE )
		middle_click = state;
	if( button == GLUT_WHEEL_DOWN )
		level.camera->action2(0,5);
	if( button == GLUT_WHEEL_UP )
		level.camera->action2(0,-5);
		
	xold = x;
	yold = y;
	
	heldCtrl = (glutGetModifiers() == GLUT_ACTIVE_CTRL);
	heldShift = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
}

void mouseMotionFunc(int x, int y) {
	
	if ( left_click == GLUT_DOWN ) {
		level.stick.rotate( (x-xold)/5. );			
    }
	
	if ( right_click == GLUT_DOWN ) {
		// stick manual attack
		level.stick.changePower( (y-yold)/5. );
		
	    if( level.stick.getAttackStrenght()<level.ball.getRadius() ) {
			level.ball.applyForce((yold-y),level.stick.getAngleInXZ()+90);  //some naughty magic numbers here
			level.stick.attack();
		}
	}
	
	if ( middle_click ==GLUT_DOWN  ) {
		level.camera->action1(x - xold, y - yold);
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
	level.updateVariables();
	glutTimerFunc(1000/STATEUPDATES_PER_SEC, updateState, 0);
}




int main (int argc, char **argv) {
    cout << "Initializing...\n";
    glutInit (&argc, argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL ); //set the display to Double buffer, with depth and stencil buffers
    glutInitWindowSize (1280, 720);
    glutInitWindowPosition (400, 0);              					 //set the position of the window
    glutCreateWindow ("SiNoS - middleButton/c: camera  mouse: stick  spacebar: attack!");
    //glutGameModeString("1920x1080:16@60");			//Full Screen Mode (adjust resolution for your full resolution values)
    //glutEnterGameMode();
    init();
    
    glutDisplayFunc (display);
    
    
    glutIdleFunc (display);							// update any variables in display, display can be changed to anyhing,
                            						// as long as you move the variables to be updated, in this case, angle++;
    
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	glutTimerFunc(1000/*1sec*/, updateFPS, 0);    
	glutTimerFunc(1000/STATEUPDATES_PER_SEC , updateState, 0);
	
    glutMainLoop(); 
    
    
    return 0;
} 
