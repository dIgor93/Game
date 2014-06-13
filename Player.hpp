#ifndef PLAY_H
#define PLAY_H

#include "Globals.hpp"
#include "Entity.hpp"
#include "Level.hpp"

class PLAYER: public Entity {
public:
	enum { stay, walk, duck, 
		jump, climb, jumping_attack,
		climbing_attack, walking_attack, ducking_attack
	} STATE;
	bool onLadder, shoot, hit;
	std::map<std::string,bool> key;

	PLAYER(AnimationManager &a, Level &lev,int x,int y):Entity(a,x,y){	
		option("Player",0,100,"stay");
		STATE = stay;
		hit = 0;
		obj = lev.GetAllObjects();
	}

	void KeyCheck(){
		if(key["L"]){
			dir=1;  
			if (STATE != duck) 
				dx = -0.3; 
			if (STATE == stay) 
				STATE = walk;
			if (shoot) 
				STATE = walking_attack;
		}

		if(key["R"]){ 		    
			dir = 0; 	
			if (STATE != duck) 
				dx = 0.3;
			if (STATE == stay) 
				STATE = walk;  
			if (shoot) 
				STATE = walking_attack;
		}

		if (key["Up"]){
			if (onLadder) 
				STATE = climb;
			if (STATE == stay || STATE == walk){
				dy = -0.37; 
				STATE = jump;
				anim.play("jump");
			} 
			if (STATE == climb)
				dy = -0.05; 
			if (shoot) 
				STATE = jumping_attack;
		}

		if (key["Down"]){ 
			if (STATE == stay || STATE == walk){ 
				STATE = duck; 
				dx = 0;
			} 
			if (STATE == climb) 
				dy = 0.05;    
			if (shoot) 
				STATE = ducking_attack;
		}

		if (key["Space"]){
			shoot = true;
		}

		/////////////////////если клавиша отпущена///////////////////////////
		if (!(key["R"] || key["L"])){
			dx = 0; 
			if (STATE == walk) 
				STATE = stay;
		}

		if (!(key["Up"] || key["Down"])){
			if (STATE == climb)
				dy = 0;  
		}

		if (!key["Down"]){
			if (STATE == duck){ 
				STATE = stay;
			}	
		}

		if (!key["Space"]){
			shoot = false;
		}
	}

	void update(float time){	
		KeyCheck();
	switch(STATE){
		case stay:
			anim.set("stay");
			break;
		case walk:
			anim.set("walk");
			break;
		case jump:
			anim.set("jump");
			break;
		case duck:
			anim.set("duck");
			break;
		case climb:
			anim.set("climb"); 
			anim.pause(); 
			if(dy != 0) 
				anim.play(); 
			if(!onLadder) 
				STATE = stay;
			break;
	}
		/*if (STATE == stay) anim.set("stay");
		if (STATE==walk) anim.set("walk");
		if (STATE==jump) anim.set("jump");
		if (STATE==duck) anim.set("duck");
		if (STATE==climb) {anim.set("climb"); anim.pause(); if (dy!=0) anim.play(); if (!onLadder) STATE=stay;} 
		*/
		if (shoot){
			switch(STATE){
				case stay:
					anim.set("shoot");
					break;
				case walk:
					anim.set("walking_attack");  
					break;
				case jump:
					anim.set("jumping_attack");
					break;
				case duck:
					anim.set("ducking_attack");
					break;
				case climb:
					anim.set("climbing_attack"); 
					break;
			}
			/*if (STATE==jump) anim.set("jumping_attack"); 
			if (STATE==duck) anim.set("ducking_attack"); 
			if (STATE==climb) anim.set("climbing_attack"); 
			if (STATE==walk) anim.set("walking_attack");  
			if (STATE==stay) anim.set("shoot");
			*/
		}   

		if(hit){ 
			timer += time; 
			if(timer > 1000){
				hit = false; 
				timer = 0;
			}
			//anim.set("hit");
		}

		if(dir)
			anim.flip();
		
		if((offsetX - x) < -260){ // если пытаемся выбежать вперед за правую границу экрана - нас отбрасывает назад
			x = offsetX + 260;
		}
		if((offsetX - x) > 300){ //если тупим и не успеваем, то нас выталкивает вперед и придает скорости
			x = offsetX - 300;
			dx = 0.3;
		}
		x += dx * time;	
		Collision(0);  
		
		//мы оказались за пределом экрана до этого и теперь dx>0, а так же Collision отбросила нас обратно за пределы
		if (((offsetX - x) > 300) && (dx > 0)){ // если после всего мы по прежнему за пределом экрана, то нас "прищемляет"
			Health -= 50;
			//screen_speed = 0;
			x = offsetX - 300;
		}

		if (STATE != climb)
			dy += 0.0005*time;
		y += dy*time; 
		onLadder = false; 
		Collision(1);


		anim.tick(time);

		key["R"] = key["L"] = key["Up"] = key["Down"] = key["Space"] = false;
	}

	void Collision(int num){	
		for (int i = 0; i < obj.size(); i++){
			if (getRect().intersects(obj[i].rect)){
				if (obj[i].name=="solid"){           
					if (dy > 0 && num == 1){
						y = obj[i].rect.top - h;
						dy = 0;
						STATE = stay;
					}
					if (dy < 0 && num == 1){
						y = obj[i].rect.top + obj[i].rect.height;
						dy = 0;
					}
					if (dx > 0 && num == 0){
						x = obj[i].rect.left - w;
					}
					if (dx < 0 && num == 0){
						x = obj[i].rect.left + obj[i].rect.width;
					}
				} 
				if (obj[i].name=="ladder"){ 
					onLadder=true; 
				}
				if (obj[i].name=="SlopeLeft"){
					FloatRect r = obj[i].rect;
					int y0 = (x + w/2 - r.left) * r.height/r.width + r.top - h;
					if (y > y0)
						if (x+w/2>r.left){
							y =  y0; 
							dy = 0;
							STATE = stay;
						}
				}
				if (obj[i].name=="SlopeRight"){
					FloatRect r = obj[i].rect;
					int y0 = -(x + w/2 - r.left)*r.height/r.width + r.top + r.height - h;
					if (y > y0)
						if (x + w/2 < r.left + r.width){
							y = y0; 
							dy = 0; 
							STATE = stay;
						}
				}
			}
		}
	}
};

#endif PLAY_H