#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "lib/glm.h"



#define NSTARS 1000
#define VFACTOR 150
#define DFACTOR 10



float 	xpos = 0, 
		ypos = 0,
		zpos = 0, 
		xrot = 0, 
		yrot = 0, 
		angle= 0.0;
		
GLfloat posit=0; //iterational position for the SUN

struct point
{
	float x,y,z;
};

// Objects
point star[NSTARS];
GLMmodel* tableModel = NULL;



void drawTable(void)
{
    // Load the model only if it hasn't been loaded before
    // If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
    if (!tableModel)
    {

        // this is the call that actualy reads the OBJ and creates the model object
        tableModel = glmReadOBJ("obj/pooltable.obj");
        if (!tableModel) exit(0);
        // This will rescale the object to fit into the unity matrix
        // Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
        glmUnitize(tableModel);
        // These 2 functions calculate triangle and vertex normals from the geometry data.
        // To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
        // So if you can export these directly from you modeling tool do it and comment these line
        // 3DS Max can calculate these for you and GLM is perfectly capable of loading them
        //glmFacetNormals(tableModel);
        //glmVertexNormals(tableModel, 90.0);

    }
    // This is the call that will actually draw the model
    // Don't forget to tell it if you want textures or not :))
    glmDraw(tableModel, GLM_SMOOTH); 

} 


void initStars (void) { //set the positions of the cubes

    for (int i=0;i<NSTARS;i++)
    {
    	star[i].x = rand()%100 - 50;
    	star[i].y = rand()%100 - 50;
    	star[i].z = rand()%100 - 50;
    }
}

void drawObjects (void) {
    // stars
    for (int i=0;i<NSTARS;i++)
    {
    	glPushMatrix();
    	glTranslated(star[i].x *20, star[i].y *20 , star[i].z *20);
	    glutSolidSphere(0.5,10,10);
    	glPopMatrix();
    }
    
    // table
	glPushMatrix();
	glTranslated(0,0,0);
	glScaled(5,5,5);
    drawTable();
	glPopMatrix();
	
	// ball
	glPushMatrix();
	glTranslated(0,0.46,0);
	//glutSolidTeapot(0.5);
	glutSolidSphere(0.07,100,100);
	glPopMatrix();
	
	// sun
	//glPushMatrix();
    //glTranslated(cos(posit/VFACTOR)*DFACTOR -DFACTOR/2, sin(posit/VFACTOR)*DFACTOR -DFACTOR/2, cos(posit/VFACTOR)*DFACTOR -DFACTOR/2);
	//glutSolidSphere(2,10,10);
    //glPopMatrix();  
}

void init (void) {
	initStars();
	
    // Especifica que a cor de fundo da janela sera branca
	glClearColor(0, 0, 0, 1.0f);
    // Limpa a janela e habilita o teste para eliminar faces ocultas por outras
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
	
	GLfloat ambientLight[] = { 0, 0, 0, 1.0f };
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glShadeModel (GL_SMOOTH);
    
}

void camera (void) {
	
	gluLookAt(cos(posit/VFACTOR)*DFACTOR -DFACTOR/2, sin(posit/VFACTOR)*DFACTOR -DFACTOR/2, cos(posit/VFACTOR)*DFACTOR -DFACTOR/2, 0, 0, 0, 0, 1, 0);
    
	//glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
	//glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    //glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
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
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0); 	//set the perspective
    															//(angle of sight, width, height, depth)
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
