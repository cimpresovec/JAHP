#include <SDL.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "Functions.h"
//#include <Algorithm>
#include "Globals.h"
#include "Classes.h"
#include <fmod.h>

StateEditor::StateEditor()
{
	loadLevel(o_gameObjects, i_levelNumber);

	b_placeObject = false; b_selectObject = false; b_deleteObject = true;
	CURRENT_OBJECT = 1;
	f_left = 0; f_right = 0; f_speed = 0; f_time = 60;
	direction = 1;
	play = false;
	objectNumber = o_gameObjects.size()-1;
	left = false; right = false; up = false ;down = false;

}

void StateEditor::handleEvents()
{
//////////////EVENT////////////////


	if ( SDL_PollEvent(&event) )
	{
		if ( event.type == SDL_QUIT ) setNextGameState(0);

			////////////LEVEL EDITOR EVENT
			if ( event.type == SDL_MOUSEBUTTONDOWN )
			{
				if (event.button.button == SDL_BUTTON_LEFT) b_placeObject = true;
				if (event.button.button == SDL_BUTTON_RIGHT) b_selectObject = true;
			}

			if ( event.type == SDL_MOUSEBUTTONUP )
			{
				if (event.button.button == SDL_BUTTON_LEFT) b_placeObject = false;
				if (event.button.button == SDL_BUTTON_RIGHT) b_selectObject = false;
			}

			if ( event.type == SDL_KEYDOWN )
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_LEFT: f_left+=25; left = true;  break;
					case SDLK_RIGHT: f_right+=25; right = true; break;
					case SDLK_DOWN: f_left = 0; f_right = 0; down = true; break;
					case SDLK_UP: up = true; break;
					default: break;
				}
			}

			if (event.type == SDL_KEYUP )
			{
				switch(event.key.keysym.sym)
				{
				case SDLK_1: CURRENT_OBJECT = OBJECT_BLOCK; break;
				case SDLK_2: CURRENT_OBJECT = OBJECT_SPIKED; break;
				case SDLK_3: CURRENT_OBJECT = OBJECT_CONTROL; break;
				case SDLK_4: CURRENT_OBJECT = OBJECT_BREAKABLE; break;
				case SDLK_5: CURRENT_OBJECT = OBJECT_TIMED; break;
				case SDLK_6: CURRENT_OBJECT = OBJECT_PLAYER_BLOCK; break;
				case SDLK_7: CURRENT_OBJECT = OBJECT_FORCE_JUMP; break;
				case SDLK_8: CURRENT_OBJECT = OBJECT_EXIT; break;
				case SDLK_0: saveLevel(o_gameObjects, 0);break;//if ( play == false ) {saveLevel(o_gameObjects, 1); setUpLevel(o_gameObjects); play = true; objectNumber = NULL; } break;
				case SDLK_ESCAPE: setNextGameState(2);break;
				case SDLK_e:
					{
						for ( int n = 0; n < ( signed ) o_gameObjects.size(); n++ )
						{
							GameObject * p;
							p = o_gameObjects[n];
							delete p;
						}
						o_gameObjects.clear(); break;
					}
				/*case SDLK_c:
					{
						for ( int n = 0; n < 200; n++ )
						{
							o_gameObjects.push_back(new Block(25,25));
						}
						break;
					}*/
				case SDLK_LEFT: f_left+=25; left = false;  break;
				case SDLK_RIGHT: f_right+=25; right = false; break;
				case SDLK_DOWN: f_left = 0; f_right = 0; down = false; break;
				case SDLK_UP: up = false; break;
				//case SDLK_j: while ( 1 ) { std::cin >> f_speed; if ( std::cin.good() == true ) break; } break;
				//case SDLK_l: while ( 1 ) { std::cin >> f_time; if ( std::cin.good() == true ) break; } break;
				//case SDLK_DELETE: if ( b_deleteObject == false ) b_deleteObject = true; else if ( b_deleteObject == true ) b_deleteObject = false; break;
				default: break;
				}
			}
	}


}



void StateEditor::logic()
{
	/////////////LEVEL EDITOR LOGIC

		if ( b_selectObject == true && play == false)
		{
			//b_selectObject = false;
			int x,y;
			SDL_GetMouseState(&x,&y);

			Box bux;
			bux.x = (float)x;
			bux.y = (float)y;
			bux.w = 1;
			bux.h = 1;

			bux.x += (int)roomX;
			bux.y += (int)roomY;

			bool b_doIt = false;

			for ( int n = 0; n < (signed)o_gameObjects.size(); n++ )
			{
				if ( checkCollisionQuads( bux, o_gameObjects[n]->getBox() ) == true ) { b_doIt = true; break; }
			}

			if ( b_doIt == true )
			{

			for ( int n = 0; n < (signed)o_gameObjects.size(); n++ )
			{
				if ( checkCollisionQuads(bux, o_gameObjects[n]->getBox()) == true )
				{
					objectNumber = n; break;

					if ( o_gameObjects[n]->i_type == OBJECT_TIMED ) f_time = o_gameObjects[n]->Timed_f_time;
				}
			}

			if ( b_deleteObject == true )
			{
				o_gameObjects.erase(o_gameObjects.begin()+objectNumber);
				objectNumber = NULL;
			}

			o_gameObjects[objectNumber]->Timed_f_time = f_time;
			}
		}

		if ( b_placeObject == true && play == false )
		{
			//b_placeObject = false;

			int x,y;
			SDL_GetMouseState(&x,&y);

			x += (int)roomX;
			y += (int)roomY;

			x -= x%25;
			y -= y%25;

			Box bux;
			bux.x = (float)x;
			bux.y = (float)y;
			bux.w = 1;
			bux.h = 1;

			bool b_doIt = true;

			for ( int n = 0; n < (signed)o_gameObjects.size(); n++ )
			{
				if ( checkCollisionQuads( bux, o_gameObjects[n]->getBox() ) == true ) { b_doIt = false; break; }
			}

			if ( b_doIt == true )
			{

			switch(CURRENT_OBJECT)
			{
			case OBJECT_BLOCK: o_gameObjects.push_back(new Block((float)x,(float)y)); break;
			case OBJECT_SPIKED: o_gameObjects.push_back(new SpikedBlock((float)x,(float)y)); break;
			case OBJECT_CONTROL: o_gameObjects.push_back(new ControlBlock((float)x,(float)y)); break;
			case OBJECT_BREAKABLE: o_gameObjects.push_back( new BreakableBlock((float)x,(float)y)); break;
			case OBJECT_TIMED: o_gameObjects.push_back( new TimedBlock((float)x,(float)y, f_time) ); break;
			case OBJECT_PLAYER_BLOCK: o_gameObjects.push_back( new PlayerBlock((float)x,(float)y)); break;
			case OBJECT_FORCE_JUMP: o_gameObjects.push_back( new ForceJumpBlock((float)x,(float)y)); break;
			case OBJECT_EXIT: o_gameObjects.push_back( new ExitBlock((float)x,(float)y)); break;
			}


			objectNumber = o_gameObjects.size()-1;

			}
		}
		//b_placeObject = false; b_selectObject = false;


		 if ( play == false )
		{
			if ( left == true ) roomX -= 25;
			if ( right == true ) roomX += 25;
			if ( up == true ) roomY -=25;
			if ( down == true ) roomY += 25;

			if ( roomX < 0 ) roomX = 0;
			if ( roomY < 0 ) roomY = 0;
		}

}


void StateEditor::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

		glOrtho(0,800,600,0,-1,1);

		glColor4ub(0,124,20,255);

			glBegin(GL_QUADS);
				glVertex2f(0,0);
				glVertex2f(800,0);
				glVertex2f(800,600);
				glVertex2f(0,600);
			glEnd();


		glPopMatrix();

		glPushMatrix();

		glTranslated ( -(float)roomX/400, (float)roomY/300, 0);

		for ( int n = 0; n < (signed)o_gameObjects.size(); n++ )
		{

			o_gameObjects[n]->render();

		}

		glPopMatrix();

		SDL_GL_SwapBuffers();


}


