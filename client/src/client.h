#ifndef __CLIENT__
#define __CLIENT__
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "RakPeerInterface.h"
#include "packetManager.cpp"
#include "Player.cpp"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

class client{
	private:
		char str[512];
		bool isServer;
		RakNet::Packet *packet;
		RakNet::SocketDescriptor* sd;
		packetManager packet_manager;
		Player* player;
		std::map<std::string, Player> otherPlayers;
		ServerDataContainer* serverData; //Should only 1 that exist
		bool isUp,isDown,isLeft,isRight, repeat, isShiftDown, isChatting;
		sf::Font font;
		sf::Text chatText;
		sf::Text sentText;
		sf::Text chatCheck;

	protected:
		enum GameMessages
		{
			ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM+1
		};
	
	public:
		RakNet::RakPeerInterface *peer;
		std::vector<std::string> *chatlog;

		void clientInit();
		client();
		char keypressToChar(sf::Keyboard::Key keypressed, bool isShiftPressed);
		void setChatlog();
		void clientConnectionStart();
		bool getIsServer();
		void drawManager(sf::RenderWindow &window);
		void requestsToServer();
		void inputHandler(sf::Event &event, sf::RenderWindow &window);
		void packetManager();

};

#endif
