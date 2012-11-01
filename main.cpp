/////////////////////////////////////////////////
/*
Ime igre:

Zaèetek: 3. 1. 2010, 14.27

Avtor: Luka Horvat

*/
///////////////////////////////////////////////
#include <SDL.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "Functions.h"
#include "Globals.h"
#include "Classes.h"
#include <fmod.h>
#include <fmod_errors.h>
#include <ctime>
#include <cstdlib>
#include <fstream>


int main ( int argc, char* args[] )
{
	init();

	srand(time(NULL));

	int FPS = 0;

	///////GLOBAL LOADING
	tex_spikes = loadTexture("data/gfx/Spikes.png");
	tex_block1 = loadTexture("data/gfx/block1.png");
	tex_menu = loadTexture("data/gfx/menu.png");
	tex_playgame = loadTexture("data/gfx/playgame.png");
	tex_selectlevel = loadTexture("data/gfx/selectlevel.png");
	tex_editor = loadTexture("data/gfx/editor.png");
	tex_exit = loadTexture("data/gfx/exitmenu.png");
	tex_player1 = loadTexture("data/gfx/player.png");
	tex_door = loadTexture("data/gfx/door.png");
	tex_font = loadTexture("data/gfx/font.png");
	tex_done = loadTexture("data/gfx/ok.png");
	tex_gold = loadTexture("data/gfx/gold.png");
	tex_silver = loadTexture("data/gfx/silver.png");
	tex_bronze = loadTexture("data/gfx/bronze.png");
	tex_eyes = loadTexture("data/gfx/playgame.png");
	tex_arrow = loadTexture("data/gfx/arrow.png");
	tex_info = loadTexture("data/gfx/info.png");


	///////soundsss
	sample_playerJump = FSOUND_Sample_Load(FSOUND_UNMANAGED,"data/sfx/jump.wav",0,0,0);
	FSOUND_Sample_SetDefaults(sample_playerJump, -1,100,-1,-1);
	sample_playerHit = FSOUND_Sample_Load(FSOUND_UNMANAGED,"data/sfx/hit.wav",0,0,0);
	FSOUND_Sample_SetDefaults(sample_playerHit, -1,100,-1,-1);
	sample_playerForce = FSOUND_Sample_Load(FSOUND_UNMANAGED,"data/sfx/force.wav",0,0,0);
	FSOUND_Sample_SetDefaults(sample_playerForce, -1,100,-1,-1);

	song2 = FSOUND_Stream_Open( "data/music/Random-SitgesSavepoint.mp3",FSOUND_LOOP_NORMAL,0,0);

	FSOUND_Stream_Play( 1, song2 );
	FSOUND_Stream_SetLoopCount(song2,-1);

	//FSOUND_SetVolume(1,80);

	std::ifstream file ("data/levels/score.lvl");

	while ( !file.eof() )
	{
		o_scores.push_back(new Score());

		file >> o_scores.back()->i_number;
		file >> o_scores.back()->b_unlocked;
		file >> o_scores.back()->i_highScore;
		file >> o_scores.back()->i_B;
		file >> o_scores.back()->i_S;
		file >> o_scores.back()->i_G;

	}

	file.close();

	//o_scores.pop_back();


	gameState = new StateMenu();
	currentGameState = STATE_MENU;



/*
	for ( int n = 0; n < 100; n++ )
	{
		o_scores.push_back( new Score () );
		o_scores.back()->b_unlocked = true;
		o_scores.back()->i_highScore = 0;
		o_scores.back()->i_B = 0;
		o_scores.back()->i_G = 0;
		o_scores.back()->i_S = 0;
	}*/

	//saving

	FPSA = SDL_GetTicks();

	while ( nextGameState != STATE_EXIT )
	{

		FPS = SDL_GetTicks();

		gameState->handleEvents();

		gameState->logic();

		FPSA = SDL_GetTicks();

		gameState->render();

		changeGameState();



		////////////////////////////
		if ( (SDL_GetTicks() - FPS) < 1000/60 )
		{
			//SDL_Delay( ( 1000/60) - (SDL_GetTicks() - FPS) );
		}
		SDL_Delay(2);

	}

    FSOUND_Stream_Close(song2);
	FSOUND_Close();

	return 0;
}

