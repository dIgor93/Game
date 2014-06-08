#ifndef PLUH_H
#define PLUH_H

#include <SFML\Graphics.hpp>
#include "Globals.hpp"
#include "Level.hpp"

using namespace sf;

class Plyuhi: public Entity
{
public:

	Plyuhi(AnimationManager &a, Level &lev,int x,int y, std::string Name, int bonus):Entity(a,x,y)
	{
		option(Name, 0.00, bonus, "walk2");	
	}

	void update(float time)
	{	
		if (Health<=0) {
			anim.set("dead");
			timer_end+=time;
			if (timer_end>4000) life=false;
		}
		anim.tick(time);
	}
};

#endif PLUH_H