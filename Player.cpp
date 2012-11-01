#include "Classes.h"
#include "Globals.h"
#include "Functions.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <vector>
#include <fmod.h>

Player::Player()
{
	box.x = 0;
	box.y = 0;
	box.w = 25;
	box.h = 25;
	box.a = 0;

	f_xVel = 0;
	f_yVel = 0;

	f_speedFactor = 1.00;

	b_left = false;
	b_right = false;
	b_up = false;
	b_inAir = true;
	b_jump = false;
	b_wallLeft = false;
	b_wallRight = false;
	b_slidingLeft = false;
	b_slidingRight = false;
	b_rejump = false;
	b_slowMotion = false;
	b_onFloor = false;
	b_die = false;
	b_power = false;
	b_powerReady = false;
	b_playMusic = false;
	b_win = false;

	f_levelTimer = 0;


	tex_eyes = NULL;
	tex_eyes = loadTexture("data/gfx/playerEyesNormal.png");
}

void Player::handleEvents()
{
	///////tipke nekak

	if ( event.type == SDL_KEYUP )
	{
		switch ( event.key.keysym.sym )
		{
		case SDLK_a: b_left = false; break;
		case SDLK_d: b_right = false; break;
		case SDLK_w: b_up = false; break;
		default: break;
		}
	}

	if ( event.type == SDL_KEYDOWN )
	{
		switch ( event.key.keysym.sym )
		{
		case SDLK_a: b_left = true; break;
		case SDLK_d: b_right = true; break;
		case SDLK_w: b_up = true; b_rejump = true; break;
		default: break;
		}
	}
/*
	if ( event.type == SDL_MOUSEBUTTONDOWN )
	{
		switch ( event.button.button )
		{
		case SDL_BUTTON_RIGHT: b_slowMotion = true; PLAYER_SLOWMOTION = true; break;
		case SDL_BUTTON_LEFT: b_power = true; break;
		default: break;
		}
	}

	if ( event.type == SDL_MOUSEBUTTONUP )
	{
		switch ( event.button.button )
		{
		case SDL_BUTTON_RIGHT: b_slowMotion = false; PLAYER_SLOWMOTION = false; break;
		case SDL_BUTTON_LEFT: b_power = false; break;
		default: break;
		}
	}*/
}

void Player::power()
{
	if ( b_power == true && b_inAir == true && b_slowMotion == true && b_powerReady == true)
	{
		b_power = false;
		b_powerReady = false;
		int x,y;
		SDL_GetMouseState(&x,&y);

		Box mouse;
		mouse.x = (float)x + roomX;
		mouse.y = (float)y + roomY;
		mouse.w = 1;
		mouse.h = 1;

		float angle = calculateAngle( box, mouse );

		f_xVel = calculateSpeed( angle, 20, 1 );
		f_yVel = calculateSpeed( angle, 10, 0 );

		FSOUND_PlaySound(FSOUND_FREE, sample_playerForce);
	}
}

void Player::move( std::vector<GameObject*> &gameObjects )
{
    float k = getFPS();

	f_levelTimer += 30*k;

	b_slidingLeft = false;
	b_slidingRight = false;


	////SLOOOOMOOOOO :D
	if ( b_slowMotion == true ) f_speedFactor = 3.00;
	else f_speedFactor = 1.00;

	f_angle = 0;
	if ( b_left == true && b_right == false ) f_angle = -35;
	if ( b_right == true && b_left == false ) f_angle = +35;

	if ( box.a < f_angle ) { if ( (box.a+=120/f_speedFactor*k) >= f_angle ) box.a = f_angle; }
	if ( box.a > f_angle ) { if ( (box.a-=120/f_speedFactor*k) <= f_angle ) box.a = f_angle; }

	if ( b_left == true )
	{
		f_xVel-=(float)33/f_speedFactor*k;

		b_wallRight = false;

		if ( b_wallLeft == true )
		{
			box.a = 0;
			b_slidingLeft = true;
		}
	}
	if ( f_xVel < -1200 ) f_xVel = -1200;
	if ( b_right == true )
	{
		f_xVel+=(float)33*k/f_speedFactor;

		b_wallLeft = false;

		if ( b_wallRight == true )
		{
			box.a = 0;
			b_slidingRight = true;
		}
	}
	if ( f_xVel > 1200 ) f_xVel = 1200;

	b_wallLeft = false;
	b_wallRight = false;

	if ( b_up == true && b_jump == false ) b_jump = true;
	if ( b_up == false ) b_jump = false;

	if ( (b_left == true && b_right == true) || (b_left == false && b_right == false) )
	{
		if ( f_xVel > 0 ) {f_xVel-=60/f_speedFactor*k; if ( f_xVel < 0 ) {f_xVel = 0;}}
		if ( f_xVel < 0 ) {f_xVel+=60/f_speedFactor*k; if ( f_xVel > 0 ) {f_xVel = 0;}}
	}

	////////Jump .... maybe TEMP

	if ( b_jump == true )
	{
		if ( b_rejump == true && b_onFloor == false)
		{
			if ( b_slidingLeft == true ) {f_xVel += 6.000; f_yVel = -10; b_slidingLeft = false; b_wallLeft = false; FSOUND_PlaySound(FSOUND_FREE,sample_playerJump);b_playMusic = true;}
			else if ( b_slidingRight == true ) {f_xVel -= 6.000; f_yVel = -10; b_slidingRight = false; b_wallRight = false;FSOUND_PlaySound(FSOUND_FREE,sample_playerJump);b_playMusic = true;}
		}

		if ( (b_inAir == false )|| (b_powerReady == true ) )
		{
			b_inAir = true;
			f_yVel = -10;
			b_onFloor = false;
			FSOUND_PlaySound(FSOUND_FREE, sample_playerJump);
			b_playMusic = true;
		}

		b_jump = false;
		b_rejump = false;
		b_powerReady = false;
	}


	////////gravitation

	if ( b_inAir == true )
	{
		f_yVel +=(float)30/f_speedFactor*k;

		b_onFloor = false;
	}


	///////za vnaprej collision

	Box next;
	next.x = box.x;
	next.y = box.y;
	next.w = 25;
	next.h = 25;

	//////up-down collision
	float inY = f_yVel;
	next.y += 60*f_yVel/f_speedFactor*k;

	bool b_found = false;

	for ( int n = 0; n < ( signed ) gameObjects.size(); n++ )
	{
		if ( gameObjects[n]->collidable() == true )
		{
			if ( gameObjects[n]->getBox().y >= box.y )
			{
				if ( checkCollisionQuads( next, gameObjects[n]->getBox() ) == true )
				{
					if ( b_playMusic == true && f_yVel > 2 ) FSOUND_PlaySound(FSOUND_FREE,sample_playerHit);
					b_playMusic = false;
					b_inAir = false;
					f_yVel = 0;
					box.y = gameObjects[n]->getBox().y - box.h;
					b_wallLeft = false;
					b_wallRight = false;

					gameObjects[n]->Block_b_light[0] = true;
					b_onFloor = true;
					b_found = true;

					if ( gameObjects[n]->Spiked_b == true && gameObjects[n]->Spiked_b_spikes[0]== true )
					{
						b_die = true;

						if ( gameObjects[n]->i_type == OBJECT_TIMED && gameObjects[n]->Timed_b_state == 0 )
						{
							b_die = false;
						}
					}
				}
			}

			if ( b_found == false ) b_inAir = true; b_playMusic = true;

		}
	}

	for ( int n = 0; n < ( signed ) gameObjects.size(); n++ )
	{
		if ( gameObjects[n]->collidable() == true )
		{
			if ( gameObjects[n]->getBox().y+gameObjects[n]->getBox().h <= box.y )
			{
				if ( checkCollisionQuads( next, gameObjects[n]->getBox() ) == true )
				{
					f_yVel = 1;
					box.y = gameObjects[n]->getBox().y+gameObjects[n]->getBox().h+1;
					gameObjects[n]->Block_b_light[2] = true;

					if ( gameObjects[n]->Spiked_b == true && gameObjects[n]->Spiked_b_spikes[2]== true )
					{
						b_die = true;

						if ( gameObjects[n]->i_type == OBJECT_TIMED && gameObjects[n]->Timed_b_state == 0 )
						{
							b_die = false;
						}
					}
					break;
				}
			}
		}
	}

	/////////////left-right collision

	next.y -= 60*inY/f_speedFactor*k; next.x += 60*f_xVel/f_speedFactor*k;
	next.y -= 1;
	Box right = next;
	right.x += 1;
	Box left = next;
	left.x -= 1;


	for ( int n = 0; n < (signed) gameObjects.size(); n++ )
	{
		if ( gameObjects[n]->collidable() == true )
		{
			if ( gameObjects[n]->getBox().x >= box.x+box.w )
			{
				if ( (b_left == false) || (f_xVel >= 0.000 ) ){
				if ( checkCollisionQuads( right, gameObjects[n]->getBox() ) == true )
				{
					f_xVel = 0;
					box.x = gameObjects[n]->getBox().x - box.w;
					b_wallRight = true;
					gameObjects[n]->Block_b_light[3] = true;
					if ( gameObjects[n]->Spiked_b == true && gameObjects[n]->Spiked_b_spikes[3]== true )
					{
						b_die = true;
						if ( gameObjects[n]->i_type == OBJECT_TIMED && gameObjects[n]->Timed_b_state == 0 )
						{
							b_die = false;
						}
					}
				}
				}
			}

			else if ( gameObjects[n]->getBox().x + gameObjects[n]->getBox().w <= box.x )
			{
				if ( (b_right == false) || ( f_xVel <= 0.000) )

            {
				if ( checkCollisionQuads( left, gameObjects[n]->getBox() ) )
				{
					f_xVel = 0;
					box.x = gameObjects[n]->getBox().x + gameObjects[n]->getBox().w;
					b_wallLeft = true;
					gameObjects[n]->Block_b_light[1] = true;

					if ( gameObjects[n]->Spiked_b == true&& gameObjects[n]->Spiked_b_spikes[1]== true )
					{
						b_die = true;
						if ( gameObjects[n]->i_type == OBJECT_TIMED && gameObjects[n]->Timed_b_state == 0 )
						{
							b_die = false;
						}
					}
				}
				}
			}
		}
	}

	////applyment
	box.x += 60*f_xVel/f_speedFactor*k;
	box.y += 60*f_yVel/f_speedFactor*k;

	for ( int g = 0; g < ( signed) gameObjects.size(); g++ )
	{
		if ( gameObjects[g]->collidable() == true )
		{
			if ( checkCollisionQuads( box, gameObjects[g]->getBox() ) )
			{
				if ( f_yVel < 0 )
				{
					f_yVel = 1;
					box.y = gameObjects[g]->getBox().y+gameObjects[g]->getBox().h+1;
					gameObjects[g]->Block_b_light[2] = true;
					if ( gameObjects[g]->Spiked_b == true && gameObjects[g]->Spiked_b_spikes[2]== true )
					{
						b_die = true;
						if ( gameObjects[g]->i_type == OBJECT_TIMED && gameObjects[g]->Timed_b_state == 0 )
						{
							b_die = false;
						}
					}
				}

				else if ( f_yVel > 0 )
				{
					f_yVel = 0;
					box.y = gameObjects[g]->getBox().y - box.h;
					gameObjects[g]->Block_b_light[0] = true;

					if ( gameObjects[g]->Spiked_b == true && gameObjects[g]->Spiked_b_spikes[0]== true )
					{
						b_die = true;
						if ( gameObjects[g]->i_type == OBJECT_TIMED && gameObjects[g]->Timed_b_state == 0 )
						{
							b_die = false;
						}
					}
				}



			break;
			}
		}

		if ( gameObjects[g]->collidable() == false )
		{
			if ( gameObjects[g]->i_type == OBJECT_FORCE_JUMP )
			{
				if ( checkCollisionQuads(box, gameObjects[g]->getBox() ) == true )
				{
					b_powerReady = true;

				}
			}

			if ( gameObjects[g]->i_type == OBJECT_EXIT )
			{
				if ( checkCollisionQuads(box, gameObjects[g]->getBox() ) == true )
				{
					b_win = true;
				}
			}

		}
	}


	if ( f_yVel == 0 && b_inAir == false )
	{
		if ( f_xVel > 0.000 )
		{
			Gparticles.push_back ( new Particle ( box.x+5,box.y+box.h, (float)(320 - (rand() % 30))));
			Gparticles.push_back ( new Particle ( box.x+5,box.y+box.h, (float)(320 - (rand() % 30))));
			Gparticles.push_back ( new Particle ( box.x+5,box.y+box.h, (float)(320 - (rand() % 30))));
		}
		else if ( f_xVel < 0.000 )
		{
			Gparticles.push_back ( new Particle ( box.x+box.w-5,box.y+box.h, (float)(70 - (rand() % 30))));
			Gparticles.push_back ( new Particle ( box.x+box.w-5,box.y+box.h, (float)(70 - (rand() % 30))));
			Gparticles.push_back ( new Particle ( box.x+box.w-5,box.y+box.h, (float)(70 - (rand() % 30))));
		}
	}

}

void Player::die(std::vector<GameObject*> &gameObjects)
{
	box.x = 0;
	box.y = 0;
	for ( int n = 0; n < (signed) gameObjects.size(); n++ )
	{
		if ( gameObjects[n]->i_type == OBJECT_PLAYER_BLOCK )
		{
			box.x = gameObjects[n]->getBox().x;
			box.y = gameObjects[n]->getBox().y;
			break;
		}
	}
	box.w = 25;
	box.h = 25;
	box.a = 0;

	b_left = false;
	b_right = false;
	b_up = false;

	f_xVel = 0;
	f_yVel = 0;

	f_levelTimer = 0;

	b_die = false;
	b_slowMotion = false;

}

void Player::render()
{
	glPushMatrix();

		glOrtho(0,800,600,0,-1,1);

		///////rotation

		glTranslatef( box.x + box.w/2, box.y + box.h/2, 0 );
		glRotatef( box.a, 0,0,1 );
		glTranslatef( -(box.x + box.w/2), -(box.y+box.h/2), 0);

		//glColor4ub(153,217,234,255);
		glColor4ub(255,255,255,255);

		/// main square
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_player1);

			glBegin(GL_QUADS);
				glTexCoord2i(0,0); glVertex2f(box.x,box.y);
				glTexCoord2i(1,0);glVertex2f(box.x+box.w,box.y);
				glTexCoord2i(1,1);glVertex2f(box.x+box.w,box.y+box.h);
				glTexCoord2i(0,1);glVertex2f(box.x,box.y+box.h);
			glEnd();

		/*glColor4f(0,0,0,0.5);
		glLineWidth(2);

		glBegin(GL_LINE_LOOP);
			glVertex2f(box.x,box.y);
			glVertex2f(box.x+box.w,box.y);
			glVertex2f(box.x+box.w,box.y+box.h);
			glVertex2f(box.x,box.y+box.h);
		glEnd();*/


	glPopMatrix();



	////non rotated

	glPushMatrix();

		glOrtho(0,800,600,0,-1,1);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_eyes );

		glColor4f(255,255,255,255);

		glBegin(GL_QUADS);

			glTexCoord2i(0,0);glVertex2f(box.x,box.y-15);
			glTexCoord2i(1,0);glVertex2f(box.x+box.w,box.y-15);
			glTexCoord2i(1,1);glVertex2f(box.x+box.w,box.y-2);
			glTexCoord2i(0,1);glVertex2f(box.x,box.y-2);
		glEnd();

		glDisable(GL_TEXTURE_2D);


	glPopMatrix();



}

