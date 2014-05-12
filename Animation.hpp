#ifndef ANIM_H
#define ANIM_H

#include <SFML/Graphics.hpp>
#include "TinyXML\tinyxml.h"
#include <map>
#include <vector>
#include "Level.hpp"
using namespace sf;

class Animation
{
public:
	std::vector<IntRect> frames, frames_flip;
	float currentFrame, speed;
	bool loop, flip, isPlaying;   // loop показвает зациклена ли анимация. Например анимация взрыва должна проиграться один раз и остановиться, loop=false
	Sprite sprite;

	Animation()
	{
		currentFrame = 0;
		isPlaying=true;
		flip=false;
	}

	void tick(float time)
	{
		if (!isPlaying) return;

		currentFrame += speed * time;

		if (currentFrame > frames.size()) { currentFrame -= frames.size();
		if (!loop) {isPlaying=false; return;}
		}

		int i = currentFrame;
		if (flip) {
			sprite.setTextureRect( frames_flip[i]);
			std::cout << frames_flip[i].left << std::endl;
		}
		else {
			sprite.setTextureRect( frames[i] ); 
			std::cout << frames[i].left << std::endl;
		}
	}

};



class AnimationManager
{

public:
	String currentAnim;
	std::map<String, Animation> animList;

	AnimationManager()
	{}

	~AnimationManager()
	{ animList.clear();
	}

	//создание анимаций вручную
	void Create(String name, Texture &texture, int x, int y, int w, int h, int count, float speed, int step=0, bool Loop=true)
	{
		Animation a;
		a.speed = speed;
		a.loop = Loop;
		a.sprite.setTexture(texture);
		a.sprite.setOrigin(0,h);

		for (int i=0;i<count;i++)
		{			
			a.frames.push_back( IntRect(x+i*step, y, w, h)  );
			a.frames_flip.push_back( IntRect(x+i*step+w, y, -w, h)  );
		}
		animList[name] = a;
		currentAnim = name;
	}

	//загрузка из файла XML
	void loadFromXML(std::string fileName,Texture &t)
	{	  
		TiXmlDocument animFile(fileName.c_str());

		animFile.LoadFile();

		TiXmlElement *head;
		head = animFile.FirstChildElement("sprites");

		TiXmlElement *animElement;
		animElement = head->FirstChildElement("animation");
		while(animElement)
		{

			Animation anim;
			currentAnim = animElement->Attribute("title");
			int delay = atoi(animElement->Attribute("delay")); 
			anim.speed = 1.0/delay; anim.sprite.setTexture(t); 

			TiXmlElement *cut;
			cut = animElement->FirstChildElement("cut");
			while (cut)
			{
				int x = atoi(cut->Attribute("x"));
				int y = atoi(cut->Attribute("y"));
				int w = atoi(cut->Attribute("w"));
				int h = atoi(cut->Attribute("h"));

				anim.frames.push_back( IntRect(x,y,w,h) );
				anim.frames_flip.push_back( IntRect(x+w,y,-w,h)  );
				cut = cut->NextSiblingElement("cut");
			}

			anim.sprite.setOrigin(0,anim.frames[0].height);

			animList[currentAnim] = anim;
			animElement = animElement->NextSiblingElement("animation");
		}
	}
	void Set(String name)
	{   
		currentAnim = name; 
		animList[currentAnim].flip = 0;
	}

	void Draw(RenderWindow &window,int x=0, int y=0)
	{
		animList[currentAnim].sprite.setPosition(x,y);
		window.draw( animList[currentAnim].sprite );
	}

	void Flip(bool b) {animList[currentAnim].flip = b;}

	void Tick(float time)	 {animList[currentAnim].tick(time);}

	void Pause() {animList[currentAnim].isPlaying=false;}

	void Play()  {animList[currentAnim].isPlaying=true;}

	void Play(String name)  {animList[name].isPlaying=true;}

	bool isPlaying()  {return animList[currentAnim].isPlaying;}

	float getH()  {return animList[currentAnim].frames[0].height;}

	float getW() {return animList[currentAnim].frames[0].width;}

};


/*
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
float getH();
float getW();
bool isPlaying();
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

bool AnimationManager::isPlaying()  {
return animList[currentAnim].isPlaying;
}

float AnimationManager::getH()  {
return animList[currentAnim].frames[0].height;
}

float AnimationManager::getW() {
return animList[currentAnim].frames[0].width;
}
*/
#endif ANIM_H