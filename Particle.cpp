#include "Classes.h"
#include "Functions.h"

Particle::Particle(float x, float y, float angle)
{
	f_x = x;
	f_y = y;

	f_xVel = calculateSpeed( angle, 9, 1 );
	f_yVel = calculateSpeed( angle, 15, 0 );

	timer = 0;

}

void Particle::move()
{
    float k = getFPS();
	if ( f_yVel < 10 ) f_yVel += (float)0.3;

	f_x += 60*f_xVel*k;
	f_y += 60*f_yVel*k;

	timer += 200 * k;
}
