#ifndef BULL_H
#define BULL_H

#include <SFML\Graphics.hpp>
#include "Animation.hpp"
#include "Globals.hpp"
#include "Entity.hpp"

using namespace sf;

class Bullet : public Entity {
public:
	//float x, y, dx, dy, w, h;
	//AnimationManager anim;
	//bool life;
public:
	Bullet() {};
	Bullet(AnimationManager &a, int X, int Y, bool dir, int Health);
	void update(float time);
	void draw(RenderWindow &window);
};

Bullet::Bullet(AnimationManager &a, int X, int Y, bool dir, int Health) {
	Name = "Bullet";
	health = Health;
	anim = a;
	anim.Set("move");
	anim.Flip(dir);
	x = X;
	y = Y;
	dx = 0.3;
	dy = 0;
	if (dir==true) dx=-0.3;
	w = 91;
	h = 70;
	life = true;
}

void Bullet::update(float time) {
	x += dx * time;

	for (int i = (y+offsetY)/tile_size; i<(y+h+offsetY)/tile_size; i++)
		for (int j= (x+offsetX)/tile_size; j<(x+w+offsetX)/tile_size; j++)
			if (TileMap[i][j]=='0') {
				//anim.Set("explode");
				dx = 0;
				life = false;
			}
			anim.Tick(time);
}
void Bullet::draw(RenderWindow &window) {
	anim.Draw(window, x, y);
}

#endif BULL_H