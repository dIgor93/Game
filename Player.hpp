#ifndef PLAY_H
#define PLAY_H

#include "Globals.hpp"
#include "Entity.hpp"
#include "Level.hpp"

class Player : public Entity {
public:
	//float x, y, dx, dy, h, w;
	//AnimationManager anim;
	bool onLadder, shoot, hit;//, dir;
	enum {stay, walk, duck, jump, climb, swim} STATE;
	std::map<std::string, bool> key;

public:	
	Player(AnimationManager &a, Level &lev, int X, int Y);
	void KeyCheck();
	void update(float time);
	void Collision(int dir);
};

Player::Player(AnimationManager &a, Level &lev, int X, int Y):Entity(a,X,Y) {
	Name = "Player";
	option("Player",0,100,"stay");
	STATE=stay; hit=0;
	obj = lev.GetAllObjects();
	//shoot = false;
	dir = false;
	onLadder = false;
}

void Player::KeyCheck() {
	if (key["L"]) {
		dir = true;
		if (STATE!=duck) dx=-0.3;
		if (STATE==stay) STATE=walk;
	}
	if (key["R"]) {
		dir = false;
		if (STATE!=duck) dx=0.3;
		if (STATE==stay) STATE=walk;
	}
	if (key["Up"]) {
		if (onLadder) STATE=climb;
		if (STATE==walk || STATE==stay) {
			dy = -0.37;
			STATE = jump;
		}
		if (STATE==swim || STATE==climb) { dy =  -0.05;	}
	}
	if (key["Down"]) {
		if (STATE==walk || STATE==stay) {
			dy = 0;
			STATE = duck;
		}
		if (STATE==swim || STATE==climb) { dy =  0.05; }
	}
	if (key["Space"]) { shoot = true; }

	//////////////////////RELEASEING KEYS////////////////////////

	if (!(key["L"]||key["R"])) {
		dx = 0;
		if (STATE==walk) STATE = stay;
	}
	if (!(key["Up"]||key["Down"])) {
		if (STATE==swim || STATE==climb) dy = 0;
	}
	if (!key["Down"]) {
		if (STATE==duck) { STATE = stay; }
	}
	if (!key["Space"]) {
		shoot = false;
	}
}

void Player::update(float time) {
	KeyCheck();

	if (STATE==stay) anim.Set("stay");
	if (STATE==walk) anim.Set("walk");
	if (STATE==jump) anim.Set("jump");
	if (STATE==duck) anim.Set("duck");
	if (STATE==climb) {
		anim.Set("climb");
		anim.Pause();
		if (dy!=0) anim.Play();
		if (!onLadder) STATE=stay;
	}

	if (shoot) { 
		anim.Set("shoot"); 
		if (STATE==walk) anim.Set("shootAndWalk");
	}

	if (hit) { 
		timer+=time; 
		if (timer>1000) {
			hit=false; 
			timer=0;
		}
		anim.Set("hit");
	}
	//dir = (dx < 0);
	anim.Flip(dir);

	x += dx * time;
	Collision(0);

	if (STATE!=climb) dy += 0.0005 * time;
	y += dy * time; onLadder = false;
	Collision(1);

	anim.Tick(time);

	key["R"] = key["L"] = key["Up"] = key["Down"] = key["Space"] = false;
}

void Player::Collision(int num)
{	

	for (int i=0;i<obj.size();i++) 
		if (getRect().intersects(obj[i].rect))
		{
			if (obj[i].name=="solid")
			{           
				if (dy>0 && num==1)	{ y = obj[i].rect.top -  h;  dy=0;   STATE=stay;}
				if (dy<0 && num==1)	{ y = obj[i].rect.top + obj[i].rect.height ;   dy=0;}
				if (dx>0 && num==0)	{ x =  obj[i].rect.left -  w; }
				if (dx<0 && num==0)	{ x =  obj[i].rect.left + obj[i].rect.width ;}
			} 

			if (obj[i].name=="ladder")	{ onLadder=true; }

			if (obj[i].name=="SlopeLeft")
			{  FloatRect r = obj[i].rect;
			int y0 = (x+w/2-r.left) * r.height/r.width+ r.top - h;
			if (y>y0)
				if (x+w/2>r.left)
				{y =  y0; dy=0; STATE=stay;}
			}

			if (obj[i].name=="SlopeRight")
			{   FloatRect r = obj[i].rect;
			int y0 = - (x+w/2-r.left) * r.height/r.width + r.top+r.height - h;
			if (y > y0)
				if (x+w/2<r.left+r.width)
				{y = y0 ; dy=0; STATE=stay;}
			}

		}
}

#endif PLAY_H