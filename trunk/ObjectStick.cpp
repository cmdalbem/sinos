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
	setRotX(25);
	setRotY(-180);
	setRotZ(0);
	
	isHidden = false;
	
	setAttackStrenght(1);
}
ObjectStick::ObjectStick(string filename, ObjectBall* objectCenter)
:	ObjectModel(filename)
{
	setRotX(25);
	setRotY(-180);
	setRotZ(0);
	
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

float ObjectStick::getAngleInXZ()
{
	return angleInXZ;
}

void ObjectStick::setAngleInXZ(float newAngle)
{
	angleInXZ = newAngle;
	
	if(angleInXZ>360)
		angleInXZ = (int)angleInXZ % 360;
}

float ObjectStick::getAttackStrenght()
{
	return attackStrenght;
}

void ObjectStick::setAttackStrenght(float newForce)
{
	if ( newForce > 25 )
		attackStrenght = 25;
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
	if( !isHidden )
	{
		setAttackStrenght(attackStrenght + var);
		setRotX(25);
		calculatePos();
	}
}

void ObjectStick::calculatePos()
{
	double degrees = RAD(getAngleInXZ());
	
	setPosX(center->getPosX() + sin(degrees)*(getAttackStrenght()/2.));
	setPosY(center->getPosY() + cos(20)		*(getAttackStrenght()/2.));
	setPosZ(center->getPosZ() + cos(degrees)*(getAttackStrenght()/2.));
}

void ObjectStick::attack()
{
	setAttackStrenght(center->getRadius()+1);
	hide();
}

void ObjectStick::draw() const
{
	if(!isHidden)
		ObjectModel::draw();
}

void ObjectStick::hide()
{
	isHidden = true;
}

void ObjectStick::show()
{
	isHidden = false;
}
