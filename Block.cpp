#include "Classes.h"
#include "Globals.h"
#include "Functions.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>

GameObject::GameObject()
{
	box.x = 0; box.h = 0;box.w = 0;box.h = 0;box.a = 0;

	Block_b_light[0] = false;
	Block_b_light[1] = false;
	Block_b_light[2] = false;
	Block_b_light[3] = false;

	b_line[0] = true;
	b_line[1] = true;
	b_line[2] = true;
	b_line[3] = true;

	f_timer = 0;

	Timed_f_time = 0;
	Timed_b_state = 0;

}

GameObject::~GameObject()
{

}

BreakableBlock::BreakableBlock(float x, float y)
{
	box.x = x;
	box.y = y;
	box.w = 25;
	box.h = 25;

	b_collidable = true;

	Spiked_b = false;

	i_type = OBJECT_BREAKABLE;
}

void BreakableBlock::logic()
{
    float k = getFPS();
	for ( int n = 0; n < 4; n++ )
	{
		if ( Block_b_light[n] == true )
		{
			if ( PLAYER_SLOWMOTION == true ) f_timer+=(float)(60/3.00)*k;
			else f_timer+=60*k;
		}
	}
}

void BreakableBlock::render()
{
	glPushMatrix();

			glOrtho(0,800,600,0,-1,1);

			glColor4ub(127,31,50,255);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex_block1);

			glBegin(GL_QUADS);
				glTexCoord2i(0,0); glVertex2f(box.x,box.y);
				glTexCoord2i(1,0);glVertex2f(box.x+box.w,box.y);
				glTexCoord2i(1,1);glVertex2f(box.x+box.w,box.y+box.h);
				glTexCoord2i(0,1);glVertex2f(box.x,box.y+box.h);
			glEnd();

		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);

		if ( Block_b_light[0] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x,box.y);
			glVertex2f(box.x+box.w,box.y);
			glColor4f(255,255,255,0);
			glVertex2f(box.x+box.w,box.y-10);
			glVertex2f(box.x,box.y-10);
		}

		if ( Block_b_light[1] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x+box.w,box.y);
			glVertex2f(box.x+box.w,box.y+box.h);
			glColor4f(255,255,255,0);
			glVertex2f(box.x+box.w+10,box.y+box.h);
			glVertex2f(box.x+box.w+10,box.y);
		}

		if ( Block_b_light[3] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x,box.y);
			glVertex2f(box.x,box.y+box.h);
			glColor4f(255,255,255,0);
			glVertex2f(box.x-10,box.y+box.h);
			glVertex2f(box.x-10,box.y);
		}
	glEnd();

			glColor4ub(0,0,0,255);
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
	glPopMatrix();


}


Block::Block( float x, float y )
{
	box.x = x; box.y = y;
	box.w = 25; box.h = 25;

	b_collidable = true;
	Spiked_b = false;

	i_type = OBJECT_BLOCK;
}

void Block::render()
{
	glPushMatrix();

	glOrtho(0,800,600,0,-1,1);

	glColor4ub(0,40,255,255);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_block1);

	glBegin(GL_QUADS);
		glTexCoord2i(0,0); glVertex2f(box.x,box.y);
		glTexCoord2i(1,0);glVertex2f(box.x+box.w,box.y);
		glTexCoord2i(1,1);glVertex2f(box.x+box.w,box.y+box.h);
		glTexCoord2i(0,1);glVertex2f(box.x,box.y+box.h);
	glEnd();

		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		if ( Block_b_light[0] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x,box.y);
			glVertex2f(box.x+box.w,box.y);
			glColor4f(255,255,255,0);
			glVertex2f(box.x+box.w,box.y-10);
			glVertex2f(box.x,box.y-10);
		}

		if ( Block_b_light[1] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x+box.w,box.y);
			glVertex2f(box.x+box.w,box.y+box.h);
			glColor4f(255,255,255,0);
			glVertex2f(box.x+box.w+10,box.y+box.h);
			glVertex2f(box.x+box.w+10,box.y);
		}

		if ( Block_b_light[2] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x, box.y+box.h);
			glVertex2f(box.x+box.w,box.y+box.h);
			glColor4f(255,255,255,0);
			glVertex2f(box.x+box.w,box.y+box.h+10);
			glVertex2f(box.x,box.y+box.h+10);

		}

		if ( Block_b_light[3] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x,box.y);
			glVertex2f(box.x,box.y+box.h);
			glColor4f(255,255,255,0);
			glVertex2f(box.x-10,box.y+box.h);
			glVertex2f(box.x-10,box.y);
		}
	glEnd();

	glColor4ub(0,0,0,255);
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


	glPopMatrix();

}





/////////CONTROL BLOCK

ControlBlock::ControlBlock( float x, float y )
{
	box.x = x; box.y = y;
	box.w = 25; box.h = 25;

	b_collidable = false;

	i_type = OBJECT_CONTROL;
}

void ControlBlock::render()
{
	glPushMatrix();

	glOrtho(0,800,600,0,-1,1);

	glColor4ub(255,255,255,255);

	glBegin(GL_QUADS);
		glVertex2f(box.x,box.y);
		glVertex2f(box.x+box.w,box.y);
		glVertex2f(box.x+box.w,box.y+box.h);
		glVertex2f(box.x,box.y+box.h);
	glEnd();

	glPopMatrix();

}

/////////////////// SPIKED BLOCK

SpikedBlock::SpikedBlock( float x, float y )
{
	box.x = x; box.y = y;
	box.w = 25; box.h = 25;

	b_collidable = true;

	i_type = OBJECT_SPIKED;

	Spiked_b = true;
	Spiked_b_spikes[0] = true;
	Spiked_b_spikes[1] = true;
	Spiked_b_spikes[2] = true;
	Spiked_b_spikes[3] = true;

}

void SpikedBlock::render()
{
	glPushMatrix();

	glOrtho(0,800,600,0,-1,1);

	glColor4ub(130,0,0,255);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_block1);

	glBegin(GL_QUADS);
		glTexCoord2i(0,0); glVertex2f(box.x,box.y);
		glTexCoord2i(1,0);glVertex2f(box.x+box.w,box.y);
		glTexCoord2i(1,1);glVertex2f(box.x+box.w,box.y+box.h);
		glTexCoord2i(0,1);glVertex2f(box.x,box.y+box.h);
	glEnd();

		glDisable(GL_TEXTURE_2D);

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

	glBegin(GL_QUADS);
	if ( Block_b_light[0] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x,box.y);
			glVertex2f(box.x+box.w,box.y);
			glColor4f(255,255,255,0);
			glVertex2f(box.x+box.w,box.y-10);
			glVertex2f(box.x,box.y-10);
		}

		if ( Block_b_light[1] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x+box.w,box.y);
			glVertex2f(box.x+box.w,box.y+box.h);
			glColor4f(255,255,255,0);
			glVertex2f(box.x+box.w+10,box.y+box.h);
			glVertex2f(box.x+box.w+10,box.y);
		}

		if ( Block_b_light[2] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x, box.y+box.h);
			glVertex2f(box.x+box.w,box.y+box.h);
			glColor4f(255,255,255,0);
			glVertex2f(box.x+box.w,box.y+box.h+10);
			glVertex2f(box.x,box.y+box.h+10);

		}

		if ( Block_b_light[3] == true )
		{
			glColor4f(255,255,255,0.5);
			glVertex2f(box.x,box.y);
			glVertex2f(box.x,box.y+box.h);
			glColor4f(255,255,255,0);
			glVertex2f(box.x-10,box.y+box.h);
			glVertex2f(box.x-10,box.y);
		}
	glEnd();

	glColor4ub(0,0,0,255);
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

	glPopMatrix();

}
