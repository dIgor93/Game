#ifndef PLAY_H
#define PLAY_H

#include "Globals.hpp"
#include "Entity.hpp"

class Player : public Entity {
public:
	//float x, y, dx, dy, h, w;
	//AnimationManager anim;
	bool onLadder, shoot, hit;//, dir;
	enum {stay, walk, duck, jump, climb, swim} STATE;
	std::map<std::string, bool> key;

public:	
	Player(AnimationManager &a);
	void KeyCheck();
	void update(float time);
	void Collision(int dir);
};

Player::Player(AnimationManager &a) {
	Name = "Player";
	health = 1000;
	x=100;
	y=100;
	dx = 0;
	dy = 0;
	auto i = a.currentAnim;
	h = 70;
	w = a.animList[i].frames[0].width;
	shoot = false;
	hit = false;
	dir = false;
	onLadder = true;
	anim = a;
	STATE = stay;
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

	anim.Flip(dir);

	x += dx * time;
	Collision(0);

	dy += 0.0005 * time;
	y += dy * time;
	Collision(1);

	anim.Tick(time);

	key["R"] = key["L"] = key["Up"] = key["Down"] = key["Space"] = false;
}

void Player::Collision(int dir) {
	for (int i=y/tile_size; i<(y+h)/tile_size; i++)
		for (int j=x/tile_size; j<(x+w)/tile_size; j++) {
			if (TileMap[i][j] == '0') {
				if ((dx > 0) && (dir==0)) x = j * tile_size - w;
				if ((dx < 0) && (dir==0)) x = j * tile_size + tile_size;
				if ((dy > 0) && (dir==1)) {y = i * tile_size - h; dy = 0; onLadder=true; STATE=stay; };
				if ((dy < 0) && (dir==1)) {y = i * tile_size + tile_size; dy = 0;}
			}
		}
}

#endif PLAY_H