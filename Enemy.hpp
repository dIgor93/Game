#ifndef ENEMY_H
#define ENEMY_H

#include <SFML\Graphics.hpp>
#include "Globals.hpp"
#include "Level.hpp"

using namespace sf;

class ENEMY: public Entity
{
public:

	ENEMY(AnimationManager &a, Level &lev,int x,int y):Entity(a,x,y)
	{
		option("Enemy", 0.02, 15, "walk");	
	}

	void update(float time)
	{	
		x += dx * time;
		timer+=time;
		anim.flip(dx > 0);
		if (timer>13000) {dx*=-1;timer=0;}

		if (Health<=0) {anim.set("dead"); dx=0;
		timer_end+=time;
		if (timer_end>4000) life=false;
		}

		anim.tick(time);
	}
};

class MovingPlatform: public Entity
{
public:
	MovingPlatform(AnimationManager &a, Level &lev,int x,int y):Entity(a,x,y)
	{
		option("MovingPlatform", 0.05, 0, "move");
	}

	void update(float time)
	{	
		x += dx * time;
		timer+=time;
		if (timer>6000) {dx*=-1;timer=0;}
		anim.tick(time);
	}

};



#endif ENEMY_H