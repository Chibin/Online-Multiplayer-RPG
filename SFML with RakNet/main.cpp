#include <stdio.h>
#include <string.h>
#include "serverclient.cpp"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" //MessageID
#include <time.h>
#include <iostream>
#include <vector>
#include "packetHandler.cpp"
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
	int x,y;
	x = y = 0;
	std::vector<std::string> chatLog;
	serverclient *user = new serverclient();
	RakNet::RakPeerInterface *peer = user->peer;
	bool isServer = user->getIsServer();
	RakNet::Packet *packet;
	user->serverclientConnectionStart();
	user->chatlog = &chatLog;
	user->setChatlog();
	sf::RenderWindow window(sf::VideoMode(800, 600,32), "SFML window",1);
	if (isServer) window.close();
	sf::Clock clock;
	sf::Time time;
	 // Create a graphical text to display
	sf::Font font;
	if (!font.loadFromFile("Book Antiqua.ttf"))
		return EXIT_FAILURE;
	sf::Text chatText("",font,25);
	sf::Text sentText("",font,25);
	sf::Text chatCheck("Chat: off",font,17);
	chatCheck.setPosition(0,550);
	chatText.setPosition(0,565);
	sf::Texture texture;
	if (!texture.loadFromFile("Images/Lloyd.png"))
	{
		std::cout << "cannot load the image" << std::endl;
	}
	sf::Sprite player(texture);
	player.setScale(2.0f,2.0f);
	float a = 0;
	bool repeat = true;
	bool isShiftDown = false;
	bool isChatting = false;
	
	window.setFramerateLimit(30);
	while (window.isOpen() || isServer)
	{
		user->packetManager();
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			user->inputHandler(event, window);
			if (event.type == sf::Event::KeyPressed){
				window.setKeyRepeatEnabled(repeat);
				if (event.type == sf::Event::Closed) window.close();
			}
		}
		//------------
		// Client stuff for drawing
		//------------
		if(!isServer){
			window.clear();
			user->drawManager(window);
			window.draw(chatCheck);
			if(chatLog.size() > 0)
				sentText.setString(chatLog[chatLog.size()-1]);
			window.draw(sentText);
			window.draw(chatText);
	//		sf::IntRect test(0+24*x,0+32*y,24,32);
		//	if( clock.getElapsedTime().asMilliseconds() > 450){
		//		clock.restart();
		//		x = x < 2? x+1:0;
	//		}
	//		player.setTextureRect(test);
			//window.draw(player);
			window.display();
		}
	}
	//------------------------------
	RakNet::RakPeerInterface::DestroyInstance(peer);
	return 0;
}