#ifndef __CLIENTCPP__
#define __CLIENTCPP__

#include "client.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "MessageIdentifiers.h"
#include "BitStream.h"
#include <stdio.h>

using namespace RakNet;

void client::clientInit(){
	isUp = isDown = isLeft = isRight = isShiftDown = isChatting = false;
	repeat = true;
	if (!font.loadFromFile("Book Antiqua.ttf"))
		std::exit(0);
	chatText.setFont(font); chatText.setCharacterSize(25); chatText.setString("");
	sentText.setFont(font); sentText.setCharacterSize(25); sentText.setString("");
	chatCheck.setFont(font); chatCheck.setCharacterSize(17); chatCheck.setString("Chat: off");
	chatCheck.setPosition(0,550);
	chatText.setPosition(0,565);
	printf("Starting client...\n");
	sd = new RakNet::SocketDescriptor();
	peer->Startup(1,*&sd, 1);
	//=========
	//player setup
	//=========
	player = new Player();
	player->playerInit();
	//---------------------------------------
	//Packet Handling classes inited
	//---------------------------------------
	packet_manager.init(peer,player);
	packet_manager.getClientPacketHandler()->otherPlayers = &otherPlayers;
}

client::client(){
	packet = NULL;
	sd = NULL;
	player = NULL;
	peer = RakNet::RakPeerInterface::GetInstance();
	clientInit();
}

char client::keypressToChar(sf::Keyboard::Key keypressed, bool isShiftPressed=false){
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
			return keypressed;
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

void client::setChatlog(){
	if(chatlog != NULL)
		packet_manager.getClientPacketHandler()->setChatlog(chatlog);
}

void client::clientConnectionStart(){
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		gets(str);
		if (str[0]==0){
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client.\n");
		printf("IP being used is: %s\n",str);
		peer->Connect(str, SERVER_PORT, 0,0);
}

void client::drawManager(sf::RenderWindow &window){
	std::map<std::string, Player>::iterator it;
	for (it = otherPlayers.begin(); it !=otherPlayers.end(); it++){
		it->second.draw(window);
	}
	if(player != NULL){
		player->draw(window);
	}
	window.draw(chatCheck);
	if(chatlog->size() > 0) sentText.setString((*chatlog)[chatlog->size()-1]);
	window.draw(sentText);
	window.draw(chatText);
}

void client::requestsToServer(){
	//-----------
	//character calculations: Request to move the character
	//------------
	if( (isUp==true) || (isDown==true) || (isLeft==true) || (isRight==true) ) {
		BitStream bsOut;
		bsOut.Write((RakNet::MessageID)REQUEST_FOR_PLAYER_TO_MOVE);
		RakNet::RakString playerName("%s",player->getName());
		printf("PLAYERNAME IS %s",playerName.C_String());
		bsOut.Write(playerName);
		bsOut.Write(isUp); bsOut.Write(isDown); bsOut.Write(isLeft); bsOut.Write(isRight);
		peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);	
	}
}

void client::inputHandler(sf::Event &event, sf::RenderWindow &window){
	int y;
	if (event.type == sf::Event::KeyReleased){
		if (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift)	isShiftDown = false;
		if(event.key.code == sf::Keyboard::Up) { isUp = false; printf("UP release\n");}
		if(event.key.code == sf::Keyboard::Down)	{ isDown = false;  printf("DOWN release\n");}
		if(event.key.code == sf::Keyboard::Left) { isLeft = false;  printf("LEFT release\n");}
		if(event.key.code == sf::Keyboard::Right)	{isRight = false; printf("RIGHT release\n"); }
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
		else std::cout << event.type << " " <<keypressToChar(event.key.code) << std::endl;
	}
}

void client::packetManager(){
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
#endif
