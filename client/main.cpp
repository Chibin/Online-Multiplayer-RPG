#include <stdio.h>
#include <string.h>
#include "src/client.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" //MessageID
#include <time.h>
#include <iostream>
#include <vector>
#include "src/packetHandler.cpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#define MAX_CLIENTS 10
#define SERVER_PORT 60000
using namespace RakNet;

int main()
{
	//------------------
	//controls
	//------------------
	bool isUp,isDown,isLeft,isRight;
	isUp = isDown = isLeft = isRight = false;
	//------------------
	std::vector<std::string> chatLog;
	client *user = new client();
	RakNet::RakPeerInterface *peer = user->peer;
	bool isServer = user->getIsServer();
	RakNet::Packet *packet;
	user->clientConnectionStart();
	user->chatlog = &chatLog;
	user->setChatlog();
	sf::RenderWindow window(sf::VideoMode(800, 600,32), "SFML window",1);
	if (isServer) window.close();
	 // Create a graphical text to display
	float a = 0;
	bool repeat = true;
	
	window.setFramerateLimit(30);
	while (window.isOpen() || isServer)
	{
		user->packetManager();
		sf::Event event;
		while (window.pollEvent(event)) user->inputHandler(event, window);
		user->requestsToServer();
		//------------
		// Client stuff for drawing
		//------------
		if(!isServer){
			window.clear();
			user->drawManager(window);
			window.display();
		}
	}
	//------------------------------
	RakNet::RakPeerInterface::DestroyInstance(peer);
	return 0;
}
