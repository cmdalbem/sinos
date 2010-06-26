// Game.cpp
//

#include "Game.h"

//--------------------------------------------------- CONSTRUCTORS
Game::Game()
{
	currentPlayer = 0;
	scorePlayer1  = 0;
	scorePlayer2  = 0;
	fps=0;
	
	level	= NULL;
}

Game::Game(Level *_level)
{
	currentPlayer = 0;
	scorePlayer1  = 0;
	scorePlayer2  = 0;
	fps=0;
	
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
void Game::attack( int yvar )
{
	if( !level->stick.isHidden ) {
		level->balls[0].applyForce(yvar,level->stick.getAngleInXZ()+90);  //some naughty magic numbers here
		level->stick.attack();
		
		currentPlayer = (currentPlayer+1) % NPLAYERS;
		cout << "Vez do jogador " << currentPlayer+1 << endl;
	}
}

void Game::drawOsd()
/* Draws On-Screen Display */
/* Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/bitfont.c */
{
  float y=0.5;
  
  glDisable(GL_LIGHTING);
  
  glColor3f(1.0f, 1.0f, 1.0f);
  for( int line=0; line<2; line++ )
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
		
  sprintf(osd[1],"Current Player: %i ",currentPlayer);
}
