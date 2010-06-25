// Game.h
//
#pragma once

#include <iostream>
using namespace std;

#include "Level.h"
#include "constants.h"


class Game
{
	private:
		char osd[2][1024];
		
	public:
		int		scorePlayer1, scorePlayer2;
		bool	currentPlayer;
		Level	*level;
		
		long int 	frameCounter, fps;	//frames per second counter and register
	
		//----------------------------------------------- CONSTRUCTORS
		Game();
		Game(Level *_level);
		
		//----------------------------------------------- DESTRUCTORS
		~Game();
		
		//----------------------------------------------- GETTERS & SETTERS
		void	setLevel(Level *newLevel);
		
		//----------------------------------------------- OTHER METHODS
		void	attack( int yvar );

		/* drawOsd()
		 * Draws On-Screen Display
		 * 	Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/bitfont.c
		 */
		void 	drawOsd();
		void 	updateOsd();
};
