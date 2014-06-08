#ifndef PS_H
#define PS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

#include "TinyXML\tinyxml.h"

using namespace sf;

class PlayerScores
{
public:
	Texture t;
	Sprite s;
	Font f;
	Text text;
	int max;
	RectangleShape bar;

	PlayerScores()
	{
		f.loadFromFile("sansation.ttf");
		text.setString("hello");
		text.setFont(f);
		text.setCharacterSize(20);
		//text.setPosition(70,10);
		text.setStyle(sf::Text::Bold);
		text.setColor(sf::Color::Black);
		
		t.loadFromFile("textures/megaman_bullets.png");
		s.setTexture(t);
		s.setTextureRect( IntRect(783,2,15,84) );

		bar.setFillColor(Color(0,0,0));
		max = 100;
	}

	void update(int k, std::string sc)

	{
		if (k>0) 
			if (k<max)
        		bar.setSize( Vector2f(10, (max-k)*70/max));
	
		text.setString(String(sc));
	}

   	void draw(RenderWindow &window)
	{  
		Vector2f center = window.getView().getCenter();
		Vector2f size = window.getView().getSize();

		s.setPosition(   center.x - size.x/2 + 10, center.y - size.y/2 + 10);
	    bar.setPosition( center.x - size.x/2 + 14, center.y - size.y/2 + 14);
	    text.setPosition( center.x - size.x/2 + 34, center.y - size.y/2 + 10);
		
		window.draw(text);
		window.draw(s);
		window.draw(bar);
	}

};

#endif PS_H