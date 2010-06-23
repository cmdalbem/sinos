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
	/*
	 * Drawing of ball needs some adjusments on drawing time, i.e. multiplication
	 * by a special rotation matrix. So, Object::drawBegin() needed to 
	 * be overloaded for ObjectBall.
	 */
	void 				drawBegin() const;
	virtual void 		draw() const;
	bool 				updateState();
	void				applyForce( float magnitude, float direction );
	void				changeSpeed( float multFactor );
		
	/* updateRotateMatrix()
	 * 
	 * This function is golden. It will update rotMat with clean world, 
	 * coordinates. The matrix, when multiplied on drawBegin(), will
	 * allow us to take care of the ball's rotations using World 
	 * Coordinates - what seems to be the only way of making it the ball
	 * to rotate well when moving.
	 * It must be called in DisplayFunc before any transformation of
	 * the scene.
	 */
	void				updateRotateMatrix();
	
	/* resetRotateMatrix()
	 * 
	 * This will set primary values on the rotationMatrix.
	 */
	void				resetRotateMatrix();
	
	//---------------------------- POSITION DETECTION
	// TO DO: Study about moving these functions to an dedicated class.
	bool				hasSnooked();
	bool 				canMoveX();
	bool 				canMoveZ();
};
