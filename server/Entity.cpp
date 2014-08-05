#ifndef __ENTITY__
#define __ENTITY__
#include <string>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
class Entity{
//========
//Abstract class
//========
private:
	//----------------------------------
	//Stats and other conditions
	//----------------------------------
protected:
	//----------
public:
	Entity(){}
	virtual void setName(std::string _name) = 0;
	virtual std::string getName() = 0;
	virtual void draw(sf::RenderWindow &window) = 0;

};

#endif