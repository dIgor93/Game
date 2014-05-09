#ifndef ENT_H
#define ENT_H

class Entity {
public:
	float x, y, dx, dy, w, h;
	int health;
	AnimationManager anim;
	String Name;
	bool life, dir;

	Entity() {};
	Entity(AnimationManager &A, int X, int Y, int Health) {
		Name = "Entity";
		health = Health;
		anim = A;
		x = X;
		y = Y;
		dir = false;
		life = true;
		dx = dy = 0;
	}

	virtual void update(float time) = 0;

	FloatRect getRect(){
		auto t = anim.animList[anim.currentAnim].frames[0];
		return FloatRect(Vector2f(float(x), float(y)), Vector2f(float(t.width), float(t.height)));
	}

	void draw(RenderWindow &window){
		anim.Draw(window, x, y);
	}
};

#endif ENT_H