// Game.cpp
//

#include "Game.h"

//--------------------------------------------------- CONSTRUCTORS
Game::Game()
{
	currentPlayer = 0;
	scorePlayer1  = 0;
	scorePlayer2  = 0;
	
	level	= NULL;
}

Game::Game(Level *_level)
{
	currentPlayer = 0;
	scorePlayer1  = 0;
	scorePlayer2  = 0;
	
	level = _level;
}

//--------------------------------------------------- DESTRUCTORS
Game::~Game() {   }

//--------------------------------------------------- GETTERS & SETTERS
void Game::setLevel(Level *newLevel)
{
	level = newLevel;
}

//--------------------------------------------------- OTHER METHODS
void Game::attack(int y, int yold)
{
	level->ball.applyForce((yold-y),level->stick.getAngleInXZ()+90);  //some naughty magic numbers here
	level->stick.attack();
	
	currentPlayer = (currentPlayer+1) % NPLAYERS;
	cout << "Vez do jogador " << currentPlayer+1 << endl;
}
