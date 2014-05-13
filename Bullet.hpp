#ifndef BULL_H
#define BULL_H

#include <SFML\Graphics.hpp>
#include "Animation.hpp"
#include "Globals.hpp"
#include "Entity.hpp"
#include "Level.hpp"

using namespace sf;

class Bullet:public Entity
{
public:

	Bullet(AnimationManager &a, Level &lev,int x,int y,bool dir):Entity(a,x,y)
	{
		option("Bullet", 0.3, 10, "shoot");

		if (dir) dx=-0.3;
   		obj = lev.GetObjects("solid");
		
	}

	void update(float time)
	{
		x+=dx*time;
		
		for (int i=0;i<obj.size();i++) 
			if (getRect().intersects(obj[i].rect)) {
				Health=0;
			}

		if (Health<=0) {
			anim.set("explode");
			dx=0;
		    if (anim.isPlaying()==false) ;
			life=false;    
		}

		anim.tick(time);		 
	}

};


#endif BULL_H