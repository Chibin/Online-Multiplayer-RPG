#ifndef __PLAYER__
#define __PLAYER__

#include "Player.h"

Player::Player(){
	spritePath = "Images/Lloyd.png";
	setPosition(50.0,50.0);
	currentFrame = 0;
	player_direction = 2;
}

void Player::playerInit(std::string playerName){
		if(playerName == ""){
			std::cout << "Enter character name: ";
			std::cin >> playerName;
		}
		name = playerName;
}

void Player::setSpritePath(std::string _newPath){
	spritePath = _newPath;
}

std::string Player::getSpritePath(){
	return spritePath;
}

void Player::setName(std::string _name) { name = _name;}

std::string Player::getName() { std::cout << "returning the name: " << name << std::endl; return name;}

void Player::draw(sf::RenderWindow &window){
		playerSprite.setPosition(currentPos.x,currentPos.y);
		spriteSheet.loadFromFile(spritePath);
		sf::IntRect test(0+24*currentFrame,0+32*player_direction,24,32);
		if( clock.getElapsedTime().asMilliseconds() > 450){
			clock.restart();
			currentFrame = currentFrame < 2? currentFrame+1:0;
		}
		playerSprite.setTextureRect(test);
		window.draw(playerSprite);
}

Player::position Player::getPosition(){
	return currentPos;
}

void Player::setDireciton(int _player_direction){
	player_direction = _player_direction;
}

int Player::getDirection(){
	return player_direction;
}

void Player::setPosition(float _x, float _y){
	currentPos.x = _x;
	currentPos.y = _y;
}

void Player::move(float _x, float _y){
	currentPos.x += _x;
	currentPos.y += _y;
}

void Player::loadTexture(std::string _spritePath=""){
	if (_spritePath != "")
		spritePath = _spritePath;
	if (!spriteSheet.loadFromFile(spritePath)){
		std::cout << "cannot load the image" << std::endl;
	}
	playerSprite.setTexture(spriteSheet);
}

#endif
