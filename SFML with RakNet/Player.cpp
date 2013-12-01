#ifndef __PLAYER__
#define __PLAYER__

#include "Entity.cpp"
#include <string>
#include <iostream>
class Player : public Entity{
private:
	std::string spritePath;
	std::string name;
	struct position{
		float x,y;
	};
	position currentPos;

	sf::Clock clock;
	sf::Texture texture;
	sf::Sprite playerSprite;

public:
	Player(){
		spritePath = "Images/Lloyd.png";
	}
	void playerInit(std::string playerName=""){
			if(playerName == ""){
				std::cout << "Enter character name: ";
				std::cin >> playerName;
			}
			name = playerName;
	}
	std::string getSpritePath(){
		return spritePath;
	}
	void setName(std::string _name) { name = _name;}
	std::string getName() { std::cout << "returning the name: " << name << std::endl; return name;}
	void draw(sf::RenderWindow &window){
		int x = 1,y = 0;
			sf::IntRect test(0+24*x,0+32*y,24,32);
			if( clock.getElapsedTime().asMilliseconds() > 450){
				clock.restart();
				x = x < 2? x+1:0;
			}
			//playerSprite.setTextureRect(test);
	}
	position getPosition(){
		return currentPos;
	}
	void setPosition(float _x, float _y){
		currentPos.x = _x;
		currentPos.y = _y;
	}

};
#endif