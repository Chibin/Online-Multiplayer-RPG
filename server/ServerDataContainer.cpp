#ifndef __SERVERDATACONTAINER__
#define __SERVERDATACONTAINER__
#include "Player.cpp"
#include <iostream>
#include <map>
class ServerDataContainer{
	//===================================
	//Where the data should be kept
	//===================================
private:
	//Key, value
	std::map<std::string, Player> listOfPlayers;

public:
	ServerDataContainer(){
	}

	Player* getPlayer(std::string username){
		return &listOfPlayers[username];
	}
	void addNewPlayer(Player playerToAdd){
		std::cout << listOfPlayers.size() << std::endl;
		listOfPlayers.insert( std::pair<std::string, Player>(playerToAdd.getName(),playerToAdd));
		std::cout << listOfPlayers.size() << std::endl;
		std::cout <<  "Accessing storage to get the name: " <<listOfPlayers[playerToAdd.getName()].getName() << std::endl;
	}
};

#endif