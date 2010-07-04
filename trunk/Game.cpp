// Game.cpp
//

#include "Game.h"

//--------------------------------------------------- CONSTRUCTORS
Game::Game()
{
	currentPlayer = 1;
	fps=0;
	resetPoints();
	hasControl = true;
	
	level = NULL;
}

Game::Game(Level *_level)
{
	currentPlayer = 1;
	fps=0;
	resetPoints();
	hasControl = true;
	
	level = _level;
}

//--------------------------------------------------- DESTRUCTORS
Game::~Game() {   }

//--------------------------------------------------- GETTERS & SETTERS
void Game::setLevel(Level *newLevel)
{
	level = newLevel;
}

void Game::resetPoints()
{
	for(int i=0; i<NPLAYERS; i++)
		points[i] = 0;
}

void Game::giveControl()
{
	if( hasControl == false )
	{
		hasControl = true;
		nextPlayer();
		
		level->stick.setCenter(&level->balls[0]);
		level->stick.isHidden = false;
	}
}

//--------------------------------------------------- OTHER METHODS
void Game::attack( int yvar )
{
	if( hasControl ) {
		level->balls[0].applyForce( yvar, level->stick.rot[1]+90 );  //some naughty magic numbers here
		level->stick.attack();
		hasControl = false;
	}
}

void Game::updateState()
{
	pair<int, bool> returnValue = level->updateState();
	
	if( returnValue.first ) {//if player has scored
		points[currentPlayer-1] += returnValue.first;
		updateOsd();
	}
	
	if( !returnValue.second ) //if nothing moved
	{
		giveControl();
	}
}

void Game::nextPlayer()
{
	currentPlayer++;
	if(currentPlayer>NPLAYERS) currentPlayer=1;
	//cout << "Vez do jogador " << currentPlayer << endl;
}

void Game::drawOsd()
/* Draws On-Screen Display */
/* Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/bitfont.c */
{
	float y=0.5;
  
	glDisable(GL_LIGHTING);
  
	glColor3f(1.0f, 1.0f, 1.0f);
	for( int line=0; line<OSD_LINES; line++ )
	{
		glRasterPos3f(-1, y, -1);
		for( int ch = 0; ch < (int)strlen(osd[line]); ch++) 
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, osd[line][ch]);
		y-=0.05;
	}
   
	glEnable(GL_LIGHTING);
}

void Game::updateOsd()
{
	// OSD setup
	sprintf(osd[0],"FPS: %li ",fps);
	for( int i=0; i<NCAMERAMODES; i++ )
		if( level->camera->getMode() == i )
			sprintf(osd[0],"%s - [%s]",osd[0],level->camera->getModeName(i));
		else
			sprintf(osd[0],"%s -  %s ",osd[0],level->camera->getModeName(i));
		
	for( int i=0; i<NPLAYERS; i++)
	{
		if( i == currentPlayer-1 )
			sprintf(osd[i+1],"->");
		else
			sprintf(osd[i+1],"     ");
		
		sprintf(osd[i+1],"%s Player %i: %i",osd[i+1],i+1,points[i]);
	}
}
