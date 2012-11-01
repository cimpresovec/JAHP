#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL.h>
#include <fmod.h>
#include <vector>

extern SDL_Event event;

extern FSOUND_SAMPLE *  sample_playerJump;
extern FSOUND_SAMPLE * sample_playerHit;
extern FSOUND_SAMPLE * sample_playerForce;

extern FSOUND_STREAM * song2;

extern unsigned int tex_spikes;
extern unsigned int tex_block1;
extern unsigned int tex_player1;
extern unsigned int tex_door;
extern unsigned int tex_eyes;
extern unsigned int tex_done;
extern unsigned int tex_gold;
extern unsigned int tex_silver;
extern unsigned int tex_bronze;
extern unsigned int tex_arrow;



///menu
extern unsigned int tex_info;
extern unsigned int tex_menu;
extern unsigned int tex_playgame;
extern unsigned int tex_selectlevel;
extern unsigned int tex_editor;
extern unsigned int tex_exit;
extern unsigned int tex_font;

extern unsigned int i_levelNumber;

unsigned const short int OBJECT_BLOCK = 1;
unsigned const short int OBJECT_SPIKED = 2;
unsigned const short int OBJECT_CONTROL = 3;
unsigned const short int OBJECT_BREAKABLE = 4;
unsigned const short int OBJECT_TIMED = 5;
unsigned const short int OBJECT_PLAYER_BLOCK = 6;
unsigned const short int OBJECT_FORCE_JUMP = 7;
unsigned const short int OBJECT_EXIT = 8;

enum GAME_STATE
{
	STATE_NULL, STATE_INTRO,STATE_MENU,STATE_LEVEL, STATE_SELECTION ,STATE_EXIT, STATE_EDITOR, STATE_INFO
};

extern GAME_STATE currentGameState;
extern GAME_STATE nextGameState;

class Player;

extern Player * Gplayer;

extern bool PLAYER_SLOWMOTION;
extern bool b_music;

extern float roomX;
extern float roomY;
extern float maxRoomX;
extern float maxRoomY;
extern float minRoomX;
extern float minRoomY;

class GameState;
extern GameState * gameState;

class Particle;
extern std::vector<class Particle*> Gparticles;

class Score;

extern std::vector<class Score*> o_scores;

extern int FPSA;


#endif
