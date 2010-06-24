// Level.cpp
//
// Function implementation of Level.h (and some POG, on a first time).
//

#include "Level.h"

//------------------------------------------------------------ CONSTRUCTORS
Level::Level(	map<string,Object*> *_objects, vector<LightInfo*> *_theLights,
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
	ball.setRadius(10);
	ball.setPos(0, TABLE_PLANE_Y+ball.getRadius(), 0);
	
	// ball2
	static ObjectBall ball2("obj/poolball.obj");
	ball2.setRadius(10);
	ball2.setPos(30, TABLE_PLANE_Y+ball2.getRadius(), 10);
	// ball3
	static ObjectBall ball3("obj/poolball.obj");
	ball3.setRadius(10);
	ball3.setPos(40, TABLE_PLANE_Y+ball2.getRadius(), 20);
		balls.push_back(&ball);
		balls.push_back(&ball2);
		balls.push_back(&ball3);
	
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
}

//------------------------------------------------------------ DESTRUCTORS
Level::~Level()	{ }

//------------------------------------------------------------ GETTERS & SETTERS

//------------------------------------------------------------ OTHER METHODS

void Level::drawObjects () {
    // drawing of not-lit objects
    glDisable(GL_LIGHTING);
		// Holes delimiters
		for(int i=0; i<NHOLES; i++)
			glCircle3f(HC[i][0],TABLE_PLANE_Y+1,HC[i][1],HC[i][2]);
			
	glEnable(GL_LIGHTING);
	
	ball.draw();
	stick.draw();
	
	// draw all objects
	map<string,Object*>::iterator it;
	for( it = objects->begin(); it!=objects->end(); it++ )
		if( (*it).first!="crypt" )
			(*it).second->draw();
			
	for( int i=0; i<balls.size(); i++ )
		balls[i]->draw();
	
	// TEMP: directional light only on crypt
	glEnable (GL_LIGHT2);
		(*objects)["crypt"]->draw();
	glDisable (GL_LIGHT2);
}

void Level::drawObjects_partial ()
{
	ball.draw();
	stick.draw();
	(*objects)["tableStruct"]->draw();
	(*objects)["tableTop"]->draw();
#ifdef SHOW_TABLE_FRAME
	(*objects)["tableFrame"]->draw();
#endif
	
}

void Level::castShadows() {
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
				//tableShadow.draw(); 
				stick.draw();
	glPopMatrix();
	
	// Cast shadows on the table
	glPushMatrix();
		glShadowProjection(lightSource,tablePlane,planeNormal); //manipulates the matrix so everything will be projected in the TABLEPLANE
			glClear( GL_STENCIL_BUFFER_BIT);
			glColorMask(false, false, false, false); 			//disables Color Buffer
			glDepthMask(false); 								//disables Depth Buffer
			glStencilFunc(GL_ALWAYS, 1, 1); 					//creates the mask that will define where shadow will be cast
				(*objects)["tableTop"]->draw();
			
			glColorMask(true,true,true,true);
			glDepthMask(true); 
			glStencilFunc(GL_EQUAL, 1, 1); 						//now the shadow is cast only where stencil buffer==1, i.e. the table
				stick.draw();
				ball.draw();
				for(int i=0;i<balls.size();i++)
					balls[i]->draw();
				#ifdef SHOW_TABLE_FRAME
					tableFrame.draw();
				#endif
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
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
	bool	ballHasMoved = ball.updateState();
	
	for(int i=0; i<balls.size(); i++)
		balls[i]->updateState();
	
	if(ballHasMoved)
	{
		if(camera->getMode() == 1)
			camera->setMode(1, &ball);
		stick.setCenter(&ball);
	}
	
	testBallsCollision();
}

void Level::testBallsCollision()
{
	for(int i=0; i<balls.size(); i++)
		for(int j=0; j<balls.size(); j++)
			if(i!=j)
				if( balls[i]->distanceFromObject(*balls[j]) < balls[i]->getRadius() + balls[j]->getRadius()) // is distance > sum of their radius
				{
					float impactv[3];
					impactv[0] = balls[i]->getPosX() - balls[j]->getPosX();
					impactv[1] = 0;
					impactv[2] = balls[i]->getPosZ() - balls[j]->getPosZ();
					
					balls[i]->applyForce( balls[i]->getSpeed() + balls[j]->getSpeed(), 200 );
					balls[j]->applyForce( balls[i]->getSpeed() + balls[j]->getSpeed(), -200 );
				}
}
