#ifndef ENT_H
#define ENT_H

#include "Level.hpp"
#include <vector>

class Entity {
public:
	float x, y, dx, dy, w, h;
	int health;
	std::vector<Object> obj;
	AnimationManager anim;
	String Name;
	float timer, timer_end;
	bool life, dir;

	Entity() {};
	Entity(AnimationManager &A, int X, int Y) {
		Name = "Entity";
		health = 10;
		anim = A;
		x = X;
		y = Y;
		dir = false;
		life = true;
		dx = dy = 0;
		timer = timer_end = 0;
	}

	virtual void update(float time) = 0;

	void option(String NAME, float SPEED=0, int HEALTH=10, String FIRST_ANIM="")
	{
		Name = NAME;
		if (FIRST_ANIM!="") anim.Set(FIRST_ANIM);
		w = anim.getW();
		h = anim.getH();
		dx = SPEED;
		dir = false;
		health = HEALTH;	   
	}

	FloatRect getRect(){
		//auto t = anim.animList[anim.currentAnim].frames[0];
		//return FloatRect(Vector2f(float(x), float(y)), Vector2f(float(t.width), float(t.height)));
		return FloatRect(x,y,w,h);
	}

	void draw(RenderWindow &window){
		anim.Draw(window, x, y);
	}
};

#endif ENT_H