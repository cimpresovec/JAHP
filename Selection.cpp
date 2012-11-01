#include <SDL.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "Functions.h"
#include "Globals.h"
#include "Classes.h"
#include <fmod.h>
#include <fstream>
#include <sstream>



StateSelection::StateSelection()
{
	i_numberOfLevels = 0;


	for ( int n = 1; 1; n++ )
	{
		std::stringstream name;
		name << "data/levels/" << n << ".lvl";
		std::ifstream file (name.str().c_str());

		if ( file.is_open() == true )
		{
			i_numberOfLevels++;
		}

		else break;

	}

	if ( (signed)o_scores.size() > i_numberOfLevels ) o_scores.pop_back();

	std::cout << o_scores.size() << std::endl;



}



void StateSelection::handleEvents()
{
	if ( SDL_PollEvent(&event) )
	{
		if ( event.type == SDL_QUIT ) setNextGameState(0);
		if ( event.key.keysym.sym == SDLK_ESCAPE ) setNextGameState (2);


		if ( event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT )
		{
			int x,y;
			SDL_GetMouseState(&x,&y);

			Box mouse;
			mouse.x = (float)x;
			mouse.y = (float)y;
			mouse.w = 1;
			mouse.h = 1;

			Box col;
			col.w = 25;
			col.h = 25;

			for ( int n = 0, x = 50, y = 50; n < i_numberOfLevels ; n++, x+= 50 )
			{
				if ( x >= 750 ) { x = 50; y+=50; }
				col.x = (float)x;
				col.y = (float)y;

				if ( checkCollisionQuads(mouse, col) == true )
				{
					if ( o_scores[n]->b_unlocked == true )
					{
						i_levelNumber = n+1;
						setNextGameState(3);
						break;
					}
				}

			}

		}
	}


}



void StateSelection::logic()
{


}


void StateSelection::render()
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

		glLineWidth(5);
		glColor4ub(0,00,0,255);

		for ( int n = 0, x = 50, y = 50; n <i_numberOfLevels ; n++, x+= 50 )
			{
				if ( x >= 750 ) { x = 50; y+=50; }

				if ( n < ( (signed)o_scores.size() - 1 ) )
				{
					if ( o_scores[n+1]->b_unlocked == true )
					{
						glBegin(GL_LINES);
							glVertex2i(x+12,y+12);
							if ( x == 700 ) glVertex2i(x+12,y+12);
							else glVertex2i(x+62,y+12);
						glEnd();
					}
				}

			}


		glBegin(GL_QUADS);
			for ( int n = 0, x = 50, y = 50; n < i_numberOfLevels; n++, x+= 50 )
			{
				if ( o_scores[n]->i_highScore <= o_scores[n]->i_G )
				{
					glColor4ub(201,137,16,255);
				}

				else if (o_scores[n]->i_highScore <= o_scores[n]->i_S)
				{
					glColor4ub(168,168,168,255);
				}

				else if (o_scores[n]->i_highScore <= o_scores[n]->i_B)
				{
					glColor4ub(150,90,56,255);
				}

				else if (o_scores[n]->i_highScore > 0 )
				{
					glColor4ub(255,255,255,255);
				}

				if ( o_scores[n]->b_unlocked == false ) glColor4ub(10,10,10,255);

				if ( x >= 750 ) { x = 50; y+=50; }

				glVertex2i(x,y);
				glVertex2i(x+25,y);
				glVertex2i(x+25,y+25);
				glVertex2i(x,y+25);

			}
		glEnd();

		glLineWidth(5);
		glColor4ub(0,00,0,255);


			for ( int n = 0, x = 50, y = 50; n <i_numberOfLevels; n++, x+= 50 )
			{
				if ( x >= 750 ) { x = 50; y+=50; }
				glBegin(GL_LINE_LOOP);
					glVertex2i(x,y);
					glVertex2i(x+25,y);
					glVertex2i(x+25,y+25);
					glVertex2i(x,y+25);
				glEnd();

			}






	glPopMatrix();

	SDL_GL_SwapBuffers();

}

void Info::handleEvents()
{
    if ( SDL_PollEvent(&event) )
	{
		if ( event.type == SDL_QUIT ) setNextGameState(0);
		if ( event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYUP ) setNextGameState (2);

		if ( event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYUP ) setNextGameState(3);
	}

}

void Info::logic()
{
}

void Info::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

		glOrtho(0,800,600,0,-1,1);

		glColor4ub(255,255,255,255);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_info);

		glBegin ( GL_QUADS );
			glTexCoord2i(0,0); glVertex2f(0,0);
			glTexCoord2i(1,0); glVertex2f(800,0);
			glTexCoord2i(1,1); glVertex2f(800,600);
			glTexCoord2i(0,1); glVertex2f(0,600);
		glEnd();



		glDisable(GL_TEXTURE_2D);


	glPopMatrix();

	SDL_GL_SwapBuffers();

}
