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

void ObjectBall::setSize (GLfloat x, GLfloat y, GLfloat z)
{
	Object::setSize(x,y,z);
	if(x==z && z==y)
		radius = x;
}

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

void ObjectBall::backTrack( double v[3], bool invert /*default=false*/ )
{
	double tempv[3];
	tempv[0] = v[0];
	tempv[1] = v[1];
	tempv[2] = v[2];
	
	normalizeVector(v);
	
	if(invert) {
		setPosX( getPosX() + v[0]/20.);
		setPosZ( getPosZ() + v[2]/20.); 
	}
	else {
		setPosX( getPosX() - v[0]/20.);
		setPosZ( getPosZ() - v[2]/20.);
	}
	
	v[0] = tempv[0];
	v[1] = tempv[1];
	v[2] = tempv[2];
}

//------------------------------------------------------------ OTHER METHODS

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
		/*setRot( (((moveVector[2]/STATEUPDATES_PER_SEC)*360.)/getPerimeter()),
			    0,
			    (-((moveVector[0]/STATEUPDATES_PER_SEC)*360.)/getPerimeter()));*/
		setRot( RAD((moveVector[2]*180.)/M_PI*1.),
			    0,
			    RAD(-(moveVector[0]*180.)/M_PI*1.));
		
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
				
				if( !canMoveHorizontal() ) { //invert x
					
					int rep=0;
					while( !canMoveHorizontal() && rep++ < MAX_BACKTRACK ) {
						backTrack(moveVector);
						/*if(rep>10000){
							trouble=1;
							troublePos[0] = getPosX();
							troublePos[1] = getPosY();
							troublePos[2] = getPosZ();
							glutPostRedisplay();							
							getchar();
							cout<<"horizontal trouble: "<<moveVector[0]<<" "<<moveVector[2]<<endl;
						}*/
					}
					moveVector[0] = -moveVector[0];   //reflection
					changeSpeed(BALL_DECELERATION_R); //absortion of energy by the wall
				}
				
				if( !canMoveVertical() ) { //invert z
					
					int rep=0;
					while( !canMoveVertical() && rep++ < MAX_BACKTRACK ) {
						backTrack(moveVector);
						/*if(rep>10000){
							trouble=1;
							troublePos[0] = getPosX();
							troublePos[1] = getPosY();
							troublePos[2] = getPosZ();
							glutPostRedisplay();								
							getchar();
							cout<<"vertical trouble: "<<moveVector[0]<<" "<<moveVector[2]<<endl;
						}*/
					}
					moveVector[2] = -moveVector[2];   //reflection
					changeSpeed(BALL_DECELERATION_R); //absortion of energy by the wall
				}
			}
		}
		else
			if( getPosY() > 1 ) { //until is over the ground
				moveVector[1] *= BALL_ACCELERATION_G;
				moveVector[0] *= BALL_DECELERATION_N;
				moveVector[2] *= BALL_DECELERATION_N;
			}
			else {
				hasFallen = false;
				setPos(getRandBetween(-10,10),TABLE_PLANE_Y+getRadius(),getRandBetween(-10,10));
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
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			
			glBindTexture(GL_TEXTURE_2D, texture->texID);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			gluSphere(quadricSphere,radius,res,res);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
			
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);				
		}
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

void ObjectBall::setQuad()
{
	quadricSphere = gluNewQuadric();
	gluQuadricNormals(quadricSphere, GL_SMOOTH);
	gluQuadricTexture(quadricSphere, GL_TRUE);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
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

bool ObjectBall::canMoveHorizontal()
{
	float posx = getPosX();
	
	if( posx > RIGHTBOUND  ||  posx < LEFTBOUND )
		return false;
	else
		return true;
}

bool ObjectBall::canMoveVertical()
{
	float posz = getPosZ();
	
	if( posz > TOPBOUND  ||  posz < BOTBOUND )
		return false;
	else
		return true;
}
