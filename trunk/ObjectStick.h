// ObjectStick.h
//
// This is the stick that will hit the ball. On the game, we will have
// two of these. The idea is to have one to each player.
// The stick is an ObjectModel, since it is loaded from an object file,
// but also has its own features, like the attack force with which is
// goint to hit the ball, for example.
//
#pragma once

#include <iostream>
using namespace std;

#include "ObjectModel.h"
#include "ObjectBall.h"

class ObjectStick : public ObjectModel
{
	private:
		ObjectBall*	center;
		float 		attackStrenght;
		
	public:
		bool		isHidden;
	
		//---------------------------- CONSTRUCTORS
		ObjectStick();
		ObjectStick(string filename, ObjectBall* objectCenter);
		
		//---------------------------- DESTRUCTORS
		
		~ObjectStick();
		//---------------------------- GETTERS & SETTERS
		void	setCenter(ObjectBall* newCenter);
		
		float	getAttackStrenght() const;
		void	setAttackStrenght(float newForce);
		
		//---------------------------- OTHER METHODS
		/* rotateLeft() & rotateRight()
		 * 
		 * Move the stick left or right using rotations centering on the
		 * object "center" (the instance variable we have on ObjectStick
		 * class).
		 */
		void			rotate( float rotation );
		void			changePower( float var );
		
		/* CalculatePos()
		 * 
		 * After the movement, the new position of the Stick is calculated
		 * and put in the position variables called pos ([0], [1] and [2]).
		 */
		void			calculatePos();
		void 			attack();
		void  			drawBegin() const;
		virtual void	draw() const;
};
