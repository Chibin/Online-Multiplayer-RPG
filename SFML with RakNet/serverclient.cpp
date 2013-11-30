#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include <stdio.h>
#include "packetManager.cpp"
#include "Player.cpp"
#include "ServerDataContainer.cpp"
#include <vector>
#include <iostream>
#include <string>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#define MAX_CLIENTS 10
#define SERVER_PORT 60000
using namespace RakNet;

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
	Player* player;
	ServerDataContainer* serverData; //Should only 1 that exist
	void serverclientInit(){
		printf("(C) or (S)erver?\n");
		gets(str);
		if ((str[0]=='c')||(str[0]=='C'))
		{
			sd = new RakNet::SocketDescriptor();
			peer->Startup(1,*&sd, 1);
			isServer = false;
			//=========
			//player setup
			//=========
			player = new Player();
			player->playerInit();
		} else {
			sd = new RakNet::SocketDescriptor(SERVER_PORT,0);
			peer->Startup(MAX_CLIENTS, *&sd, 1);
			isServer = true;
		}
		//---------------------------------------
		//Packet Handling classes inited
		//---------------------------------------
		packet_manager.init(peer,isServer,player);
		serverData = new ServerDataContainer();
		if(serverData != NULL) packet_manager.setServerData(serverData);
		else std::cout << "SERVER DATA IS NULL" << std::endl;
	}
public:
	serverclient(){
		peer = RakNet::RakPeerInterface::GetInstance();
		serverclientInit();
	}
	void setChatlog(){
		if(chatlog != NULL && (!isServer))
			packet_manager.getClientPacketHandler()->setChatlog(chatlog);
	}
	void serverclientConnectionStart(){
			if (isServer)
			{
				printf("Starting the server.\n");
				peer->SetMaximumIncomingConnections(MAX_CLIENTS);
			} else {
				printf("Enter server IP or hit enter for 127.0.0.1\n");
				gets(str);
				if (str[0]==0){
					strcpy(str, "127.0.0.1");
				}
				printf("Starting the client.\n");
				peer->Connect(str, SERVER_PORT, 0,0);
			}
		}
		bool getIsServer(){ return isServer;}

	void drawManager(sf::RenderWindow &window){
		//std::cout << player->getName() << std::endl;
	}

	void packetManager(){
			RakNet::Packet *packet = NULL;
			packet = peer->Receive();
			//Packet Events
			int packetCounter = 0;
			while( packet != NULL ) {
				packet_manager.manager(packet,isServer);
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