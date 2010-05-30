// ObjectBall.cpp
//
// Function implementation of ObjectBall.h
//

#include <iostream>
#include "ObjectBall.h"

//------------------------------------------------------------ CONSTRUCTORS

ObjectBall::ObjectBall()
:	Object()
{
	radius = 0;
	slices = 0;
	stacks = 0;	
}

ObjectBall::ObjectBall(GLdouble _radius, GLint _slices, GLint _stacks)
:   Object()
{
	radius = _radius;
	slices = _slices;
	stacks = _stacks;
}


//------------------------------------------------------------ DESTRUCTORS

ObjectBall::~ObjectBall() { }

//------------------------------------------------------------ GETTERS & SETTERS

void ObjectBall::draw() const
{
	Object::drawBegin();
	
	glutSolidSphere(radius,slices,stacks);
	
	Object::drawEnd();
}
