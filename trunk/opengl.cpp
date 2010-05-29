#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "lib/glm.h"
#include "lib/imageloader.h"
#include "Object.h"
#include "ObjectModel.h"
#include "ObjectBall.h"
#include "constants.h"


//camera
float 	xpos = 5, 
		ypos = 7,
		zpos = 5, 
		xrot = 45, 
		yrot = -45, 
		angle= 0.0;

//mouse
static int	xold, yold;		
static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;
		
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
ObjectModel wall("obj/wall.obj");
ObjectBall  ball(0.05,100,100);

void drawWall(int d, int parts, int nx, int ny, int nz)
{
	for( int i=0; i<parts; i++ )
	{
		glNormal3f(nx, ny, nz);
		glBegin(GL_QUADS);
			glVertex3i(-d,d,d);
			glNormal3f(nx, ny, nz);
			glVertex3i(d,d,d);
			glNormal3f(nx, ny, nz);
			glVertex3i(d,0,d);
			glNormal3f(nx, ny, nz);
			glVertex3i(-d,0,d);
		glEnd();
	}
}


void initObjects (void)
{
	tableStruct.material.setDiffuse(0.25,0.09,0.07);
	tableStruct.setSize(5,5,5);
	tableStruct.material.setShininess(120);
	tableStruct.material.setSpecular(0.3,0.3,0.3);
	
	tableTop.material.setDiffuse(0.078, 0.66, 0.078);
	tableTop.setSize(5,5,5);
	
	stick.material.setDiffuse(RGB(238),RGB(221),RGB(195));
	stick.material.setSpecular(0.3,0.3,0.3);
	stick.material.setShininess(80);
	stick.setPos(0,1.675,0);
	//stick.setRot(30,330,0); // aehoo não consigo fazer isso aqui funcionar!!
	stick.setSize(0.5,0.5,0.5);
	
	ball.setPos(0,1.475,0);
	ball.material.setShininess(120);
	ball.material.setDiffuse(0.6, 0.6, 0.6);
	ball.material.setSpecular(0.9, 0.9, 0.9);
	
	wall.material.setDiffuse(0.8,0.8,0.8);
	wall.material.setSpecular(0.7,0.7,0.7);
	wall.material.setShininess(120);
	wall.setPos(0,20,0);
	wall.setSize(20,20,20);
	wall.setRot(60,0,0);


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
	
	
	// Walls
	glPushMatrix();
	Material wallsMaterial;
	wallsMaterial.setDiffuse(0.8,0.8,0.8);
	wallsMaterial.setSpecular(0.7,0.7,0.7);
	wallsMaterial.setShininess(100);
	wallsMaterial.apply();
	int d=25;
	//tentativa fracassada de função pra criar parede iterativamente, com "parts" partições
	/*int parts=4;
	int f = d/parts;
	for(int i=0; i<parts; i++)
	{
		glNormal3f(0.0, -1.0f, 0.0f);
		glBegin(GL_QUADS); //ceiling
			glVertex3i(-f,d,-f);
			glVertex3i(f,d,-f);
			glVertex3i(f,d,f);
			glVertex3i(-f,d,f);
		glEnd();
		glTranslated(f,0,f);
	}*/
	
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0f, -1.0f);
		glVertex3i(-d,d,d);
		glNormal3f(0.0, 0.0f, -1.0f);
		glVertex3i(d,d,d);
		glNormal3f(0.0, 0.0f, -1.0f);
		glVertex3i(d,0,d);
		glNormal3f(0.0, 0.0f, -1.0f);
		glVertex3i(-d,0,d);
	glEnd();
	glNormal3f(0.0, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex3i(d,0,-d);
		glVertex3i(d,d,-d);
		glVertex3i(-d,d,-d);
		glVertex3i(-d,0,-d);
	glEnd();
	glNormal3f(1.0, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex3i(-d,d,-d);
		glVertex3i(-d,d,d);
		glVertex3i(-d,0,d);
		glVertex3i(-d,0,-d);
	glEnd();
	wallsMaterial.unapply();
    glPopMatrix();
        
	// objects
	tableStruct.draw();
	tableTop.draw();
	stick.draw();
	ball.draw();
	wall.draw();
	wall.rotate(1,0,0);
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
	GLfloat lightColor1[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat direction[] = {0.0f, 1.0f, 0.0f, 0.0f};
	//glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, direction);
	
	
	// spotlight
	//GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	//GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light1_position[] = { 0.0, -5.0, 0.0, 1.0 };
	//GLfloat spot_direction[] = { 0.0, 1.0, 0.0 };
	////glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	////glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	////glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	////glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

	
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
    
    
    Image* image = loadBMP("textures/tiger.bmp");
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
    

	glLoadIdentity();  
    
    camera();
   
    drawObjects();
    
    
    glutSwapBuffers(); //swap the buffers
    angle++; //increase the angle  /* wtf is this for??? (cristiano)*/
}

void reshape (int w, int h) {
    glMatrixMode (GL_PROJECTION);				//set the matrix to projection
    
	glLoadIdentity ();
	
    glViewport (0, 0, (GLsizei)w, (GLfloat)h);
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 2000.0);
    					
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model
}

void keyboardFunc (unsigned char key, int x, int y) {
   
    if( key=='e')
    {
		ypos += 0.2;
	}
	
	if( key=='c')
    {
		ypos -= 0.2;
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
    
    if (key==K_ESC)
    {
	    exit(0);
    }
}



void mouseFunc(int button, int state, int x, int y)
{
  if (GLUT_LEFT_BUTTON == button)
    left_click = state;
  if (GLUT_RIGHT_BUTTON == button)
    right_click = state;
  xold = x;
  yold = y;
}

void mouseMotionFunc(int x, int y)
{
	if (GLUT_DOWN == left_click)
    {
		xrot = xrot + (y - yold) / 5.f;
		yrot = yrot + (x - xold) / 5.f;
		glutPostRedisplay();
    }
	
	if (GLUT_DOWN == right_click)
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos += (yold - y)/10.0 * float(sin(yrotrad)) ;
		zpos -= (yold - y)/10.0 * float(cos(yrotrad)) ;
		ypos -= (yold - y)/10.0 * float(sin(xrotrad)) ;
		glutPostRedisplay();
	}

	xold = x;
	yold = y;
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
    
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
  
    glutMainLoop(); 
    
    
    return 0;
} 
