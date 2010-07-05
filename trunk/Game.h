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
		char osd[OSD_LINES][1024];
		
	public:
		int		points[NPLAYERS];
		int		currentPlayer;
		Level	*level;
		bool 	hasControl;
		
		bool	wrongBallFlag;
		bool	inSinucaMode;
		
		long int 	frameCounter, fps;	//frames per second counter and register
	
		//----------------------------------------------- CONSTRUCTORS
		Game();
		Game(Level *_level);
		
		//----------------------------------------------- DESTRUCTORS
		~Game();
		
		//----------------------------------------------- GETTERS & SETTERS
		void	setSinucaMode(bool _sinuca);
		void	setLevel(Level *newLevel);
		void	resetPoints();
		void	giveControl();
		
		//----------------------------------------------- OTHER METHODS
		void	attack( int yvar );
		void	updateState();
		void	nextPlayer();
		
		/* drawOsd()
		 * Draws On-Screen Display
		 * 	Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/bitfont.c
		 */
		void 	drawOsd();
		void 	updateOsd();
};
