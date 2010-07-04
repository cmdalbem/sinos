// ObjectStick.cpp
//
// Implementations of the functions in ObjectStick.h
//

#include "ObjectStick.h"
#include "constants.h"

//------------------------------------------------------------ CONSTRUCTORS
ObjectStick::ObjectStick()
: ObjectModel()
{
	rot[0] = -10;
	isHidden = false;
	setAttackStrenght(1);
}

ObjectStick::ObjectStick(string filename, ObjectBall* objectCenter)
:	ObjectModel(filename)
{
	rot[0] = -10;
	isHidden = false;
	center = objectCenter;
	setAttackStrenght(center->getRadius()+1);
	calculatePos();
}

//------------------------------------------------------------ DESTRUCTORS
ObjectStick::~ObjectStick()	{ }

//------------------------------------------------------------ GETTERS & SETTERS
void ObjectStick::setCenter(ObjectBall* newCenter)
{
	center = newCenter;
	calculatePos();
}

float ObjectStick::getAttackStrenght() const
{
	return attackStrenght;
}

void ObjectStick::setAttackStrenght(float newForce)
{
	if ( newForce > STICK_MAX_STR )
		attackStrenght = STICK_MAX_STR;
	else
		attackStrenght = newForce;
}

//------------------------------------------------------------ OTHER METHODS

void ObjectStick::rotate( float rotat )
{
	rot[1] += rotat;	
	calculatePos();
}

void ObjectStick::changePower( float var )
{
	if( !isHidden )
	{
		setAttackStrenght(attackStrenght + var);
		calculatePos();
	}
}

void ObjectStick::calculatePos()
{
	double roty = RAD(rot[1]),
		   rotx = RAD(rot[0]);
	
	setPosX(center->getPosX() + sin(roty)*(getAttackStrenght()/2.));
	setPosY(center->getPosY() - sin(rotx)*(getAttackStrenght()/2.));
	setPosZ(center->getPosZ() + cos(roty)*(getAttackStrenght()/2.));
}

void ObjectStick::attack()
{
	setAttackStrenght(center->getRadius()+1);
	isHidden = true;
}

void ObjectStick::drawBegin() const
/* Overloading of original Object::drawBegin() */
{
	glPushMatrix();
	
	glTranslated(getPosX(), getPosY(), getPosZ());
	glScalef(getSizeX(), getSizeX(), getSizeX());
	glRotatef (rot[1], 0,1,0);
    glRotatef (rot[0], 1,0,0);
	    		
	material.apply();	
}

void ObjectStick::draw() const
{
	if(!isHidden)
	{
		if(getModelPointer())
		{
			ObjectStick::drawBegin();
			if( texture!=NULL )
			{
				glBindTexture(GL_TEXTURE_2D, texture->texID);
				glmDraw(getModelPointer(), GLM_SMOOTH | GLM_TEXTURE);
			}
			else
				glmDraw(getModelPointer(), GLM_SMOOTH);
			drawEnd();
		}
	}
}
