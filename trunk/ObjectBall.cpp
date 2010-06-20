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

//------------------------------------------------------------ CONSTRUCTORS

ObjectBall::ObjectBall()
:	Object()
{
	radius = 0;
	slices = 0;
	stacks = 0;	
	resetSpeed();
}

ObjectBall::ObjectBall(GLdouble _radius, GLint _slices, GLint _stacks)
:   Object()
{
	radius = _radius;
	slices = _slices;
	stacks = _stacks;
	resetSpeed();
}


//------------------------------------------------------------ DESTRUCTORS

ObjectBall::~ObjectBall() { }

//------------------------------------------------------------ GETTERS & SETTERS

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
}

float ObjectBall::getSpeed()
{
	return abs(moveVector[0]) + abs(moveVector[1]) + abs(moveVector[2]);	
}

float ObjectBall::getNewX()
{
	return getPosX() + moveVector[0] / STATEUPDATES_PER_SEC;
}

float ObjectBall::getNewY()
{
	return getPosY() + moveVector[1] / STATEUPDATES_PER_SEC;
}

float ObjectBall::getNewZ()
{
	return getPosZ() + moveVector[2] / STATEUPDATES_PER_SEC;
}

//------------------------------------------------------------ OTHER METHODS

void ObjectBall::updateState()
{
	if( getSpeed() )
	{
		if( !moveVector[1] )
		{
			bool can_move_x = canMoveX(),
				 can_move_z = canMoveZ();

			if( hasSnooked() ) { 
				//setPos(0,BALL_O_Y,0);
				//resetSpeed();
				moveVector[1]=-30;
			}
			else {
				if( !can_move_x ) {
					moveVector[0] = -moveVector[0];
					changeSpeed(0.995);
				}
				if( !can_move_z ) {
					moveVector[2] = -moveVector[2];
					changeSpeed(0.995);
				}
			}
		}
		
		// update position
		setPos( getNewX(), getNewY(), getNewZ() );
		
		// update velocity
		if( !moveVector[1] ) { //if is not falling
			changeSpeed(0.965);
			if( getSpeed() < 0.05)
				resetSpeed();
		}
		else
			if( getPosY() > 1 ) {//until fall to the ground
				moveVector[1] *= 1.2;
				moveVector[0] *= 0.965;
				moveVector[2] *= 0.965;
			}
			else {
				resetSpeed();
				setPos(0,BALL_O_Y,0);
			}
		
	}
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

void ObjectBall::draw() const
{
	Object::drawBegin();
	
	glutSolidSphere(radius,slices,stacks);
	
	Object::drawEnd();
}


//--------------------------------------------------- POSITION DETECTION

bool ObjectBall::hasSnooked()
{
	float posx = getNewX(),
		  posy = getNewY(),
		  posz = getNewZ();

	/*// Limit Boundaries Test
	if( posx>B2P1[0] && posx<B2P2[0] &&
		posy>B2P1[1] )
		return true;
	else if( posx>B5P1[0] && posx<B5P2[0] &&
			 posy<B5P1[1] )
			return true;
	else return false;*/
	
	for(int i=0; i<NHOLES; i++)
		if( abs(posx-HC[i][0]) + abs(posz-HC[i][1]) <= HC[i][2] )
			return true;
	return false;
	
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
