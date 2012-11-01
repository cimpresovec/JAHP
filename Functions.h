#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include <SDL_opengl.h>

bool init();

GLuint loadTexture(const std::string &fileName);

struct Box;

bool checkCollisionQuads( Box A, Box B );
float calculateAngle( Box A, Box B);
float calculateSpeed( float angle, float xSpeed, int mode );

float getFPS();

class GameObject;

void setUpLevel ( std::vector<GameObject*> &objects );
void loadLevel ( std::vector<GameObject*> &objects, int levelNumber );
void saveLevel ( std::vector<GameObject*> &objects, int levelNumber );
void setNextGameState ( int nextState );
void changeGameState ();


#endif
