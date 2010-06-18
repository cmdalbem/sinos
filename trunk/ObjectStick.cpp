// ObjectStick.cpp
//
// Implementations of the functions in ObjectStick.h
//

#include "ObjectStick.h"
#include "constants.h"

float movex = 5, movey = 5,
	  moreStrenght = 0.1,
	  rotationConstant = 180;

//------------------------------------------------------------ CONSTRUCTORS
ObjectStick::ObjectStick(string filename, Object* objectCenter)
:	ObjectModel(filename)
{
	center = objectCenter;
	setAttackStrenght(0.5);
	setRotZ(10 - attackStrenght);
	calculatePos();
}

//------------------------------------------------------------ DESTRUCTORS
ObjectStick::~ObjectStick()	{ }

//------------------------------------------------------------ GETTERS & SETTERS
float ObjectStick::getAttackStrenght()
{
	return attackStrenght;
}

void ObjectStick::setAttackStrenght(float newForce)
{
	if( newForce < 0.5 )
		attackStrenght = 0.5;
	else if ( newForce > 20 )
		attackStrenght = 20;
	else
		attackStrenght = newForce;
}

//------------------------------------------------------------ OTHER METHODS

void ObjectStick::rotateLeft()
{
	setRotX(getRotX() - movex);
	setRotY(getRotY() - movey);
	
	calculatePos();
}

void ObjectStick::rotateRight()
{
	setRotX(getRotX() + movex);
	setRotY(getRotY() + movey);
	
	calculatePos();
}

void ObjectStick::strenghtUp()
{
	setAttackStrenght(attackStrenght + moreStrenght);
	setRotZ(10 - getAttackStrenght()/5);
	
	calculatePos();
}

void ObjectStick::strenghtDown()
{
	setAttackStrenght(attackStrenght - moreStrenght);
	setRotZ(10 - getAttackStrenght()/5);
	
	calculatePos();
}

void ObjectStick::calculatePos()
{
	double degrees = RAD(getRotX());
	
	setPosX(center->getPosX() + sin(degrees)*(attackStrenght/5));
	setPosY(center->getPosY() + 0.02*(attackStrenght));
	setPosZ(center->getPosZ() + cos(degrees)*(attackStrenght/5));
}

void ObjectStick::drawBegin()
{
	glPushMatrix();
	
	material.apply();
	
	// basic transformations using object's attributes
	glTranslated(getPosX(), getPosY(), getPosZ());
	glRotatef(getRotY() - rotationConstant,0,1,0);
	glRotatef(getRotZ()		 ,1,0,0);		// Here, the name "rotz" is only a name. It is used
											// to set the inclination of the stick.
	glScalef(getSizeX(), getSizeY(), getSizeZ());
}

void ObjectStick::draw()
{
	if(getModelPointer())	// Test if there is anything to draw
	{
		drawBegin();		// Move, scale and rotate the object to the right place
		if( texture!=NULL )
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture->texID);
			glmDraw(getModelPointer(), GLM_SMOOTH | GLM_TEXTURE);
			glDisable(GL_TEXTURE_2D);
		}
		else
			glmDraw(getModelPointer(), GLM_SMOOTH);
		Object::drawEnd();
		
	}
	else
		cout << "There is nothing to draw D= D=" << endl;
		//exit(0);
}

