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

void ObjectBall::resetSpeed()
{
	moveVector[0] = 0;
	moveVector[1] = 0;
	moveVector[2] = 0;
}

float ObjectBall::getSpeed()
{
	return abs(moveVector[0]) /*+ abs(moveVector[1])*/ + abs(moveVector[2]);	
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
		bool can_move_x = canMoveX(),
			 can_move_z = canMoveZ();

		if( !can_move_x || !can_move_z ) {
			if( hasSnooked() ) {
				setPos(0,BALL_O_Y,0);
				resetSpeed();
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
		setPos( getNewX(), getPosY(), getNewZ() );
		
		// update velocity
		changeSpeed(0.965);
		if( getSpeed() < 0.05) //magic number detected
			resetSpeed();
	}
}

void ObjectBall::applyForce( float magnitude, float direction )
{
	moveVector[0] += magnitude * cos( RAD(direction) );
	moveVector[2] += -magnitude * sin( RAD(direction) );
	
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
	/*	
const float B1P1[] = {-48.3, -19.9};
const float B1P2[] = {-45  , -23.5};
const float B2P1[] = {-2.4 , -23.5};
const float B2P2[] = { 2.1 , -23.5};
const float B3P1[] = { 44.7, -23.5};
const float B3P2[] = { 48  , -19.9};
const float B4P1[] = {-48.3,  24.8};
const float B4P2[] = {-45  ,  28.1};
const float B5P1[] = {-2.4 ,  28.1};
const float B5P2[] = { 1.8 ,  28.1};
const float B6P1[] = { 48  ,  24.5};
const float B6P2[] = { 44.4,  28.1};
	*/

	float posx = getNewX(),
		  posy = getPosY(),
		  posz = getNewZ();

	if( posx>B2P1[0] && posx<B2P2[0] &&
		posy>B2P1[1] )
		return true;
	else if( posx>B5P1[0] && posx<B5P2[0] &&
			 posy<B5P1[1] )
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
