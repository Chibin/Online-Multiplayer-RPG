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
	void draw(){}
	position getPosition(){
		return currentPos;
	}
	void setPosition(float _x, float _y){
		currentPos.x = _x;
		currentPos.y = _y;
	}

};
#endif