// ObjectStick.cpp
//
// Implementations of the functions in ObjectStick.h
//

#include "ObjectStick.h"
#include "constants.h"

//------------------------------------------------------------ CONSTRUCTORS
ObjectStick::ObjectStick(string filename, Object* objectCenter)
:	ObjectModel(filename)
{
	setRotX(25);
	setRotY(-180);
	setRotZ(0);
	
	center = objectCenter;
	
	setAttackStrenght(1);
	calculatePos();
}

//------------------------------------------------------------ DESTRUCTORS
ObjectStick::~ObjectStick()	{ }

//------------------------------------------------------------ GETTERS & SETTERS
float ObjectStick::getAngleInXZ()
{
	return angleInXZ;
}

void ObjectStick::setAngleInXZ(float newAngle)
{
	angleInXZ = newAngle;
}

float ObjectStick::getAttackStrenght()
{
	return attackStrenght;
}

void ObjectStick::setAttackStrenght(float newForce)
{
	if( newForce < 1 )
		attackStrenght = 1;
	else if ( newForce > 20 )
		attackStrenght = 20;
	else
		attackStrenght = newForce;
}

//------------------------------------------------------------ OTHER METHODS

void ObjectStick::rotate( float rot )
{
	setAngleInXZ(getAngleInXZ() + rot);
	setRotY(getRotY() + rot);
	
	calculatePos();
}

void ObjectStick::changePower( float var )
{
	setAttackStrenght(attackStrenght + var);
	setRotX(25);
	
	calculatePos();
}

void ObjectStick::calculatePos()
{
	double degrees = RAD(getAngleInXZ());
	
	setPosX(center->getPosX() + sin(degrees)*(getAttackStrenght()));
	setPosY(center->getPosY() + cos(20)		*(getAttackStrenght()));
	setPosZ(center->getPosZ() + cos(degrees)*(getAttackStrenght()));
}
