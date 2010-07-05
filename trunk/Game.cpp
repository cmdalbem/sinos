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
	
	wrongBallFlag = true;
	wrongBallFallenFlag = false;
	
	hasEnded = false;
	
	level = NULL;
}

Game::Game(Level *_level)
{
	currentPlayer = 1;
	fps=0;
	resetPoints();
	hasControl = true;
	
	wrongBallFlag = true;
	wrongBallFallenFlag = false;
	
	hasEnded = false;
	
	level = _level;
}

//--------------------------------------------------- DESTRUCTORS
Game::~Game() {   }

//--------------------------------------------------- GETTERS & SETTERS
void Game::setSinucaMode(bool _sinuca)
{
	inSinucaMode = _sinuca;
	resetPoints();	
}

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
	if(currentPlayer == 3)	// POG to fix a really strange problem D=
		currentPlayer = 2;
	
	vector<int> updatingValues = level->updateState(currentPlayer);
	
	if( !updatingValues[0] ) 		//if nothing moved
	{
		giveControl();
	}
	
	if( (updatingValues[1] > 0) && wrongBallFlag)	// if player hit the wrong ball
	{
		wrongBallFlag = false;
		//cout << "FallenFlag: " << wrongBallFallenFlag << endl;
		cout << "updatingValues: " << updatingValues[1] << endl;
		if (updatingValues[1] == 2)
			wrongBallFallenFlag = true;
	}
	
	if( (updatingValues[2] > 0) || (updatingValues[3] > 0))
	{
		if( updatingValues[2] > 0 )		// if player's ball were snooked
			points[currentPlayer-1] += updatingValues[2];
		
		if( updatingValues[3] > 0 )		// if enemy's ball were snooked
			points[currentPlayer % NPLAYERS] += updatingValues[3];
			
		updateOsd();
	}
}

void Game::nextPlayer()
{
	bool hasAlreadyDroppedABall = false;
	
	/* Testing if the player hasn't hit any ball
	 * during his turn (if so, the enemy "gains"
	 * a ball, i.e, gains one point.
	 */
	if(wrongBallFlag == true)
	{
		knockTheEnemysBall();
		hasAlreadyDroppedABall = true;
	}
	
	/* Test if the player hit an enemy's ball. If he did,
	 * the enemy "gains" (actually, lose) a ball.
	 */
	if( wrongBallFallenFlag == true )
	{
		knockTheEnemysBall();
		hasAlreadyDroppedABall = true;
	}
	
	/* Test if the current player snooked the main ball.
	 * If he did, his enemy "gains" (or lose, actually),
	 * a ball, i.e, gains one point.
	 */
	if ( level->balls[0].hasFallen == true )
	{
		if ( hasAlreadyDroppedABall == false )
			knockTheEnemysBall();
		level->balls[0].hasFallen = false;
		level->balls[0].setPos(-20, TABLE_PLANE_Y+level->balls[0].getRadius(), 0);
	}
	
	// If the eighty ball has been snooked, the game is over
	if ( level->balls[8].pos[1] <= 1 )
	{
		hasEnded = true;
	}
	
	wrongBallFlag 		= true;
	wrongBallFallenFlag = false;
	currentPlayer++;
	if(currentPlayer > NPLAYERS)
		currentPlayer=1;
	cout << "Vez do jogador " << currentPlayer << endl;
}

void Game::restartGame()
{	
	currentPlayer = 0;
	fps=0;
	resetPoints();
	hasControl = true;
	
	wrongBallFlag = true;
	wrongBallFallenFlag = false;
	
	level->EndTheGame();
}

void Game::knockTheEnemysBall()
{
	if (currentPlayer % NPLAYERS == 0)
	{
		for(int i = 1; i <= 8; i++)
			if (level->balls[i].hasFallen == false)
			{
				level->balls[i].hasFallen = true;
				level->balls[i].moveVector[1] = -1;
				points[(currentPlayer) % NPLAYERS]++;
				break;			// get out of the "for".
			}
	}
	else if ( (currentPlayer % NPLAYERS) == 1 )
	{
		bool found = false;
		for(int i = 9; i < 16; i++)
			if (level->balls[i].hasFallen == false)
			{
				level->balls[i].hasFallen = true;
				level->balls[i].moveVector[1] = -1;
				points[(currentPlayer) % NPLAYERS]++;
				found = true;
				break;
			}
		if (found == false)
		{
			level->balls[8].hasFallen = true;
			level->balls[8].moveVector[1] = -1;
		}
	}
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
	if( hasEnded )
	{
			char restartGame[] = "Thanks for playing SiNoS\n Would you like to play again?\n (Y / N)";
			
			sprintf(osd[0], "%s", restartGame);
	}
	else
	{
		char stripes[] = "Stripes",
			 solids [] = "Solids";
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
			
			sprintf(osd[i+1],"%s Player %i: %i -- %s",osd[i+1],i+1,points[i],
													  i == 0? solids : stripes);
		}
	}
}
