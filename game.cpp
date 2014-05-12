#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

#include "TinyXML\tinyxml.h"

#include "Animation.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Level.hpp"

using namespace sf; 
using namespace std;


int main()
{
	RenderWindow window(sf::VideoMode(tile_size*W/2, tile_size*H), "SFML Application");

	View view( FloatRect(0, 0, tile_size*W/2, tile_size*H) );
	
	Level lvl;
	lvl.LoadFromFile("Level_megaman.tmx");

	Texture t,pp;
	Texture e;
	Texture b;
	t.loadFromFile("textures/explosion.png");
	e.loadFromFile("textures/ninja.png");
	b.loadFromFile("textures/sur.png");
	

	AnimationManager anim;
	//(name, texture, x, y, w, h, count, speed, step;)
	/*anim.Create("stay", e, 0, 0, 50, 70, 1, 0.005, 50);
	anim.Create("walk", e, 0, 70, 65, 70, 8, 0.005, 65);
	anim.Create("shoot", e, 0, 155, 80, 70, 6, 0.005, 80);
	anim.Create("jump", e, 85, 455, 55, 70, 3, 0.0013, 55);
	//.Create("shootAndWalk", e, 0, 70, 65, 70, 8, 0.005, 65);
	anim.Create("duck", e, 350,269, 60, 50, 2, 0.004, 60);
	*/
	AnimationManager anim1;
	anim1.loadFromXML("animation.xml",e);
	anim1.animList["jump"].loop = 0;

	AnimationManager anim2;
	anim2.Create("move", b, 0, 130, 91, 70, 5, 0.005, 91);
	anim2.Create("explode", t, 0, 0, 78, 90, 8, 0.005, 78);

	std::list<Entity*> entities;
	std::list<Entity*>::iterator it;

	

	Player p(anim1, lvl, 100, 100);
	/*vector<Object> ti = lvl.GetObjects("solid");
	for (int i=0; i<ti.size(); i++)
		ti[i].rect.top + lvl.GetTileSize().y / 2 * (ti[i].rect.height / lvl.GetTileSize().y - 1));
		*/

	//entities.push_back(new Enemy(anim2, 100, 200));
	//entities.push_back(new Enemy(anim2, 600, 220));
	//entities.push_back(new Enemy(anim2, 1000, 120));

	/*std::vector<Object> ee = lvl.GetObjects("enemy");
	for (int i=0;i < ee.size();i++)
		entities.push_back(new Enemy(anim2, lvl, ee[i].rect.left, ee[i].rect.top) );
	/*
	ee = lvl.GetObjects("MovingPlatform");
	for (int i=0;i < ee.size();i++)
		entities.push_back(new MovingPlatform(anim4, lvl, e[i].rect.left, e[i].rect.top) );
		*/
	//Enemy enemy(anim2, 100, 90, false);
	//enemy.SetEnemy(e,200,100);

	Clock clock;

	RectangleShape rectangle;
	rectangle.setSize(Vector2f(tile_size,tile_size));

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 500;

		if (time>20) time-=20;
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code==Keyboard::Space) {
					entities.push_back(new Bullet(anim2, lvl, p.x-offsetX, p.y-offsetY, p.dir));
				}
		}

		anim1.Set("stay");

		if (Keyboard::isKeyPressed(Keyboard::A)) p.key["L"]=true;
		if (Keyboard::isKeyPressed(Keyboard::D)) p.key["R"]=true;
		if (Keyboard::isKeyPressed(Keyboard::W)) p.key["Up"]=true;
		if (Keyboard::isKeyPressed(Keyboard::S)) p.key["Down"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			p.key["Space"]=true;
		}

		for (it=entities.begin(); it!=entities.end();){
			Entity *b = dynamic_cast<Entity*>(*it);
			if (b->life == false) {
				it = entities.erase(it);
				delete b;
			}
			else it++;
		}

		for (it = entities.begin(); it != entities.end(); it++)
			(*it)->update(time);

		p.update(time);

		//enemy.update(time);

		if ((p.x > 300)&&(p.x < 1300)) offsetX = p.x - 300;
		
		window.clear(Color::White);
		
		//cout << "=";
		for (it=entities.begin(); it!=entities.end(); it++) {

			if ((*it)->Name=="Enemy") {
				Entity *enemy = *it;
				if (enemy->health <= 0){
					enemy->life = false;

					enemy->anim.animList[enemy->anim.currentAnim].sprite.setColor(Color::Red);
					continue;
				}

				//cout << enemy->getRect().left << " " << enemy->getRect().top << " " << enemy->getRect().width << " " << enemy->getRect().height << endl; 
				//cout << "=";
				FloatRect R =  enemy->getRect();
				R.left += offsetX;
				R.top += offsetY;
				if (p.getRect().intersects(R))
					if (p.dy>0) {
						enemy->dx=0;
						p.dy=-0.2;
						enemy->health=0;
					}
					else 
						if (!p.hit) {
							p.health-=5;
							p.hit=true;
							cout << "hit";
							p.anim.animList[p.anim.currentAnim].sprite.setColor(Color::Red);
						}
						for (std::list<Entity*>::iterator it2=entities.begin(); it2!=entities.end(); it2++){
							Entity *bullet = *it2;
							if (bullet->Name=="Bullet")
								if (bullet->health>0) {

									if (bullet->getRect().intersects(enemy->getRect() )){
										bullet->health-=5;
										enemy->health-=5;
										anim2.Set("explode");
										if (bullet->health==0) bullet->life = false;
									}
								}
						}
			}
		}



		////////////////////////////	
		//отрисовка
		view.setCenter( p.x,p.y);
		window.setView(view);
		window.clear(Color(107,140,255));

		lvl.Draw(window);

		for (it =entities.begin(); it!=entities.end(); it++)
			(*it)->draw(window);

		//enemy.anim.Draw(window,enemy.x - offsetX,enemy.y-offsetY);
		p.anim.Draw(window,p.x-offsetX,p.y+p.h-offsetY);


		window.display();
	}
	system("pause");
	return 0;
}
