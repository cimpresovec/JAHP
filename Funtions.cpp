#include "Functions.h"
#include "Classes.h"
#include "Globals.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <wincompat.h>
#include <fmod.h>
#include <iostream>


bool init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	SDL_WM_SetCaption( "Platformer", NULL );

	SDL_SetVideoMode( 800,600,32, SDL_OPENGL );

	glShadeModel( GL_SMOOTH );

	glClearColor ( 1,1,1,1 );

	glViewport ( 0,0,800,600);

	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	#if defined(WIN32) || defined(__CYGWIN32__) || defined(__WATCOMC__)
    FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
#elif defined(__linux__)
    FSOUND_SetOutput(FSOUND_OUTPUT_OSS);
#endif


    SDL_Delay(6000);

    FSOUND_SetDriver(0);

	FSOUND_Init(44100, 32, 0);


	return true;
}

GLuint loadTexture( const std::string &fileName )
{
	SDL_Surface *Image = IMG_Load(fileName.c_str());

    if(!Image)
        return 0;

	SDL_DisplayFormatAlpha(Image);

    unsigned Object(0);

    glGenTextures(1, &Object);

    glBindTexture(GL_TEXTURE_2D, Object);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image->w, Image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	Image->pixels);

    SDL_FreeSurface(Image);

    return Object;

}

bool checkCollisionQuads( Box A, Box B )
{
	if ( A.x + A.w <= B.x ) return false;
	if ( A.y + A.h <= B.y ) return false;
	if ( A.x >= B.x + B.w ) return false;
	if ( A.y >= B.y + B.h ) return false;

	return true;
}
void setUpLevel( std::vector<GameObject*> &objects )
{

	for ( int n = 0; n < (signed) objects.size(); n++ )
	{
		for ( int g = 0; g < (signed) objects.size(); g++ )
		{
			if ( (objects[g]->i_type != OBJECT_PLAYER_BLOCK) && (objects[g]->i_type != OBJECT_FORCE_JUMP) && (objects[g]->i_type != OBJECT_EXIT) )
			{
			if ( ((objects[g]->getBox().x+objects[g]->getBox().w) == objects[n]->getBox().x ) && ( objects[g]->getBox().y == objects[n]->getBox().y) )
			{
				objects[n]->b_line[3] = false;
				objects[n]->Spiked_b_spikes[3] = false;
			}

			if ( (objects[g]->getBox().x == (objects[n]->getBox().x+objects[n]->getBox().w) ) && ( objects[g]->getBox().y == objects[n]->getBox().y) )
			{
				objects[n]->b_line[1] = false;
				objects[n]->Spiked_b_spikes[1] = false;
			}

			if ( ((objects[g]->getBox().y + objects[g]->getBox().h) == objects[n]->getBox().y ) && (objects[g]->getBox().x == objects[n]->getBox().x ) )
			{
				objects[n]->b_line[0] = false;
				objects[n]->Spiked_b_spikes[0] = false;
			}

			if ( (objects[g]->getBox().y == (objects[n]->getBox().y + objects[n]->getBox().h) ) && (objects[g]->getBox().x == objects[n]->getBox().x ) )
			{
				objects[n]->b_line[2] = false;
				objects[n]->Spiked_b_spikes[2] = false;
			}
			}

		}
	}

	//ERASE CONTROLS

	for ( int n = 0; n < (signed) objects.size(); n++ )
	{
		if ( objects[n]->i_type == OBJECT_CONTROL )
		{
			objects.erase(objects.begin()+n);
			n--;
		}
	}


}
void saveLevel( std::vector<GameObject*> &objects, int levelNumber )
{
	std::stringstream name;
	name << "data/levels/" << levelNumber << ".lvl";
	std::ofstream file (name.str().c_str());

	float maxX,maxY, minX, minY;
	maxX = 0;
	maxY = 0;
	minX = 1000000;
	minY = 1000000;
	for ( int n = 0; n < (signed) objects.size(); n++ )
	{
		if ( objects[n]->getBox().x > maxX ) maxX = objects[n]->getBox().x+25;
		if ( objects[n]->getBox().y > maxY ) maxY = objects[n]->getBox().y+25;

		if ( objects[n]->getBox().x < minX ) minX = objects[n]->getBox().x;
		if ( objects[n]->getBox().y < minY ) minY = objects[n]->getBox().y;

	}

	file << maxX << " " << maxY << "\n";
	file << minX << " " << minY << "\n";

	for ( int n = 0; n < ( signed ) objects.size(); n++ )
	{
		switch ( objects[n]->i_type )
		{
		case OBJECT_BLOCK:
			{
				file << "1 " << objects[n]->getBox().x << " " << objects[n]->getBox().y << "\n";
				break;
			}
		case OBJECT_SPIKED:
			{
				file << "2 " << objects[n]->getBox().x << " " << objects[n]->getBox().y << "\n";
				break;
			}
		case OBJECT_CONTROL:
			{
				file << "3 " << objects[n]->getBox().x << " " << objects[n]->getBox().y << "\n";
				break;
			}
		case OBJECT_BREAKABLE:
			{
				file << "4 " << objects[n]->getBox().x << " " << objects[n]->getBox().y << "\n";
				break;
			}
		case OBJECT_TIMED:
			{
				file << "5 " << objects[n]->getBox().x << " " << objects[n]->getBox().y << " " <<objects[n]->Timed_f_time << "\n";
				break;
			}
		case OBJECT_PLAYER_BLOCK:
			{
				file << "6 " << objects[n]->getBox().x << " " << objects[n]->getBox().y << "\n";
				break;
			}
		case OBJECT_FORCE_JUMP:
			{
				file << "7 " << objects[n]->getBox().x << " " << objects[n]->getBox().y << "\n";
				break;
			}
		case OBJECT_EXIT:
			{
				file << "8 " << objects[n]->getBox().x << " " << objects[n]->getBox().y << "\n";
				break;
			}

		}
	}

	file.close();
}

void loadLevel( std::vector<GameObject*> &object, int levelNumber )
{
	std::stringstream name;
	name << "data/levels/" << levelNumber << ".lvl";
	std::ifstream file (name.str().c_str());

	file >> maxRoomX;
	maxRoomX = ( maxRoomX - 800) + 25;
	file >> maxRoomY;
	maxRoomY = ( maxRoomY - 600 ) + 25;
	file >> minRoomX;

	file >> minRoomY;


	while ( file.eof() == false )
	{
		int type, x, y;
		file >> type;
		file >> x;
		file >> y;

		switch ( type )
		{
		case 1:	object.push_back( new Block((float)x,(float)y)); break;
		case 2: object.push_back( new SpikedBlock((float)x,(float)y)); break;
		case 3: object.push_back( new ControlBlock((float)x,(float)y)); break;
		case 4: object.push_back( new BreakableBlock((float)x,(float)y)); break;
		case 5:
			{
				float time;
				file >> time;
				object.push_back ( new TimedBlock((float)x,(float)y,time)); break;
			}
		case 6: object.push_back( new PlayerBlock((float)x,(float)y)); break;
		case 7: object.push_back( new ForceJumpBlock((float)x,(float)y)); break;
		case 8: object.push_back( new ExitBlock((float)x,(float)y)); break;
		}
	}

	object.pop_back();


	file.close();

}


float calculateAngle( Box A, Box B )
{
	float Ax,Ay,Bx,By;

	Ax = A.x + A.w/2;
	Ay = A.y + A.h/2;
	Bx = B.x + B.w/2;
	By = B.y + B.h/2;

	double sideA, sideB, angle;

	sideA = Bx - Ax; sideB = By - Ay;

	if ( sideA < 0 ) { sideA = -sideA; } if ( sideB < 0 ) { sideB = -sideB; }

	angle = atan2(sideB,sideA) *180/3.141592654  ;

	if ( By > Ay && Bx > Ax ) { angle += 90; }
	else if ( By > Ay && Bx < Ax ) { angle += 180; }
	else if ( By < Ay && Bx < Ax ) { angle += 270; }
	else if ( Bx == Ax && By < Ay ) { angle = 0; }
	else if ( Bx == Ax && By > Ay ) { angle = 180; }
	else if ( By == Ay && Bx > Ax ) { angle = 90; }
	else if ( By == Ay && Bx < Ax ) { angle = 270; }

	float G;

	if ( angle < 90 && angle > 0 )
	{
		angle = 90 - angle;
	}

	if ( angle > 180 && angle < 270 )
	{
		angle = 270 - angle + 180;
	}

	G = (float)angle;

	return (float)G;
}

float calculateSpeed( float angle, float xSpeed, int mode )
{
	float A;
	if ( angle >= 90 )
	{
		angle -= 90;
	}
	else
	{
		angle = (360 + ( angle - 90 ) );
	}
	A = angle;

	if ( mode == 0 )
	{
		return (float)(sin((A * (3.14159265/180))) * xSpeed);
	}

	else
	{
		return (float)(cos((A * (3.14159265/180))) * xSpeed);
	}
}

void setNextGameState( int nextState )
{
	switch ( nextState )
	{
	case 1: nextGameState = STATE_INTRO; break;
	case 2: nextGameState = STATE_MENU; break;
	case 3: nextGameState = STATE_LEVEL; break;
	case 4: nextGameState = STATE_EDITOR; break;
	case 5: nextGameState = STATE_SELECTION; break;
	case 8: nextGameState = STATE_INFO; break;
	case 0: nextGameState = STATE_EXIT; break;
	}
}

void changeGameState()
{
	if ( nextGameState != STATE_NULL )
	{
		if ( nextGameState != STATE_EXIT )
		{
			delete gameState;

			switch ( nextGameState )
			{
			case STATE_LEVEL: gameState = new StateLevel(); break;
			case STATE_EDITOR: gameState = new StateEditor(); break;
			case STATE_MENU: gameState = new StateMenu(); break;
			case STATE_SELECTION: gameState = new StateSelection(); break;
			case STATE_INFO: gameState = new Info(); break;
			default: break;
			}

			currentGameState = nextGameState;
			nextGameState = STATE_NULL;

		}
	}
}

float getFPS()
{
    return ( ( ( SDL_GetTicks() - FPSA ) / 1000.f ) );
}
