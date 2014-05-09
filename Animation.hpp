#ifndef ANIM_H
#define ANIM_H

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

using namespace sf;

class Animation
{
public:
	std::vector<IntRect> frames, frames_flip;
	float currentFrame;
	float speed;
	bool flip, isPlaying;
	Sprite sprite;
public:
	Animation(Texture &t, int x, int y, int w, int h, int count, float speed, int step);
	void Tick(float time);
	Animation() { };
};

Animation::Animation(Texture &t, int x, int y, int w, int h, int count, float Speed, int step) {
	speed = Speed;
	sprite.setTexture(t);

	currentFrame = 0;
	isPlaying = true;
	flip = false;

	for (int i=0; i<count; i++){
		frames.push_back(IntRect(x + i*step, y, w, h));
		frames_flip.push_back(IntRect(x+i*step+w, y, -w, h));
	}
}

void Animation::Tick(float time) {
	if (!isPlaying) return;
	currentFrame += speed * time;
	while (currentFrame > frames.size()) 
		currentFrame-=frames.size();

	int i = currentFrame;
	sprite.setTextureRect( frames[i] );
	if (flip) sprite.setTextureRect(frames_flip[i]);

}


class AnimationManager {
public:
	String currentAnim;
	std::map<String, Animation> animList;
public:
	AnimationManager () {};
	void Create(String name, Texture &t, int x, int y, int w, int h, int count, float speed, int step);
	void Draw(RenderWindow &window, int x=0, int y=0);
	void Set(String name);
	void Flip(bool b);
	void Tick(float time);
	void Pause();
	void Play();
};

void AnimationManager::Create(String name, Texture &t, int x, int y, int w, int h, int count, float speed, int step) {
	animList[name] = Animation(t,x,y,w,h,count,speed,step);
	currentAnim = name;
}

void AnimationManager::Draw(RenderWindow &window, int x, int y){
	animList[currentAnim].sprite.setPosition(x,y);
	window.draw(animList[currentAnim].sprite);
}

void AnimationManager::Set(String name){
	currentAnim = name;
}
void AnimationManager::Flip(bool b){
	animList[currentAnim].flip = b;
}

void AnimationManager::Tick(float time){
	animList[currentAnim].Tick(time);
}

void AnimationManager::Pause(){
	animList[currentAnim].isPlaying = false;
}
void AnimationManager::Play(){
	animList[currentAnim].isPlaying = true;
}

#endif ANIM_H