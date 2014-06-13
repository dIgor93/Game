#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
//#include <string>

#include "TinyXML\tinyxml.h"

#include "Animation.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "PlayerScores.hpp"

using namespace sf; 
using namespace std;


int main()
{  
	///////////// инициализация ///////////////////////////
	RenderWindow window(VideoMode(600, 450), "SFML works!");

	View view( FloatRect(0, 0, 600, 450) );

	Level lvl;
	lvl.LoadFromFile("Level_megaman.tmx");

	Texture tileSet, moveplatform, megaman, fang, tEnemy;

	tileSet.loadFromFile("tile.png");
	//moveplatform.loadFromFile("files/movingPlatform.png");
	megaman.loadFromFile("textures/ninja.png");
	//fang.loadFromFile("files/fanq.png");
	tEnemy.loadFromFile("textures/sheet-enemies.png");

	AnimationManager anim3;
	anim3.loadFromXML("enemy_anim.xml",tEnemy);

	AnimationManager anim;
	anim.loadFromXML("animation.xml",megaman);
	anim.animList["jump"].loop = 0;

	AnimationManager anim2;
	anim2.loadFromXML("shoot.xml",tEnemy);

	offsetX = 0;

	/*
	AnimationManager anim4;
	anim4.create("move",moveplatform,0,0,95,22,1,0);
	*/

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;

	std::vector<Object> ee = lvl.GetObjects("enemy");
	for (int i=0;i < ee.size();i++)
		entities.push_back(new ENEMY(anim3, lvl, ee[i].rect.left, ee[i].rect.top) );
	/*
	e = lvl.GetObjects("MovingPlatform");
	for (int i=0;i < e.size();i++)
	entities.push_back(new MovingPlatform(anim4, lvl, e[i].rect.left, e[i].rect.top) );
	*/
	Object pl = lvl.GetObject("player");
	PLAYER Mario(anim, lvl, pl.rect.left, pl.rect.top);
	PlayerScores plScores;
	int Scores = 0;

	Clock clock;

	/////////////////// основной цикл  /////////////////////
	while ((window.isOpen())&&(Mario.Health>0))
	{ 
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time/500;  // здесь регулируем скорость игры

		if (time > 40) time = 40; 

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)      
				window.close();

			if (event.type == Event::KeyPressed)
				if (event.key.code==Keyboard::Space)
					entities.push_back(new Bullet(anim2,lvl,Mario.x+18,Mario.y+18,Mario.dir) );	
		}


		if (Keyboard::isKeyPressed(Keyboard::A)) Mario.key["L"]=true;
		if (Keyboard::isKeyPressed(Keyboard::D)) Mario.key["R"]=true;
		if (Keyboard::isKeyPressed(Keyboard::W)) Mario.key["Up"]=true;
		if (Keyboard::isKeyPressed(Keyboard::S)) Mario.key["Down"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Space)) Mario.key["Space"]=true;


		for(it=entities.begin();it!=entities.end();)
		{
			Entity *b = *it; 
			b->update(time);
			if (b->life==false)	{ 
				if (b->Name == "Enemy") {
					Scores+=10; 
					std::cout << "kill " ;
				}
				it  = entities.erase(it); delete b;}
			else it++;
		}
		//std::cout<< entities.size() << std::endl;

		offsetX += 0.03*time;
		//std::cout << offsetX << " " << Mario.x << std::endl;

		Mario.update(time);

		plScores.update(Mario.Health,std::to_string(Scores));
		/*
		if (Mario.x < 300) offsetX = Mario.x - 300;
		if (Mario.x > 1200) offsetX = Mario.x - 1200;
		*/
		if (Mario.y < 230) offsetY = Mario.y - 230;
		if (Mario.y > 750) offsetY = Mario.y - 750;


		for(it=entities.begin();it!=entities.end();it++)
		{   
			//1. враги
			if ((*it)->Name=="Enemy")
			{
				Entity *enemy = *it;

				if (enemy->Health<=0) {

					continue; 
				}
				if  (Mario.getRect().intersects( enemy->getRect() ))
					if (Mario.dy>0) { enemy->dx=0; Mario.dy=-0.2; enemy->Health=0;}
					else if (!Mario.hit) { Mario.Health-=5; Mario.hit=true;
					if (Mario.dir) Mario.x+=10; else Mario.x-=10;}


					for (std::list<Entity*>::iterator it2=entities.begin(); it2!=entities.end(); it2++)
					{	
						Entity *bullet = *it2;
						if (bullet->Name=="Bullet") 
							if ( bullet->Health>0)
								if  (bullet->getRect().intersects( enemy->getRect() ) ) { 
									bullet->Health=0; 
									enemy->Health-=5; 
								}
					}
			}

		}

		/////////////////////отображаем на экран/////////////////////

		view.setCenter(/* Mario.x - */offsetX ,Mario.y - offsetY);
		window.setView(view);

		window.clear(Color(107,140,255));

		lvl.Draw(window);

		for(it=entities.begin();it!=entities.end();it++)
			(*it)->draw(window);

		Mario.draw(window);
		plScores.draw(window);

		window.display();
	}

	window.close();

	std::cout << std::endl;
	std::cout << "   0     000  0   0 " <<std::endl;
	std::cout << "  0 0   0   0  0 0  " <<std::endl;
	std::cout << " 0   0  0   0   0   " <<std::endl;
	std::cout << " 0   0  0   0  0 0  " <<std::endl;
	std::cout << " 0   0   000  0   0 " <<std::endl;

	system("pause");
	return 0;
}
