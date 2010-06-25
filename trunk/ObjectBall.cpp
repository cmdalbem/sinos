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
#include "utils.h"

//------------------------------------------------------------ CONSTRUCTORS

ObjectBall::ObjectBall()
:	ObjectModel()
{
	radius = 0;
	slices = 0;
	stacks = 0;	
	resetSpeed();
	resetRotateMatrix();
}

ObjectBall::ObjectBall(GLdouble _radius, GLint _slices, GLint _stacks)
:   ObjectModel()
{
	radius = _radius;
	slices = _slices;
	stacks = _stacks;
	resetSpeed();
	resetRotateMatrix();
}

ObjectBall::ObjectBall(string filename)
:	ObjectModel(filename)
{
	radius = 0;
	slices = 0;
	stacks = 0;	
	resetSpeed();	
	resetRotateMatrix();
}


//------------------------------------------------------------ DESTRUCTORS

ObjectBall::~ObjectBall() { }

//------------------------------------------------------------ GETTERS & SETTERS

GLdouble ObjectBall::getRadius() const
{
	return radius;
}

float ObjectBall::getPerimeter() const
{
	return 2*M_PI*radius;
}


void ObjectBall::setStacks(GLint newStacks)
{
	stacks = newStacks;
}

void ObjectBall::setRadius(GLdouble newRadius)
{
	radius = newRadius;
	
	setSize(newRadius,newRadius,newRadius);
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
	
	setRot(0,0,0);
}

double ObjectBall::getDirection() const
{
	return getVectorAngle(moveVector);
}	

double ObjectBall::getSpeed() const
{
	return getVectorNorma(moveVector);
}

float ObjectBall::getFutureSpeed() const
{
	return sqrt( pow(moveVector[0]*BALL_DECELERATION_N,2) + pow(moveVector[1]*BALL_DECELERATION_N,2) + pow(moveVector[2]*BALL_DECELERATION_N,2) );	
}

float ObjectBall::getFutureX() const
{
	return getPosX() + moveVector[0] / STATEUPDATES_PER_SEC;
}

float ObjectBall::getFutureY() const
{
	return getPosY() + moveVector[1] / STATEUPDATES_PER_SEC;
}

float ObjectBall::getFutureZ() const
{
	return getPosZ() + moveVector[2] / STATEUPDATES_PER_SEC;
}

float ObjectBall::getPastX() const
{
	return pastPosXZ[0];
}

float ObjectBall::getPastZ() const
{
	return pastPosXZ[2];
}

float ObjectBall::getFuturePos( float *futurePos[] ) const
{
	*futurePos[0] = getFutureX();
	*futurePos[1] = getFutureY();
	*futurePos[2] = getFutureZ();
}

void ObjectBall::setSize (GLfloat x, GLfloat y, GLfloat z)
{
	Object::setSize(x,y,z);
	if(x==z && z==y)
		radius = x/10;
}

//------------------------------------------------------------ OTHER METHODS

bool ObjectBall::updateState()
{
	if( getSpeed() )
	{
		pastPosXZ[0] = getPosX();
		pastPosXZ[1] = getPosZ(); 
		
		// rotation by movement
		setRot( moveVector[2] ,0, -moveVector[0] );
		
		// update position
		setPos( getFutureX(), getFutureY(), getFutureZ() );
		
		// test if ball is falling
		if( !moveVector[1] ) { 
			bool can_move_x = testHorizontalBound(),
				 can_move_z = testVerticalBound();

			if( hasSnooked() )
			{
				cout<<"point!"<<endl;
				moveVector[1]=-30;
			}
			else {
				if( !can_move_x ) { //invert x
					moveVector[0] = -moveVector[0];
					changeSpeed(BALL_DECELERATION_R);
				}
				if( !can_move_z ) { //invert z
					moveVector[2] = -moveVector[2];
					changeSpeed(BALL_DECELERATION_R);
				}
			}
						
			changeSpeed(BALL_DECELERATION_N);
			
			if( getSpeed() < 0.3 )
				resetSpeed();
		}
		else
			if( getPosY() > 1 ) {
				moveVector[1] *= BALL_ACCELERATION_G;
				moveVector[0] *= BALL_DECELERATION_N;
				moveVector[2] *= BALL_DECELERATION_N;
			}
			else { //fell to the ground
				resetSpeed();
				setPos(0,TABLE_PLANE_Y+radius,0);
			}
		return true;
	}
	return false;
}

void ObjectBall::applyForce( float magnitude, float direction )
{
	moveVector[0] += magnitude * cos( RAD(direction) );
	moveVector[2] += -magnitude * sin( RAD(direction) );
}
	
void ObjectBall::changeSpeed( float factor )
{
	moveVector[0] *= factor;
	moveVector[1] *= factor;
	moveVector[2] *= factor;
}

void ObjectBall::drawBegin() const
/* Overloading of original Object::drawBegin() */
{
	glPushMatrix();
	
	glTranslated(getPosX(), getPosY(), getPosZ());
	glScalef(getSizeX(), getSizeY(), getSizeZ());
	
	glMultMatrixd(rotMat); 						//see updateRotateMatrix()
	    		
	material.apply();	
}

void ObjectBall::draw() const
{
	GLMmodel* modelPointer = getModelPointer();
	
	if( modelPointer )
	{
		ObjectBall::drawBegin();
		if( texture!=NULL )
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture->texID);
			glmDraw(modelPointer, GLM_SMOOTH | GLM_TEXTURE);
			glDisable(GL_TEXTURE_2D);
		}
		else
			glmDraw(modelPointer, GLM_SMOOTH);
		Object::drawEnd();
	}

}

void ObjectBall::updateRotateMatrix()
/* Adapted from OpenGL Screen & Object Space Rotations - MaxH - Mike Werner April 28, 2009 */
{
   glLoadIdentity ();
   glRotatef (getRotX(), 1,0,0);
   glRotatef (getRotY(), 0,1,0);
   glRotatef (getRotZ(), 0,0,1);
   glMultMatrixd (rotMat);
   glGetDoublev (GL_MODELVIEW_MATRIX, rotMat);
}

void ObjectBall::resetRotateMatrix()
{
	rotMat[0]=1;
	rotMat[1]=0;
	rotMat[2]=0;
	rotMat[3]=0;
	rotMat[4]=0;
	rotMat[5]=1;
	rotMat[6]=0;
	rotMat[7]=0;
	rotMat[8]=0;
	rotMat[9]=0;
	rotMat[10]=1;
	rotMat[11]=0;
	rotMat[12]=0;
	rotMat[13]=0;
	rotMat[14]=0;
	rotMat[15]=1;
}

//--------------------------------------------------- POSITION DETECTION

bool ObjectBall::hasSnooked()
{
	float posx = getPosX(),
		  posy = getPosY(),
		  posz = getPosZ();

	for(int i=0; i<NHOLES; i++)
		if( abs(posx-HC[i][0]) + abs(posz-HC[i][1]) <= HC[i][2] )
			return true;
	return false;
	
	/*// Limit Boundaries Test
	if( posx>B2P1[0] && posx<B2P2[0] &&
		posy>B2P1[1] )
		return true;
	else if( posx>B5P1[0] && posx<B5P2[0] &&
			 posy<B5P1[1] )
			return true;
	else return false;*/
	
}

bool ObjectBall::testHorizontalBound()
{
	float posx = getPosX();
	
	if( posx > RIGHTBOUND  ||  posx < LEFTBOUND )
		return false;
	else
		return true;
}

bool ObjectBall::testVerticalBound()
{
	float posz = getPosZ();
	
	if( posz > TOPBOUND  ||  posz < BOTBOUND )
		return false;
	else
		return true;
}
