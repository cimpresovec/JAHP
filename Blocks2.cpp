#include "Classes.h"
#include "Globals.h"
#include "Functions.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <string>
#include <sstream>

TimedBlock::TimedBlock(float x, float y, float time)
{
	box.x = x;
	box.y = y;
	box.w = 25; box.h = 25;

	b_collidable = true;

	Timed_f_time = time;
	Timed_b_state = 0;

	Spiked_b = true;
	Spiked_b_spikes[0] = true;
	Spiked_b_spikes[1] = true;
	Spiked_b_spikes[2] = true;
	Spiked_b_spikes[3] = true;

	i_type = OBJECT_TIMED;
}

void TimedBlock::logic()
{
    float k = getFPS();
	if ( PLAYER_SLOWMOTION ) f_timer+=(float)(60/3.00)*k;
	else f_timer+= 60 *k;

	if ( f_timer >= Timed_f_time )
	{
		if ( Timed_b_state == 0 )
		{
			Timed_b_state = 1;
		}
		else Timed_b_state = 0;
		f_timer = 0;
	}
}

void TimedBlock::render()
{
	glPushMatrix();

			glOrtho(0,800,600,0,-1,1);

			glColor4ub(255,0,0,255);

			glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_block1);

	glBegin(GL_QUADS);
		glTexCoord2i(0,0); glVertex2f(box.x,box.y);
		glTexCoord2i(1,0);glVertex2f(box.x+box.w,box.y);
		glTexCoord2i(1,1);glVertex2f(box.x+box.w,box.y+box.h);
		glTexCoord2i(0,1);glVertex2f(box.x,box.y+box.h);
	glEnd();

		glDisable(GL_TEXTURE_2D);

			glColor4ub(0,0,0,130);
			glLineWidth(5);

			glBegin(GL_LINES);
				if ( b_line[0] == true )
				{	glVertex2f(box.x,box.y); glVertex2f(box.x+box.w,box.y); }
				if ( b_line[1] == true )
				{glVertex2f(box.x+box.w,box.y);glVertex2f(box.x+box.w,box.y+box.h);}
				if ( b_line[2] == true )
				{glVertex2f(box.x+box.w,box.y+box.h);glVertex2f(box.x,box.y+box.h);}
				if ( b_line[3]== true )
				{glVertex2f(box.x,box.y+box.h);glVertex2f(box.x,box.y);}
			glEnd();

			if ( Timed_b_state == 1 )
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, tex_spikes);

				glColor4ub(255,0,0,255);

				glBegin(GL_QUADS);

				if ( Spiked_b_spikes[0] == true )
				{

					glTexCoord2i(0,0); glVertex2f(box.x,box.y-8);
					glTexCoord2i(1,0); glVertex2f(box.x+box.w,box.y-8);
					glTexCoord2i(1,1); glVertex2f(box.x+box.w,box.y);
					glTexCoord2i(0,1); glVertex2f(box.x,box.y);

				}

				if ( Spiked_b_spikes[1] == true )
				{

					glTexCoord2i(0,1); glVertex2f(box.x+box.w,box.y);
					glTexCoord2i(0,0); glVertex2f(box.x+box.w+8,box.y);
					glTexCoord2i(1,0); glVertex2f(box.x+box.w+8,box.y+box.h);
					glTexCoord2i(1,1); glVertex2f(box.x+box.w,box.y+box.h);

				}

				if ( Spiked_b_spikes[2] == true )
				{

					glTexCoord2i(1,1); glVertex2f(box.x,box.y+box.h);
					glTexCoord2i(0,1); glVertex2f(box.x+box.w,box.y+box.h);
					glTexCoord2i(0,0); glVertex2f(box.x+box.w,box.y+box.h+8);
					glTexCoord2i(1,0); glVertex2f(box.x,box.y+box.h+8);

				}

				if ( Spiked_b_spikes[3] == true )
				{

					glTexCoord2i(0,0); glVertex2f(box.x-8,box.y);
					glTexCoord2i(0,1); glVertex2f(box.x,box.y);
					glTexCoord2i(1,1); glVertex2f(box.x,box.y+box.h);
					glTexCoord2i(1,0); glVertex2f(box.x-8,box.y+box.h);

				}

				glEnd();

				glDisable(GL_TEXTURE_2D);
			}

	glPopMatrix();

}

PlayerBlock::PlayerBlock(float x, float y)
{
	box.x = x;
	box.y = y;
	box.w = 25;
	box.h = 25;

	i_type = OBJECT_PLAYER_BLOCK;

	b_collidable = false;

}

void PlayerBlock::render()
{
	if ( currentGameState == STATE_EDITOR )
	{
		glPushMatrix();

			glOrtho(0,800,600,0,-1,1);

			glColor4ub(255,255,0,255);

		glBegin(GL_QUADS);
			glVertex2f(box.x,box.y);
			glVertex2f(box.x+box.w,box.y);
			glVertex2f(box.x+box.w,box.y+box.h);
			glVertex2f(box.x,box.y+box.h);
		glEnd();

		glPopMatrix();
	}

}

ForceJumpBlock::ForceJumpBlock ( float x, float y )
{
	box.x = x;
	box.y = y;
	box.w = 25;
	box.h = 25;

	i_type = OBJECT_FORCE_JUMP;

	b_collidable = false;
}

void ForceJumpBlock::render()
{
	glPushMatrix();

			glOrtho(0,800,600,0,-1,1);

			glColor4ub(0,255,255,255);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex_arrow);

		glBegin(GL_QUADS);
			glTexCoord2i(0,0); glVertex2f(box.x,box.y);
			glTexCoord2i(1,0);glVertex2f(box.x+box.w,box.y);
			glTexCoord2i(1,1);glVertex2f(box.x+box.w,box.y+box.h);
			glTexCoord2i(0,1);glVertex2f(box.x,box.y+box.h);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
}

ExitBlock::ExitBlock(float x, float y)
{
	box.x = x;
	box.y = y;
	box.w = 25;
	box.h = 25;

	i_type = OBJECT_EXIT;

	b_collidable = false;
}

void ExitBlock::render()
{

		glPushMatrix();

			glOrtho(0,800,600,0,-1,1);

			glColor4ub(255,255,0,255);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex_door);

		glBegin(GL_QUADS);
			glTexCoord2i(0,0); glVertex2f(box.x,box.y);
			glTexCoord2i(1,0);glVertex2f(box.x+box.w,box.y);
			glTexCoord2i(1,1);glVertex2f(box.x+box.w,box.y+box.h);
			glTexCoord2i(0,1);glVertex2f(box.x,box.y+box.h);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glPopMatrix();


}

Font::Font()
{
	for ( int n = 0, x = 0; n < 10; n++, x+= 26 )
	{
		num[n].x = (float)x;
		num[n].y = 0;
		num[n].w = 26;
		num[n].h = 25;

	}

	let[0] = '0'; let[1] = '1'; let[2] = '2'; let[3] = '3'; let[4] = '4';
	let[5] = '5'; let[6] = '6'; let[7] = '7'; let[8] = '8'; let[9] = '9';
}

void Font::render(int number, int X, int Y)
{
	std::stringstream a;
	a << number;

	char b[10];

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_font);

	glColor4ub(255,255,255,255);

	for ( int n = 0, x = X, y = Y; n < (signed) a.str().size(); n++, x+=24 )
	{
		b[n] = a.str().at(n);

		for ( int g = 0; g < 10; g++ )
		{
			if ( b[n] == let[g]  )
			{
				glBegin(GL_QUADS);
					glColor4ub(50,50,50,255);glTexCoord2d((num[g].x/260.0),0);glVertex2i(x,y);
					glTexCoord2d((num[g].x+num[g].w)/260.0,0);glVertex2i(x+26,y);
					glColor4ub(255,0,0,255);glTexCoord2d((num[g].x+num[g].w)/260.0,1);glVertex2i(x+26, y+25);
					glTexCoord2d((num[g].x/260.0),1);glVertex2i(x,y+25);
				glEnd();

			}
		}

	}

	glDisable(GL_TEXTURE_2D);

}
