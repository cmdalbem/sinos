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
	resetMovement();
}

ObjectBall::ObjectBall(GLdouble _radius, GLint _slices, GLint _stacks)
:   Object()
{
	radius = _radius;
	slices = _slices;
	stacks = _stacks;
	resetMovement();
}


//------------------------------------------------------------ DESTRUCTORS

ObjectBall::~ObjectBall() { }

//------------------------------------------------------------ GETTERS & SETTERS

void ObjectBall::resetMovement()
{
	moveVector[0] = 0;
	moveVector[1] = 0;
}

float ObjectBall::getSpeed()
{
	return abs(moveVector[0]) + abs(moveVector[1]);	
}

float ObjectBall::getNewX()
{
	return getPosX() + moveVector[0] / STATEUPDATES_PER_SEC;
}

float ObjectBall::getNewZ()
{
	return getPosZ() + moveVector[1] / STATEUPDATES_PER_SEC;
}

//------------------------------------------------------------ OTHER METHODS

void ObjectBall::updateState()
{
	if( getSpeed() )
	{
		if( !canMoveX() ) {
			moveVector[0] = -moveVector[0];
			changeSpeed(0.995);
		}
		if( !canMoveZ() ) {
			moveVector[1] = -moveVector[1];
			changeSpeed(0.995);
		}
			
		setPos( getNewX(), getPosY(), getNewZ() );
		
		// update velocity
		changeSpeed(0.95);
		if( getSpeed() < 0.05) //magic number detected
			resetMovement();
	}
}

void ObjectBall::applyForce( float magnitude, float direction )
{
	moveVector[0] += magnitude * cos( RAD(direction) );
	moveVector[1] += -magnitude * sin( RAD(direction) );
	
	/*double m1 = magnitude, m2 = speed;
	double a1 = _direction, a2 = direction;
	speed = sqrt( pow((m1*cos(a1) + m2*cos(a2)),2) + pow((m1*sin(a1) + m2*sin(a2)),2) );
	direction = acos(  (m1*cos(a1) + m2*cos(a2))
						/
						speed);						
	cout<<speed<<", ";
	cout<<direction<<endl;*/
	
}
	
void ObjectBall::changeSpeed( float mFactor )
{
	moveVector[0] *= mFactor;
	moveVector[1] *= mFactor;
}

void ObjectBall::draw() const
{
	Object::drawBegin();
	
	glutSolidSphere(radius,slices,stacks);
	
	Object::drawEnd();
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
