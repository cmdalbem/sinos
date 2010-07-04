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
	int res;
	GLUquadricObj *quadricSphere;
	GLdouble rotMat[16];
	
	
public:
	double moveVector[3]; //direction of movement
	bool hasFallen;
	
	// DEBUG
	int trouble;
	double troublePos[3];
	
	//---------------------------- CONSTRUCTORS
	ObjectBall();
	ObjectBall(GLdouble radius, int resolution);
	ObjectBall(string filename);

	//---------------------------- DESTRUCTORS
	~ObjectBall();
	
	//---------------------------- GETTERS & SETTERS

	// PHISICAL STRUCTURE
	void				setSize (GLfloat x, GLfloat y, GLfloat z); //overloads Object's function to update Ball's radius
	GLdouble			getRadius() const;
	float				getPerimeter() const;

	void				setProps( GLdouble newRadius, int newRes );
	void				setResolution( int newRes );
	void				setRadius(GLdouble newRadius);
	void				setDirection( double angle );
	void 				resetSpeed();
	
	// BEHAVIOUR
	double				getDirection() const;
	double				getSpeed() const;
	float				getFutureX() const;
	float				getFutureY() const;
	float				getFutureZ() const;
	float				getFuturePos( float *futurePos[] ) const;
	float				getPastX() const;
	float				getPastZ() const;
	
	void				backTrack( const double v[3], bool invertMovement=false );
	void				reflectAngle( double axisx, double axisy, double axisz );
	
	
	//---------------------------- OTHER METHODS
	/*
	 * Drawing of ball needs some adjusments on drawing time, i.e. multiplication
	 * by a special rotation matrix. So, Object::drawBegin() needed to 
	 * be overloaded for ObjectBall.
	 */
	void 				drawBegin() const;
	virtual void 		draw() const;
	void				drawVectors() const;
	pair<bool,bool>		updateState();
	void				applyForce( float magnitude, float direction, bool reflectAngle=false );
	void				changeSpeed( double factor );
		
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
	void 				setQuad();
	
	//---------------------------- POSITION DETECTION
	// TO DO: Study about moving these functions to an dedicated class.
	bool				hasSnooked();
	bool				isInField();
	bool 				collidedHWall();
	bool 				collidedVWall();
};
