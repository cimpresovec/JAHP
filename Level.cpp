#include <SDL.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "Functions.h"
#include "Globals.h"
#include "Classes.h"
#include <fstream>
#include <fmod.h>

StateLevel::StateLevel()
{
	loadLevel(o_gameObjects, i_levelNumber);

	Gplayer = &o_player;

	///////// LEVEL SET UP
	setUpLevel( o_gameObjects );

	o_player.die(o_gameObjects);

	if ( maxRoomX < 0 ){ maxRoomX = 100;}
	if ( maxRoomY < 0 ) maxRoomY = 100;

	std::ofstream data ("data/levels/score.lvl");

	data.clear();

	for ( int n = 0; n < (signed)o_scores.size(); n++ )
	{
		data << o_scores[n]->i_number << " " << o_scores[n]->b_unlocked << " " << o_scores[n]->i_highScore << " " << o_scores[n]->i_B << " " << o_scores[n]->i_S << " " << o_scores[n]->i_G << "\n";
	}

	b_go = false;
	b_restart = false;


}

StateLevel::~StateLevel()
{
	for ( int n = 0; n < ( signed ) o_gameObjects.size(); n++ )
	{
				GameObject * p;
				p = o_gameObjects[n];
				delete p;
	}

	o_gameObjects.clear();
	o_gameObjects.empty();

}


void StateLevel::handleEvents()
{
	if ( SDL_PollEvent(&event) )
	{
		if ( event.type == SDL_QUIT ) setNextGameState(0);

		if ( event.type == SDL_KEYUP )
		{
            if ( event.key.keysym.sym == SDLK_ESCAPE ) setNextGameState (2);
            if ( event.key.keysym.sym == SDLK_SPACE && ( (o_player.b_die == true) || (o_player.b_win == true )) ) b_go = true;
            if ( event.key.keysym.sym == SDLK_r ) b_restart = true;
		}
            if ( o_player.b_die == false && o_player.b_win == false ) o_player.handleEvents();

        if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_m )
		{
		    if ( b_music == true )
		    {
		        FSOUND_Stream_Stop(song2);

		        b_music = false;
		    }

		    else if ( b_music == false )
		    {
		        FSOUND_Stream_Play( 1, song2 );
                FSOUND_Stream_SetLoopCount(song2,-1);

                b_music = true;

		    }

		}




	}

}


void StateLevel::logic()
{
//////////LOGIC///////////////////////
	if ( o_player.b_die == false && o_player.b_win == false )
	{
		o_player.power();
		o_player.move( o_gameObjects );
	}

	if ( (o_player.b_die == true && b_go == true) || b_restart == true )
		{
			b_go = false;
			b_restart = false;
		o_player.die( o_gameObjects );

		for ( int n = 0; n < ( signed ) o_gameObjects.size(); n++ )
		{
				GameObject * p;
				p = o_gameObjects[n];
				delete p;
		}
		o_gameObjects.clear();
		o_gameObjects.empty();
		loadLevel( o_gameObjects, i_levelNumber );
		setUpLevel(o_gameObjects);

		if ( maxRoomX < 0 ){ maxRoomX = 100;}
		if ( maxRoomY < 0 ) maxRoomY = 100;
	}

	for ( int n = 0; n < (signed)o_gameObjects.size(); n++ )
	{
			o_gameObjects[n]->logic();

			if ( (o_gameObjects[n]->f_timer > 40) && (o_gameObjects[n]->i_type == OBJECT_BREAKABLE) )
			{
				o_gameObjects.erase(o_gameObjects.begin()+n);
				n--;
			}
	}

	for ( int n = 0; n < ( signed ) Gparticles.size(); n++ )
	{
		Gparticles[n]->move();

		if ( Gparticles[n]->timer >= 20 )
		{
			Particle * p;
			p = Gparticles[n];
			delete p;

			Gparticles.erase(Gparticles.begin()+n);
			n--;
		}
	}

	roomX = Gplayer->box.x-400;
	roomY = Gplayer->box.y-300;

	if ( roomX < minRoomX ) roomX = minRoomX;
	if ( roomY < minRoomY ) roomY = minRoomY;

	if ( roomX > maxRoomX ) roomX = maxRoomX;
	if ( roomY > maxRoomY ) roomY = maxRoomY;

	if ( o_player.b_win == true && b_go == true )
	{
		i_levelNumber++;

		if ( i_levelNumber <= (o_scores.size()  ))
		{

				if ( i_levelNumber < 33 )
				{
				if ( o_scores[i_levelNumber-2]->i_highScore > o_player.f_levelTimer ) o_scores[i_levelNumber-2]->i_highScore = (int)o_player.f_levelTimer;
				setNextGameState(3);
				o_scores[i_levelNumber-1]->b_unlocked = true;
				}

				else if ( i_levelNumber >= 33 )
				{
				    if ( o_scores[o_scores.size()-1]->i_highScore > o_player.f_levelTimer ) o_scores[o_scores.size()-1]->i_highScore = (int)o_player.f_levelTimer;
				    setNextGameState(2);
				    i_levelNumber--;
				    std::cout <<"End\n";
				}
		}
		else i_levelNumber--;
	}

}


void StateLevel::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

		glOrtho(0,800,600,0,-1,1);

		glColor4ub(70,90,255,255);

			glBegin(GL_QUADS);
				glVertex2f(0,0);
				glColor4ub(180,90,100,255);glVertex2f(800,0);
				glVertex2f(800,600);
				glColor4ub(70,180,50,255);glVertex2f(0,600);
			glEnd();


	glPopMatrix();

	glPushMatrix();

		glTranslated ( -(float)roomX/400, (float)roomY/300, 0);




		o_player.render();

		for ( int n = 0; n < (signed)o_gameObjects.size(); n++ )
		{

			o_gameObjects[n]->render();

		}

		glPushMatrix();

			glOrtho(0,800,600,0,-1,1);

			glColor4ub(255,255,255,255);

			glPointSize(3);
			glBegin(GL_POINTS);
			for ( int n = 0; n < (signed)Gparticles.size(); n++ )
			{
				glVertex2f(Gparticles[n]->f_x, Gparticles[n]->f_y);
			}

			glEnd();

		glPopMatrix();

	glPopMatrix();

	glPushMatrix();

		glOrtho(0,800,600,0,-1,1);

		font.render((int)o_player.f_levelTimer,10,570);

		glColor4ub(255,255,255,255);

			if ( o_player.b_win == true )
			{
				glEnable(GL_TEXTURE_2D);

				if ( o_player.f_levelTimer <= o_scores[i_levelNumber-1]->i_G )
				{
					glBindTexture(GL_TEXTURE_2D, tex_gold);
				}

				else if ( o_player.f_levelTimer <= o_scores[i_levelNumber-1]->i_S)
				{
					glBindTexture(GL_TEXTURE_2D, tex_silver);
				}

				else if (o_player.f_levelTimer <= o_scores[i_levelNumber-1]->i_B)
				{
					glBindTexture(GL_TEXTURE_2D, tex_bronze);
				}

				else
				{
					glBindTexture(GL_TEXTURE_2D, tex_done);
				}

				glBegin(GL_QUADS);
					glTexCoord2i(0,0); glVertex2i(200,150);
					glTexCoord2i(1,0); glVertex2i(600,150);
					glTexCoord2i(1,1); glVertex2i(600,450);
					glTexCoord2i(0,1); glVertex2i(200,450);
				glEnd();

				font.render(o_scores[i_levelNumber-1]->i_S, 250,370);
				font.render(o_scores[i_levelNumber-1]->i_G, 360,370);
				font.render(o_scores[i_levelNumber-1]->i_B, 460,370);

				glDisable(GL_TEXTURE_2D);
			}



	glPopMatrix();

	SDL_GL_SwapBuffers();

}
