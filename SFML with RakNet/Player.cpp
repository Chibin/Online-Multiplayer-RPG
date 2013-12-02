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

public:
	Player(){
		spritePath = "Images/Lloyd.png";
		setPosition(50.0,50.0);
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
			playerSprite.setPosition(currentPos.x,currentPos.y);
			printf("%f %f \n",currentPos.x,currentPos.y);
			spriteSheet.loadFromFile(spritePath);
			playerSprite.setTexture(spriteSheet);
			window.draw(playerSprite);
	}
	position getPosition(){
		return currentPos;
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