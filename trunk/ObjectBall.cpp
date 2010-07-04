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
	res = 0;	
	hasFallen = false;
	resetSpeed();
	resetRotateMatrix();
	setQuad();
}

ObjectBall::ObjectBall( GLdouble _radius, int newRes )
:   ObjectModel()
{
	radius = _radius;
	res = newRes;
	hasFallen = false;
	resetSpeed();
	resetRotateMatrix();
	setQuad();
}

ObjectBall::ObjectBall(string filename)
:	ObjectModel(filename)
{
	radius = 0;
	res = 0;
	hasFallen = false;
	resetSpeed();	
	resetRotateMatrix();
	setQuad();
}


//------------------------------------------------------------ DESTRUCTORS

ObjectBall::~ObjectBall() { }

//------------------------------------------------------------ GETTERS & SETTERS

/*void ObjectBall::setSize (GLfloat x, GLfloat y, GLfloat z)
{
	Object::setSize(x,y,z);
	if(x==z && z==y)
		radius = x;
}*/

GLdouble ObjectBall::getRadius() const
{
	return radius;
}

float ObjectBall::getPerimeter() const
{
	return 2*M_PI*getRadius();
}

void ObjectBall::setProps( GLdouble newRadius, int newRes )
{
	setResolution(newRes);
	setRadius(newRadius);	
}

void ObjectBall::setResolution( int newRes )
{
	res = newRes;;
}

void ObjectBall::setRadius(GLdouble newRadius)
{
	radius = newRadius;
}

void ObjectBall::setDirection( double angle )
{
	double speed = getSpeed();
	
	resetSpeed();
	
	applyForce(speed,angle+90);
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

float ObjectBall::getFuturePos( float *futurePos[] ) const
{
	*futurePos[0] = getFutureX();
	*futurePos[1] = getFutureY();
	*futurePos[2] = getFutureZ();
}

float ObjectBall::getPastX() const
{
	return getPosX() - moveVector[0] / STATEUPDATES_PER_SEC; 
}

float ObjectBall::getPastZ() const
{
	return getPosX() - moveVector[2] / STATEUPDATES_PER_SEC; 
}

//------------------------------------------------------------ OTHER METHODS

void ObjectBall::backTrack( const double origv[3], bool invert /*default=false*/ )
{
	double v[3] = {origv[0],origv[1],origv[2]};
	int xsign, zsign;
		
	xsign = 2*( v[0]>0 )-1;
	zsign = 2*( v[2]>0 )-1;
	
	if(invert) {
		setPosX( getPosX() + xsign*BACKTRACK_STEP);
		setPosZ( getPosZ() + zsign*BACKTRACK_STEP); 
	}
	else {
		setPosX( getPosX() - xsign*BACKTRACK_STEP);
		setPosZ( getPosZ() - zsign*BACKTRACK_STEP);
	}
}

void ObjectBall::reflectAngle( double axisx, double axisy, double axisz )
{
	/* Formula for reflected angle is: 2*N(V.N)-V */
	
	double axis[3] = {axisx, axisy, axisz};
	double dot = dotProduct(axis,moveVector);

	double final[3] = {axisx, axisy, axisz};
	
	final[0] *= 2;
	//final[1] *= 2; //lets ignore the y reflection
	final[2] *= 2;
	
	final[0] *= dot;
	//final[1] *= dot;
	final[2] *= dot;
	
	final[0] -= moveVector[0];
	//final[1] -= moveVector[1];
	final[2] -= moveVector[2];
	
	moveVector[0] = -final[0];
	//moveVector[1] = -final[1];
	moveVector[2] = -final[2];
}

pair<bool,bool> ObjectBall::updateState()
/* Return: first  = if ball has moved
 *         second = if ball has been snooked
 */
{ 
	bool hasMoved = getSpeed() > 0.;
	bool hasSnookedBool = false;
	
	if( hasMoved )
	{
		// Update speed			
		changeSpeed(BALL_DECELERATION_N);
			
		// rotation by movement 
		setRot( RAD(((moveVector[2])*180.)/M_PI*BALL_RADIUS),
			    0,
			    RAD(-((moveVector[0])*180.)/M_PI*BALL_RADIUS));
		
		// update position
		setPos( getFutureX(), getFutureY(), getFutureZ() );
		
		// test if ball is falling
		if( !hasFallen ) { 
			if( hasSnooked() )
			{
				moveVector[1]=-30;
				hasFallen = true;
				hasSnookedBool = true;
			}
			else {
				
				
			}
		}
		else
			if( getPosY() > 1 ) { //until is over the ground
				moveVector[1] *= BALL_ACCELERATION_G;
				moveVector[0] *= BALL_DECELERATION_N;
				moveVector[2] *= BALL_DECELERATION_N;
			}
			else {
				//hasFallen = false;
				//setPos(getRandBetween(-10,10),TABLE_PLANE_Y+getRadius(),getRandBetween(-10,10));
				resetSpeed();
			}
	}
	
	return pair<bool,bool>::pair(hasMoved,hasSnookedBool);
}

void ObjectBall::applyForce( float magnitude, float direction, bool reflectAngle )
{
	if(reflectAngle) {
		moveVector[0] += magnitude * -1 * cos( RAD(direction) );
		moveVector[2] += -magnitude * -1 * sin( RAD(direction) );
	}
	else {
		moveVector[0] += magnitude * cos( RAD(direction) );	
		moveVector[2] += -magnitude * sin( RAD(direction) );
	}
}
	
void ObjectBall::changeSpeed( double factor ) 
{ 
	factor = pow(factor,1./STATEUPDATES_PER_SEC); 
	
	moveVector[0] *= factor;
	moveVector[1] *= factor;
	moveVector[2] *= factor;
	
	if( getSpeed() > BALL_MAX_SPEED) {
		factor = (BALL_MAX_SPEED-1)/getSpeed();
		
		moveVector[0] *= factor;
		moveVector[1] *= factor;
		moveVector[2] *= factor;
	}
		
	if( getSpeed() < BALL_MIN_SPEED )
		resetSpeed();
}

void ObjectBall::drawBegin() const
/* Overloading of original Object::drawBegin() */
{
	glPushMatrix();
	
	glTranslated(getPosX(), getPosY(), getPosZ());
	glScalef(getSizeX(), getSizeX(), getSizeX());
	glMultMatrixd(rotMat); 						//see updateRotateMatrix()
	    		
	material.apply();	
}

void ObjectBall::draw() const
{
	GLMmodel* modelPointer = getModelPointer();
	
	if( modelPointer )
	{
		ObjectBall::drawBegin();
			if( texture==NULL )
				glmDraw(modelPointer, GLM_SMOOTH);
			else
			{
				glBindTexture(GL_TEXTURE_2D, texture->texID);
				glmDraw(modelPointer, GLM_SMOOTH | GLM_TEXTURE);
			}
		Object::drawEnd();
	}
    else
    {
		ObjectBall::drawBegin();  
		if( texture==NULL )
			gluSphere(quadricSphere,radius,res,res);
		else
		{
			gluQuadricTexture(quadricSphere, GL_TRUE);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glBindTexture(GL_TEXTURE_2D, texture->texID);

			gluSphere(quadricSphere,radius,res,res);
			
			gluQuadricTexture(quadricSphere, GL_FALSE);			
		}
		Object::drawEnd();
     }	

}

void ObjectBall::drawVectors() const
{
	glDisable(GL_LIGHTING);
	
		glBegin(GL_LINES);
			glVertex3f(pos[0],pos[1],pos[2]);
			glVertex3f(pos[0] + moveVector[0],
					   pos[1],
					   pos[2] );
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(pos[0],pos[1],pos[2]);
			glVertex3f(pos[0],
					   pos[1],
					   pos[2]+ moveVector[2] );
		glEnd();
		

		glBegin(GL_LINES);
			glVertex3f(pos[0],pos[1],pos[2]);
			glVertex3f(pos[0]+ getSpeed()*cos(RAD(getDirection())),
					   pos[1],
					   pos[2]+ -getSpeed()*sin(RAD(getDirection())) );
		glEnd();	
	
	glEnable(GL_LIGHTING);
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

void ObjectBall::setQuad()
{
	quadricSphere = gluNewQuadric();
	gluQuadricNormals(quadricSphere, GL_SMOOTH);
	gluQuadricDrawStyle( quadricSphere, GLU_FILL);
    gluQuadricOrientation( quadricSphere, GLU_OUTSIDE);
}

//--------------------------------------------------- POSITION DETECTION

bool ObjectBall::hasSnooked()
{
	for(int i=0; i<NHOLES; i++)
		if( abs(pos[0]-HC[i][0]) + abs(pos[2]-HC[i][1]) < HC[i][2] )
			return true;
	return false;	
}

bool ObjectBall::isInField()
{
	if( pos[0]>LEFTBOUND && pos[0]<RIGHTBOUND  &&  
		pos[2]<TOPBOUND  && pos[2]>BOTBOUND )
		return true;
	else
		return false;
}

bool ObjectBall::collidedHWall()
{
	if( (pos[2] < wallLimits[4][0][1]  &&  pos[2] > wallLimits[4][1][1] &&
		pos[0] < LEFTBOUND)
		||
		(pos[2] < wallLimits[5][0][1]  &&  pos[2] > wallLimits[5][1][1] &&
		pos[0] > RIGHTBOUND) )
		return true;
	else
		return false;
} 

bool ObjectBall::collidedVWall()
{
	if( (pos[0] > wallLimits[0][0][0]  &&  pos[0] < wallLimits[0][1][0] &&
		pos[2] > TOPBOUND)
		||
		(pos[0] > wallLimits[1][0][0]  &&  pos[0] < wallLimits[1][1][0] &&
		pos[2] > TOPBOUND) 
		||
		(pos[0] > wallLimits[2][0][0]  &&  pos[0] < wallLimits[2][1][0] &&
		pos[2] < BOTBOUND) 
		||
		(pos[0] > wallLimits[3][0][0]  &&  pos[0] < wallLimits[3][1][0] &&
		pos[2] < BOTBOUND) )
		return true;
	else
		return false;
}
