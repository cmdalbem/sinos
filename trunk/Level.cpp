// Level.cpp
//
// Function implementation of Level.h (and some POG, on a first time).
//

#include "Level.h"

//------------------------------------------------------------ CONSTRUCTORS
Level::Level(	vector<Object*> *_objects, vector<LightInfo*> *_theLights,
				Camera *_camera, Camera *_camera2,
				Texture *ballTex, Texture *stickTex)
{	
	objects   = _objects;
	theLights = _theLights;

	camera  = _camera;
	camera2 = _camera2;
	
	// the ball
	ball.loadFromFile("obj/poolball.obj");
	ball.material.setShininess(120); 
	ball.material.setAmbient(1,1,1); 
	ball.material.setEmission(0.1,0.1,0.1); 
	ball.material.setDiffuse(0.5, 0.5, 0.5);
	ball.material.setSpecular(0.9, 0.9, 0.9);
	ball.setTexture(ballTex);
	ball.setSize(10,10,10);
	ball.setPos(0, TABLE_PLANE_Y+ball.getRadius(), 0);
		//objects.push_back(level.getBall());
	
	// the stick
	stick.setCenter(&ball);
	
	stick.loadFromFile("obj/taco.obj");	
	stick.calculatePos();	// This is needed to put the Stick on the right place
							// since only now the ball is put its place.
	
	stick.material.setDiffuse(RGB(238),RGB(221),RGB(195));
	stick.material.setSpecular(0.3,0.3,0.3);
	stick.material.setShininess(80);
	stick.setTexture(stickTex);
	stick.setSize(7,7,8);
		//objects.push_back(level.getStick());
}

//------------------------------------------------------------ DESTRUCTORS
Level::~Level()	{ }

//------------------------------------------------------------ GETTERS & SETTERS

//------------------------------------------------------------ OTHER METHODS

void Level::drawObjects () {
    // drawing of not-lit objects
    glDisable(GL_LIGHTING);
		#warning Direct objects vector access.
		(*objects)[10]->draw();
		
		// Holes delimiters
		for(int i=0; i<NHOLES; i++)
			glCircle3f(HC[i][0],TABLE_PLANE_Y+1,HC[i][1],HC[i][2]);
			
	glEnable(GL_LIGHTING);
	
	ball.draw();
	stick.draw();
	
	// draw all objects
	for( int it=1; it<objects->size(); it++ )
		(*objects)[it]->draw();
	
	// TEMP: directional light only on crypt
	glEnable (GL_LIGHT2);
		(*objects)[0]->draw();
	glDisable (GL_LIGHT2);
}

void Level::drawObjects_partial ()
{
	ball.draw();
	stick.draw();
	(*objects)[1]->draw();
	(*objects)[2]->draw();
#ifdef SHOW_TABLE_FRAME
	//tableFrame.draw();		//----> refatorate this soon
#endif
	
}

void Level::lights()
{
	//for (int i = 0; i < theLights->size(); i++)
	//	(*theLights)[i]->apply();
	
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

void Level::updateVariables()
{
	bool	theBallWasMovimented;
	theBallWasMovimented = ball.updateState();
	
	if(theBallWasMovimented)
	{
		if(camera->getMode() == 1)
			camera->setMode(1, &ball);
		stick.setCenter(&ball);
	}
}
