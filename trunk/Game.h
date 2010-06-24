// Game.h
//
#pragma once

#include <iostream>
using namespace std;

#include "Level.h"
#include "constants.h"


class Game
{
	public:
		int		scorePlayer1, scorePlayer2;
		bool	currentPlayer;
		Level	*level;
	
		//----------------------------------------------- CONSTRUCTORS
		Game();
		Game(Level *_level);
		
		//----------------------------------------------- DESTRUCTORS
		~Game();
		
		//----------------------------------------------- GETTERS & SETTERS
		void	setLevel(Level *newLevel);
		
		//----------------------------------------------- OTHER METHODS
		void	attack(int y, int yold);
};
