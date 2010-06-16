// ObjectBall.cpp
//
// Function implementation of ObjectBall.h
//

#include <iostream>
#include "ObjectBall.h"
#include "constants.h"
#include <math.h>

//------------------------------------------------------------ CONSTRUCTORS

ObjectBall::ObjectBall()
:	Object()
{
	radius = 0;
	slices = 0;
	stacks = 0;	
	speed  = 0;
}

ObjectBall::ObjectBall(GLdouble _radius, GLint _slices, GLint _stacks)
:   Object()
{
	radius = _radius;
	slices = _slices;
	stacks = _stacks;
	speed  = 0;
}


//------------------------------------------------------------ DESTRUCTORS

ObjectBall::~ObjectBall() { }

//------------------------------------------------------------ GETTERS & SETTERS

//------------------------------------------------------------ OTHER METHODS

void ObjectBall::updateState()
{
	if( speed )
	{
		setPos(getPosX() + (cos(RAD(direction)) * (speed/STATEUPDATES_PER_SEC)), 
			   getPosY(),
			   getPosZ() + (sin(RAD(direction)) * (speed/STATEUPDATES_PER_SEC)));  
		
		// update velocity
		speed *= 0.95;
		if(speed < 0.03) //lower bound
			speed = 0;
	}
}

void ObjectBall::applyForce( double magnitude, double _direction )
{
	// implement vectorial operations here for the case of the ball is already in movement //
	speed = magnitude;
	direction = _direction;
	
}
	
void ObjectBall::draw() const
{
	Object::drawBegin();
	
	glutSolidSphere(radius,slices,stacks);
	
	Object::drawEnd();
}
