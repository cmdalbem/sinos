// ObjectBall.h
//
// This is an Object that is a snooker ball.
// Has special features like velocity of movement, and it's position
// is eventually used to control the stick and special cameras.

#pragma once

#include <iostream>
#include <string>
#include "lib/glm.h"
#include "Object.h"

class ObjectBall: public Object
{
private:
	GLdouble radius;
	GLint slices, stacks;
	double speed, direction; //direction is an angle

public:
	//---------------------------- CONSTRUCTORS
	ObjectBall();
	ObjectBall(GLdouble radius, GLint slices, GLint stacks);

	//---------------------------- DESTRUCTORS
	~ObjectBall();
	
	//---------------------------- GETTERS & SETTERS

	
	//---------------------------- OTHER METHODS
	virtual void 		draw() const;
	void 				updateState();
	void				applyForce( double magnitude, double direction );
};
