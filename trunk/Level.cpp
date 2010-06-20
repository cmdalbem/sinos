// Level.cpp
//
// Function implementation of Level.h (and some POG, on a first time).
//

#include "Level.h"

//------------------------------------------------------------ CONSTRUCTORS
Level::Level(vector<Object*> *_objects, vector<LightInfo*> *_theLights, Camera *_camera, Camera *_camera2)
{
	objects   = _objects;
	theLights = _theLights;
	
	camera  = _camera;
	camera2 = _camera2;
}

//------------------------------------------------------------ DESTRUCTORS
Level::~Level()	{ }

//------------------------------------------------------------ GETTERS & SETTERS


//------------------------------------------------------------ OTHER METHODS

void Level::drawObjects () {
    // drawing of not-lit objects
    glDisable(GL_LIGHTING);
	    //// Sun
		//glPushMatrix();
	    //glTranslated( SUN_MOVEMENT_EQUATION );
		//glutSolidSphere(0.5,10,10);
	    //glPopMatrix();
		
		(*objects)[12]->draw();
		
		// Holes delimiters
		for(int i=0; i<NHOLES; i++)
			glCircle3f(HC[i][0],TABLE_PLANE_Y+1,HC[i][1],HC[i][2]);
		
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
	for( int it=0; it<objects->size() && it != 12; it++ )
		(*objects)[it]->draw();
}

void Level::drawObjects_partial ()
{
	(*objects)[0]->draw();
	(*objects)[1]->draw();
	(*objects)[3]->draw();
	(*objects)[4]->draw();
	//tableFrame.draw();
}

void Level::lights()
{
		// position light (sun)

	GLfloat position[] = { 0 , 100 , 0 , 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);    
	GLfloat spot_direction[] = { 0.0 , -1.0 , 0.0 , 0.0};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	
	
	 //spotlight
	GLfloat light1_position[] = { 0.0, 100.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	
	 //spotlight 2
	GLfloat light2_position[] = { 0.0, 100.0, -300.0, 1.0 };
	glLightfv(GL_LIGHT3, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	
	 //spotlight 3
	GLfloat light3_position[] = { 0.0, 100.0, 300.0, 1.0 };
	glLightfv(GL_LIGHT4, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
	
	
	 //spotlight 4
	GLfloat light4_position[] = { 0.0, 100.0, 600.0, 1.0 };
	glLightfv(GL_LIGHT5, GL_POSITION, light4_position);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction);
	
	 //directional light
	GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT2, GL_POSITION, direction);
}
