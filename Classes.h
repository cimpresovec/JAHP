#ifndef CLASSES_H
#define CLASSES_H

#include <vector>
#include <string>

class GameObject;


struct Box
{
public:
	float x,y,w,h,a;
};

class Score
{
public:

	int i_number, i_highScore, i_B, i_S, i_G;
	bool b_unlocked;
};

class Font
{
private:
	Box num[10];
	char let[10];

public:

	Font();

	void render(int number, int X, int Y);
};

class Particle
{
private:

public:

	float f_x,f_y;

	float timer;

	float f_xVel, f_yVel;

	Particle ( float x, float y, float angle );

	void move();

};



class GameState
{
public:

	virtual void handleEvents() {};
	virtual void logic() {};
	virtual void render() {};
};

class Player
{
private:

	float f_xVel, f_yVel, f_speedFactor;

	bool b_inAir, b_up, b_jump, b_wallLeft, b_wallRight, b_slidingLeft, b_slidingRight, b_rejump, b_slowMotion, b_onFloor, b_power, b_powerReady, b_playMusic;

	unsigned int tex_eyes;

	float f_angle;


public:

	bool b_left, b_right, b_die, b_win;

	float f_levelTimer;

	Box box;

	Player();

	void handleEvents();
	void move( std::vector<GameObject*> &gameObjects );
	void render();
	void power();
	void die(std::vector<GameObject*> &gameObjects);
};

class StateLevel : public GameState
{
private:
	Player o_player;

	std::vector<class GameObject*> o_gameObjects;

	Font font;

	bool b_go, b_restart;

public:

	StateLevel();
	~StateLevel();

	void handleEvents();
	void logic();
	void render();
};

class Info : public GameState
{
    public:

    void handleEvents();
    void logic();
    void render();
};

class StateEditor : public GameState
{
private:

	std::vector<class GameObject*> o_gameObjects;

	//////////LEVEL EDITOR
	bool b_placeObject, b_selectObject, b_deleteObject;
	unsigned short int CURRENT_OBJECT;
	bool play;
	float f_left, f_right, f_speed, f_time;
	bool direction;
	int objectNumber;
	bool left,right,up,down	;

public:

	StateEditor();

	void handleEvents();
	void logic();
	void render();
};

class StateMenu : public GameState
{
private:

	Box play,select,editor,exit;

public:

	StateMenu();

	void handleEvents();
	void logic();
	void render();
};

class StateSelection : public GameState
{
private:

	int i_numberOfLevels;

public:

	StateSelection();

	void handleEvents();
	void logic();
	void render();
};


class GameObject
{
protected:

	Box box;

	bool b_collidable;

public:

	unsigned short int i_type;

	float f_timer;
	float Timed_f_time;
	bool Timed_b_state;

	bool b_line[4];
	bool Block_b_light[4];
	bool Spiked_b, Spiked_b_spikes[4];

	GameObject();
	~GameObject();

	virtual void move() {};
	virtual void logic() {};
	virtual void render() {};

	bool collidable() { return b_collidable; };
	Box getBox() { return box; };

};

class PlayerBlock : public GameObject
{
public:
	PlayerBlock(float x,float y);

	void render();
};

class ForceJumpBlock : public GameObject
{
public:
	ForceJumpBlock(float x, float y);

	void render();
};


class TimedBlock : public GameObject
{
public:
	TimedBlock ( float x, float y, float time );

	void logic();
	void render();
};

class Block : public GameObject
{

public:

	Block( float x, float y );

	void render();
};

class SpikedBlock : public GameObject
{

public:

	SpikedBlock( float x, float y );

	void render();
};

class BreakableBlock : public GameObject
{
public:

	BreakableBlock( float x, float y );

	void logic();
	void render();
};

class ControlBlock : public GameObject
{

public:

	ControlBlock( float x, float y );

	void render();
};

class ExitBlock : public GameObject
{
public:

	ExitBlock( float x , float y );

	void render();
};




#endif
