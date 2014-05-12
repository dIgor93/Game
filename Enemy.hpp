#ifndef ENEMY_H
#define ENEMY_H

#include <SFML\Graphics.hpp>
#include "Globals.hpp"
#include "Level.hpp"

using namespace sf;

class Enemy : public Entity {
public:
	enum {stay, walk, duck, jump, climb, swim} STATE;
	std::map<std::string, bool> key;
	float offsetDx;
public:
	Enemy();
	Enemy(AnimationManager &a, Level &lev, int X, int Y);
	void update(float time);
	void Collision();
};

Enemy::Enemy() {}

Enemy::Enemy(AnimationManager &a, Level &lev, int X, int Y):Entity(a,X,Y) {
	option("Enemy", 0.3, 10, "move");

	//Name = "Enemy";
	//health = Health;
	//anim = a;
	//anim.animList["move"].sprite.setColor(Color::Blue);
	//anim.Flip(dir);
	//anim.Set("move");
	//x = X;
	//y = Y;
	//dx = 0.2;
	//offsetDx = 0;
	//dy = 0.02;
	//if (dir==true) dx=-0.2;
	//w = 91;
	//h = 70;
	//life = true;
}

void Enemy::update(float time) {
	x += dx * time; // + (offsetDx - offsetX);
	//offsetDx = offsetX;
	if (timer>3200) {dx*=-1;timer=0;} 
	if (health<=0) {anim.Set("explode"); dx=0;
			           timer_end+=time;
		               if (timer_end>4000) life=false;
		              }
	//Collision();

	//anim.Flip(dx<0);

	//if ((y>500)||(y<10)) dy=-dy; 
	
	/*if (dy<0)	
		dy += 0.00005 * time;
	else 
		dy -= 0.00005 * time; 
		*/
	//y += dy * time;
	//Collision();

	anim.Tick(time);
}

void Enemy::Collision() {
	int k = 0;
	for (int i=(y+offsetY)/tile_size; i<(y+h+offsetY)/tile_size; i++) {
		for (int j=(x+offsetX)/tile_size; j<(x+w+offsetX)/tile_size; j++) {
			if (TileMap[i][j] == '0') {
				if (dx > 0) { 
					//std::cout <<" collision1 " << x << " offset " << offsetX << std::endl;
					//anim.Flip(false);
					x = j * tile_size - w - offsetX; 
					dx = -dx; 
					//std::cout << " collision2 " << x << " offset " << offsetX << std::endl;
					//anim.Flip(false);
				} else 
					if (dx < 0) { 
						x = j * tile_size + tile_size - offsetX; 
						dx = -dx; 
						//anim.Flip(true);
					};
			}
		}
	}
}



#endif ENEMY_H