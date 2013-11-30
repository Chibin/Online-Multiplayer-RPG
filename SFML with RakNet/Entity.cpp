#ifndef __ENTITY__
#define __ENTITY__
#include <string>
#include <iostream>
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
	virtual void draw() = 0;

};

#endif