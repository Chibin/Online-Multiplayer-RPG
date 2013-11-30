#include <stdio.h>
#include <string.h>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" //MessageID
#include <time.h>
#include "serverclient.cpp"
#include <iostream>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#define MAX_CLIENTS 10
#define SERVER_PORT 60000
using namespace RakNet;
char keypressToChar(sf::Keyboard::Key keypressed, bool isShiftPressed=false){
	if( keypressed >= 0 && keypressed <=25)
		return isShiftPressed?(char)(keypressed+65):(char)(keypressed+97);
	else if ( keypressed >= 26 && keypressed <= 35)
		if (isShiftPressed){
			switch (keypressed){
				case 27: return '!';
				case 28: return '@';
				case 29: return '#';
				case 30: return '$';
				case 31: return '%';
				case 32: return '^';
				case 33: return '&';
				case 34: return '*';
				case 35: return '(';
				case 26: return ')';
			}
		}
		else
			return (char)(keypressed+22);
	else if (keypressed == sf::Keyboard::Add)
		return '+';
	else if ((keypressed == sf::Keyboard::Subtract) || (keypressed == 56))
		return '-';
	else if (keypressed == sf::Keyboard::Space)
		return ' ';
	else if ( keypressed == sf::Keyboard::Period)
		return isShiftPressed? '>': '.'; 
	else if ( keypressed == 51 )
		return isShiftPressed? '"': '\'';  
	else if ( keypressed == 48 )
		return isShiftPressed? ':': ';';
	else{
		std::cout << keypressed << std::endl;
		return keypressed;
	}
}
	enum GameMessages{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM+1
	};
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
		//testing
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::KeyReleased){
				if (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift)	isShiftDown = false;
				if(event.key.code == sf::Keyboard::Up)	isUp = false;
				if(event.key.code == sf::Keyboard::Down)	isDown = false;
				if(event.key.code == sf::Keyboard::Left)	isLeft = false;
				if(event.key.code == sf::Keyboard::Right)	isRight = false;
			}

			if (event.type == sf::Event::KeyPressed){
				window.setKeyRepeatEnabled(repeat);
				if (event.key.code == sf::Keyboard::Return){
					window.setKeyRepeatEnabled(!repeat);
					isChatting = !isChatting;
					if (isChatting){ chatCheck.setString("Chat: on"); 	}
					else{
						chatCheck.setString("Chat: off");
						sentText.setString(chatText.getString());
						// Sending the message in Broadcast to all the clients connected
						BitStream bsOut;
						bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
						bsOut.Write(chatText.getString().toAnsiString().c_str());
						peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);	
						chatText.setString(std::string(""));
					}
				}
				else if (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift){  isShiftDown = true;	}
				else if (event.key.code == sf::Keyboard::BackSpace){
					std::string newChatText = chatText.getString();
					if (newChatText.size() > 0) newChatText.resize(newChatText.size()-1);
					chatText.setString(newChatText);
				}
				else if (event.key.code == sf::Keyboard::Escape){ window.close(); }
				else if (isChatting){
					std::string newChatText = chatText.getString();
					newChatText += keypressToChar(event.key.code, isShiftDown);
					chatText.setString(newChatText);
				}
				else if(event.key.code == sf::Keyboard::Up){	 y = 0;	isUp = true;	}
				else if(event.key.code == sf::Keyboard::Down){ y = 2;	isDown = true;}
				else if(event.key.code == sf::Keyboard::Left){ isLeft = true; y = 3;	}
				else if(event.key.code == sf::Keyboard::Right){ isRight = true;  y = 1; }
				std::cout << event.type << " " <<keypressToChar(event.key.code) << std::endl;
			}
			if (event.type == sf::Event::Closed) window.close();
		}

		//-----------
		//character calculations
		//------------
		//these should be commands that is sent to the server.
		//Request to move the player. 
		if(isUp) {player.move(0,-3);}
		if(isDown){player.move(0,3);}
		if(isLeft) {player.move(-3,0);}
		if(isRight) {player.move(3,0);} 

		//------------
		// Client stuff for drawing
		//------------
		user->drawManager(window);

		if(!isServer){
			window.clear();
			window.draw(chatCheck);
			if(chatLog.size() > 0)
				sentText.setString(chatLog[chatLog.size()-1]);
			window.draw(sentText);
			window.draw(chatText);
			sf::IntRect test(0+24*x,0+32*y,24,32);
			if( clock.getElapsedTime().asMilliseconds() > 450){
				clock.restart();
				x = x < 2? x+1:0;
			}
			player.setTextureRect(test);
			window.draw(player);
			window.display();
		}
	}
	//------------------------------
	RakNet::RakPeerInterface::DestroyInstance(peer);
	return 0;
}