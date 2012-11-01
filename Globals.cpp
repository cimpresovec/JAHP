#include "Globals.h"
#include <SDL.h>
#include <fmod.h>


SDL_Event event;

unsigned int tex_spikes = NULL;
unsigned int tex_block1 = NULL;
unsigned int tex_menu = NULL;
unsigned int tex_playgame = NULL;
unsigned int tex_selectlevel = NULL;
unsigned int tex_editor = NULL;
unsigned int tex_exit = NULL;
unsigned int tex_player1 = NULL;
unsigned int tex_door = NULL;
unsigned int tex_font = NULL;
unsigned int tex_done = NULL;
unsigned int tex_gold = NULL;
unsigned int tex_silver = NULL;
unsigned int tex_bronze = NULL;
unsigned int tex_eyes = NULL;
unsigned int tex_arrow = NULL;
unsigned int tex_info = NULL;

unsigned int i_levelNumber = 1;

bool b_music = true;

FSOUND_SAMPLE *sample_playerJump = 0, * sample_playerHit = 0, * sample_playerForce = 0;
FSOUND_STREAM * song2;


GAME_STATE currentGameState = STATE_NULL;
GAME_STATE nextGameState = STATE_NULL;


bool PLAYER_SLOWMOTION = false;

float roomX = 0;
float roomY = 0;

float maxRoomX = 0;
float maxRoomY = 0;
float minRoomX = 0;
float minRoomY = 0;


Player * Gplayer = NULL;

GameState * gameState = NULL;

std::vector<class Particle*> Gparticles;

std::vector<class Score*> o_scores;

int FPSA = 0;

