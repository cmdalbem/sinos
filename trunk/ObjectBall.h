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
	float moveVector[3]; //direction of movement
	

public:
	//---------------------------- CONSTRUCTORS
	ObjectBall();
	ObjectBall(GLdouble radius, GLint slices, GLint stacks);

	//---------------------------- DESTRUCTORS
	~ObjectBall();
	
	//---------------------------- GETTERS & SETTERS
	void 				resetSpeed();
	float				getSpeed();
	float				getNewX();
	float				getNewY();
	float				getNewZ();
	
	//---------------------------- OTHER METHODS
	virtual void 		draw() const;
	void 				updateState();
	void				applyForce( float magnitude, float direction );
	void				changeSpeed( float multFactor );
	
	//---------------------------- POSITION DETECTION
	bool				hasSnooked();
	bool 				canMoveX();
	bool 				canMoveZ();
};
