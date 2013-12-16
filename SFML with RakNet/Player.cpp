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
	sf::Texture spriteSheet;
	sf::Sprite playerSprite;
	int currentFrame;

public:
	Player(){
		spritePath = "Images/Lloyd.png";
		setPosition(50.0,50.0);
		currentFrame = 0;
	}
	void playerInit(std::string playerName=""){
			if(playerName == ""){
				std::cout << "Enter character name: ";
				std::cin >> playerName;
			}
			name = playerName;
	}
	void setSpritePath(std::string _newPath){
		spritePath = _newPath;
	}
	std::string getSpritePath(){
		return spritePath;
	}
	void setName(std::string _name) { name = _name;}
	std::string getName() { std::cout << "returning the name: " << name << std::endl; return name;}
	void draw(sf::RenderWindow &window){
			playerSprite.setPosition(currentPos.x,currentPos.y);
			spriteSheet.loadFromFile(spritePath);
			sf::IntRect test(0+24*currentFrame,0+32*0,24,32);
			if( clock.getElapsedTime().asMilliseconds() > 450){
				clock.restart();
				currentFrame = currentFrame < 2? currentFrame+1:0;
			}
			playerSprite.setTextureRect(test);
			window.draw(playerSprite);
	}
	position getPosition(){
		return currentPos;
	}
	int getDirection(){
		return currentFrame;
	}
	void setPosition(float _x, float _y){
		currentPos.x = _x;
		currentPos.y = _y;
	}
	void move(float _x, float _y){
		currentPos.x += _x;
		currentPos.y += _y;
	}
	void loadTexture(std::string _spritePath=""){
		if (_spritePath != "")
			spritePath = _spritePath;
		if (!spriteSheet.loadFromFile(spritePath)){
			std::cout << "cannot load the image" << std::endl;
		}
		playerSprite.setTexture(spriteSheet);
	}
};
#endif