// ObjectStick.h
//
// This is the stick that will hit the ball. On the game, we will have
// two of these. The idea is to have one to each player.
// The stick is an ObjectModel, since it is loaded from an object file,
// but also has its own features, like the attack force with which is
// goint to hit the ball, for example.
//


#include <iostream>
using namespace std;

#include "ObjectModel.h"
#include "ObjectBall.h"

class ObjectStick : public ObjectModel
{
	private:
		Object*		center;
		float 		attackStrenght;
		
	public:
		//---------------------------- CONSTRUCTORS
		ObjectStick(string filename, Object* objectCenter);
		
		//---------------------------- DESTRUCTORS
		
		~ObjectStick();
		//---------------------------- GETTERS & SETTERS
		float	getAttackStrenght();
		void	setAttackStrenght(float newForce);
		
		//---------------------------- OTHER METHODS
		void	drawBegin();
		void	draw();
		/* rotateLeft() & rotateRight()
		 * 
		 * Move the stick left or right using rotations centering on the
		 * object "center" (the instance variable we have on ObjectStick
		 * class).
		 */
		void	rotate( float rotation );

		void	strenghtUp();
		void	strenghtDown();
		
		/* CalculatePos()
		 * 
		 * After the movement, the new position of the Stick is calculated
		 * and put in the position variables called pos ([0], [1] and [2]).
		 */
		void	calculatePos();
};
