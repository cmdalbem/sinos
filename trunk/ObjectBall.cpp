// ObjectBall.cpp
//
// Function implementation of ObjectBall.h
//

#include <iostream>
using namespace std;
#include "ObjectBall.h"
#include "constants.h"
#include <math.h>
#include "constants.h"
#include "utils.h"

//------------------------------------------------------------ CONSTRUCTORS

ObjectBall::ObjectBall()
:	ObjectModel()
{
	radius = 0;
	slices = 0;
	stacks = 0;	
	resetSpeed();
	resetRotateMatrix();
}

ObjectBall::ObjectBall(GLdouble _radius, GLint _slices, GLint _stacks)
:   ObjectModel()
{
	radius = _radius;
	slices = _slices;
	stacks = _stacks;
	resetSpeed();
	resetRotateMatrix();
}

ObjectBall::ObjectBall(string filename)
:	ObjectModel(filename)
{
	radius = 0;
	slices = 0;
	stacks = 0;	
	resetSpeed();	
	resetRotateMatrix();
}


//------------------------------------------------------------ DESTRUCTORS

ObjectBall::~ObjectBall() { }

//------------------------------------------------------------ GETTERS & SETTERS

GLdouble ObjectBall::getRadius() const
{
	return radius;
}

float ObjectBall::getDirection() const
{
	if( getSpeed() ) {
		float direction = DEGREES(acosf(moveVector[0]/getSpeed())); //angle of move vector = arc cos x/hypotenuse
		
		if( (moveVector[2]/getSpeed()) > 0 )
			direction += 2*(180-direction);
			
		return direction;
	}
	else
		return 0;
}	

float ObjectBall::getPerimeter() const
{
	return 2*M_PI*radius;
}


void ObjectBall::setStacks(GLint newStacks)
{
	stacks = newStacks;
}

void ObjectBall::setRadius(GLdouble newRadius)
{
	radius = newRadius;
}

void ObjectBall::setSlices(GLint newSlices)
{
	slices = newSlices;
}

void ObjectBall::resetSpeed()
{
	moveVector[0] = 0;
	moveVector[1] = 0;
	moveVector[2] = 0;
	
	setRot(0,0,0);
}

float ObjectBall::getSpeed() const
{
	return sqrt( pow(moveVector[0],2) + pow(moveVector[1],2) + pow(moveVector[2],2) );	
}

float ObjectBall::getFutureSpeed() const
{
	return sqrt( pow(moveVector[0]*BALL_DECELERATION_N,2) + pow(moveVector[1]*BALL_DECELERATION_N,2) + pow(moveVector[2]*BALL_DECELERATION_N,2) );	
}

float ObjectBall::getNewX() const
{
	return getPosX() + moveVector[0] / STATEUPDATES_PER_SEC;
}

float ObjectBall::getNewY() const
{
	return getPosY() + moveVector[1] / STATEUPDATES_PER_SEC;
}

float ObjectBall::getNewZ() const
{
	return getPosZ() + moveVector[2] / STATEUPDATES_PER_SEC;
}

void ObjectBall::setSize (GLfloat x, GLfloat y, GLfloat z)
{
	Object::setSize(x,y,z);
	if(x==z && z==y)
		radius = x/10;
}

//------------------------------------------------------------ OTHER METHODS

bool ObjectBall::updateState()
{
	if( getSpeed() )
	{
		if( !moveVector[1] )
		{
			bool can_move_x = canMoveX(),
				 can_move_z = canMoveZ();

			if( hasSnooked() )
				moveVector[1]=-30;
			else {
				if( !can_move_x ) { //invert x
					moveVector[0] = -moveVector[0];
					changeSpeed(BALL_DECELERATION_R);
				}
				if( !can_move_z ) { //invert z
					moveVector[2] = -moveVector[2];
					changeSpeed(BALL_DECELERATION_R);
				}
			}
		}
		
		// rotation by movement
		//setRotY(getDirection()+90);
		//rotate( (360*(getSpeed()-getFutureSpeed())) / getPerimeter() ,0,0);
		setRot( moveVector[2] ,0, -moveVector[0] );
		
		// update position
		setPos( getNewX(), getNewY(), getNewZ() );
		
		// update velocity
		if( !moveVector[1] ) { //if is not falling
			changeSpeed(BALL_DECELERATION_N);
			if( getSpeed() < 0.05)
				resetSpeed();
		}
		else
			if( getPosY() > 1 ) {//until fall to the ground
				moveVector[1] *= 1.2;
				moveVector[0] *= BALL_DECELERATION_N;
				moveVector[2] *= BALL_DECELERATION_N;
			}
			else {
				resetSpeed();
				setPos(0,TABLE_PLANE_Y+radius,0);
			}
		return true;
	}
	return false;
}

void ObjectBall::applyForce( float magnitude, float direction )
{
	moveVector[0] += magnitude * cos( RAD(direction) );
	moveVector[2] += -magnitude * sin( RAD(direction) );
}
	
void ObjectBall::changeSpeed( float mFactor )
{
	moveVector[0] *= mFactor;
	moveVector[1] *= mFactor;
	moveVector[2] *= mFactor;
}

void ObjectBall::drawBegin() const
{
	glPushMatrix();
	
	glTranslated(getPosX(), getPosY(), getPosZ());
	glScalef(getSizeX(), getSizeY(), getSizeZ());
	
	glMultMatrixd(*rotMat);
	    		
	material.apply();	
}

void ObjectBall::draw() const
{
	GLMmodel* modelPointer = getModelPointer();
	
	if( modelPointer )
	{
		ObjectBall::drawBegin();
		if( texture!=NULL )
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture->texID);
			glmDraw(modelPointer, GLM_SMOOTH | GLM_TEXTURE);
			glDisable(GL_TEXTURE_2D);
		}
		else
			glmDraw(modelPointer, GLM_SMOOTH);
		Object::drawEnd();
	}
	else {
		ObjectBall::drawBegin();
		glutSolidSphere(radius,slices,stacks);
		Object::drawEnd();
	}
}

void ObjectBall::updateRotateMatrix()
/* Adapted from OpenGL Screen & Object Space Rotations - MaxH - Mike Werner April 28, 2009 */
{
   glLoadIdentity ();
   glRotatef (getRotX(), 1,0,0);
   glRotatef (getRotY(), 0,1,0);
   glRotatef (getRotZ(), 0,0,1);
   glMultMatrixd (*rotMat);
   glGetDoublev (GL_MODELVIEW_MATRIX, *rotMat);
}

void ObjectBall::resetRotateMatrix()
{
	static GLdouble rex[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	*rotMat = rex;
}

//--------------------------------------------------- POSITION DETECTION

bool ObjectBall::hasSnooked()
{
	float posx = getNewX(),
		  posy = getNewY(),
		  posz = getNewZ();

	for(int i=0; i<NHOLES; i++)
		if( abs(posx-HC[i][0]) + abs(posz-HC[i][1]) <= HC[i][2] )
			return true;
	return false;
	
	/*// Limit Boundaries Test
	if( posx>B2P1[0] && posx<B2P2[0] &&
		posy>B2P1[1] )
		return true;
	else if( posx>B5P1[0] && posx<B5P2[0] &&
			 posy<B5P1[1] )
			return true;
	else return false;*/
	
}

bool ObjectBall::canMoveX()
{
	float posx = getNewX();
	
	if( posx > RIGHTBOUND  ||  posx < LEFTBOUND )
		return false;
	else
		return true;
}

bool ObjectBall::canMoveZ()
{
	float posz = getNewZ();
	
	if( posz > TOPBOUND  ||  posz < BOTBOUND )
		return false;
	else
		return true;
}
