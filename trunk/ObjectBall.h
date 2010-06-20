// ObjectBall.h
//
// This is an Object that is a snooker ball.
// Has special features like velocity of movement, and it's position
// is eventually used to control the stick and special cameras.

#pragma once

#include <iostream>
#include <string>
#include "lib/glm.h"
#include "ObjectModel.h"

class ObjectBall: public ObjectModel 
{
private:
	GLdouble radius;
	GLint slices, stacks;
	float moveVector[3]; //direction of movement
	
public:
	GLdouble *rotMat[16];
	
	//---------------------------- CONSTRUCTORS
	ObjectBall();
	ObjectBall(GLdouble radius, GLint slices, GLint stacks);
	ObjectBall(string filename);

	//---------------------------- DESTRUCTORS
	~ObjectBall();
	
	//---------------------------- GETTERS & SETTERS

	void				setSize (GLfloat x, GLfloat y, GLfloat z); //overloads Object's function to update Ball's radius
	float				getDirection() const;
	GLdouble			getRadius() const;
	float				getPerimeter() const;

	void				setStacks(GLint newStacks);
	void				setSlices(GLint newSlices);
	void				setRadius(GLdouble newRadius);
	
	void 				resetSpeed();
	float				getSpeed() const;
	float				getFutureSpeed() const;
	float				getNewX() const;
	float				getNewY() const;
	float				getNewZ() const;
	
	//---------------------------- OTHER METHODS
	void 				drawBegin() const;
	virtual void 		draw() const;
	void 				updateState();
	void				applyForce( float magnitude, float direction );
	void				changeSpeed( float multFactor );
	void				updateRotateMatrix();
	void				resetRotateMatrix();
	
	//---------------------------- POSITION DETECTION
	bool				hasSnooked();
	bool 				canMoveX();
	bool 				canMoveZ();
};
