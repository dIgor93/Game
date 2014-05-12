#ifndef BULL_H
#define BULL_H

#include <SFML\Graphics.hpp>
#include "Animation.hpp"
#include "Globals.hpp"
#include "Entity.hpp"
#include "Level.hpp"

using namespace sf;

class Bullet : public Entity {
public:
	//float x, y, dx, dy, w, h;
	//AnimationManager anim;
	//bool life;
public:
	Bullet() {};
	Bullet(AnimationManager &a, Level &lev, int X, int Y, bool dir);
	void update(float time);
	void draw(RenderWindow &window);
};

Bullet::Bullet(AnimationManager &a, Level &lev, int X, int Y, bool dir):Entity(a,X,Y) {
	option("Bullet", 0.3, 10, "move");
	if (dir) dx=-0.3;
	obj = lev.GetObjects("solid");
	//w = 91;
	//h = 70;
	//life = true;
	obj = lev.GetObjects("solid");
}

void Bullet::update(float time) {
	x += dx * time;

	for (int i=0;i<obj.size();i++) 
		if (getRect().intersects(obj[i].rect)) { health=0; }
		if (health<=0) {
			anim.Set("explode");dx=0;
			if (anim.isPlaying()==false) life=false;
		}
		anim.Tick(time);
}
void Bullet::draw(RenderWindow &window) {
	anim.Draw(window, x, y);
}

#endif BULL_H