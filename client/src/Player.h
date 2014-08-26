#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

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
	//Determines where the player is facing.
	/*
	 * North = 0; South = 2; East = 1; West = 3;
	 */
	int player_direction;  //Determines where the player is facing.

public:
	Player();
	
	void playerInit(std::string playerName="");
	
	void setSpritePath(std::string _newPath);
	
	std::string getSpritePath();
	
	void setName(std::string _name);

	std::string getName();

	void draw(sf::RenderWindow &window);
	
	position getPosition();
	
	void setDireciton(int _player_direction);
	
	int getDirection();
	
	void setPosition(float _x, float _y);
	
	void move(float _x, float _y);
	
	void loadTexture(std::string _spritePath);
	
};

#endif
