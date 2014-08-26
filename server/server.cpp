#ifndef __SERVERCLIENT__
#define __SERVERCLIENT__
#include "server.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include <stdio.h>
#include "packetManager.cpp"
#include "ServerDataContainer.cpp"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000
using namespace RakNet;
char keypressToChar(sf::Keyboard::Key keypressed, bool isShiftPressed=false);
class serverclient{
public:
	RakNet::RakPeerInterface *peer;
	std::vector<std::string> *chatlog;
protected:
	enum GameMessages
	{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM+1
	};

private:
	char str[512];
	bool isServer;
	RakNet::Packet *packet;
	RakNet::SocketDescriptor* sd;
	packetManager packet_manager;
	std::map<std::string, Player> otherPlayers;
	ServerDataContainer* serverData; //Should only 1 that exist
	void serverInit(){
		sd = new RakNet::SocketDescriptor(SERVER_PORT,0);
		peer->Startup(MAX_CLIENTS, *&sd, 1);
		
		//---------------------------------------
		//Packet Handling init
		//---------------------------------------
		packet_manager.init(peer);
	    serverData = new ServerDataContainer();
		if(serverData != NULL) packet_manager.setServerData(serverData);
		else std::cout << "SERVER DATA IS NULL" << std::endl;
		printf("I GET HERE\n");
	}
public:
	serverclient(){
		packet = NULL;
		sd = NULL;
		serverData = NULL;
		peer = RakNet::RakPeerInterface::GetInstance();
		serverInit();
	}
	char keypressToChar(sf::Keyboard::Key keypressed, bool isShiftPressed=false){
	}
	void setChatlog(){
	}
	void serverConnectionStart(){
		printf("serverConnectionStart - starting the server.\n");
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	bool getIsServer(){ return isServer;}

	void packetManager(){
			RakNet::Packet *packet = NULL;
			packet = peer->Receive();
			//Packet Events
			int packetCounter = 0;
			while( packet != NULL ) {
				packet_manager.manager(packet);
				if(packet->data[0] == ID_CONNECTION_ATTEMPT_FAILED) {				
					printf("Trying to reconnect.\n");
					peer->Connect(str, SERVER_PORT, 0,0);
				}
				peer->DeallocatePacket(packet);
				packet = peer->Receive();
				packetCounter++;
			}
	}
};
#endif
