#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "Functions.h"
#include "Globals.h"
#include "Classes.h"
#include <fstream>
#include <fmod.h>

StateMenu::StateMenu()
{
	play.x = 200; play.y = 330;
	select.x = 200; select.y = 380;
	editor.x = 200; editor.y = 430;
	exit.x = 200; exit.y = 480;

	play.w = 400; select.w = 400; editor.w = 400; exit.w = 400;
	play.h = 50; select.h = 50; editor.h = 50; exit.h = 50;


}

void StateMenu::handleEvents()
{
	if ( SDL_PollEvent(&event) )
	{

		if ( event.type == SDL_QUIT )
		{
			setNextGameState(0);
		}

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

		if ( event.type == SDL_MOUSEBUTTONUP )
		{
			if ( event.button.button == SDL_BUTTON_LEFT )
			{
				int x,y;
				SDL_GetMouseState(&x,&y);

				Box mouse;
				mouse.x = (float)x;
				mouse.y = (float)y;
				mouse.w = 1;
				mouse.h = 1;

				if ( checkCollisionQuads( mouse, play ) )
				{
					setNextGameState(8);
				}

				else if ( checkCollisionQuads( mouse, select ) )
				{
					setNextGameState(5);
				}

				else if ( checkCollisionQuads( mouse, editor ) )
				{
					setNextGameState(4);
				}

				else if ( checkCollisionQuads( mouse, exit ) )
				{
					setNextGameState(0);
				}
			}
		}

	}

}


void StateMenu::logic()
{


}


void StateMenu::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

		glOrtho(0,800,600,0,-1,1);

		glColor4ub(255,255,255,255);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_menu);

		glBegin ( GL_QUADS );
			glTexCoord2i(0,0); glVertex2f(0,0);
			glTexCoord2i(1,0); glVertex2f(800,0);
			glTexCoord2i(1,1); glVertex2f(800,600);
			glTexCoord2i(0,1); glVertex2f(0,600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, tex_playgame);
		glBegin ( GL_QUADS );
			glTexCoord2i(0,0); glVertex2f(play.x,play.y);
			glTexCoord2i(1,0); glVertex2f(play.x+play.w,play.y);
			glTexCoord2i(1,1); glVertex2f(play.x+play.w,play.y+play.h);
			glTexCoord2i(0,1); glVertex2f(play.x,play.y+play.h);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, tex_selectlevel);
		glBegin ( GL_QUADS );
			glTexCoord2i(0,0); glVertex2f(select.x,select.y);
			glTexCoord2i(1,0); glVertex2f(select.x+select.w,select.y);
			glTexCoord2i(1,1); glVertex2f(select.x+select.w,select.y+select.h);
			glTexCoord2i(0,1); glVertex2f(select.x,select.y+select.h);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, tex_editor);
		glBegin ( GL_QUADS );
			glTexCoord2i(0,0); glVertex2f(editor.x,editor.y);
			glTexCoord2i(1,0); glVertex2f(editor.x+editor.w,editor.y);
			glTexCoord2i(1,1); glVertex2f(editor.x+editor.w,editor.y+editor.h);
			glTexCoord2i(0,1); glVertex2f(editor.x,editor.y+editor.h);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, tex_exit);
		glBegin ( GL_QUADS );
			glTexCoord2i(0,0); glVertex2f(exit.x,exit.y);
			glTexCoord2i(1,0); glVertex2f(exit.x+exit.w,exit.y);
			glTexCoord2i(1,1); glVertex2f(exit.x+exit.w,exit.y+exit.h);
			glTexCoord2i(0,1); glVertex2f(exit.x,exit.y+exit.h);
		glEnd();


		glDisable(GL_TEXTURE_2D);


	glPopMatrix();

	SDL_GL_SwapBuffers();
}
