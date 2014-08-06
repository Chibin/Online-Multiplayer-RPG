#include <stdio.h>
#include <string.h>
#include "server.cpp"
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
	std::vector<std::string> chatLog;
	serverclient *user = new serverclient();
	RakNet::RakPeerInterface *peer = user->peer;
	RakNet::Packet *packet;
	user->serverConnectionStart();
	 // Create a graphical text to display
	while (true)
	{
		user->packetManager();
	}
	//------------------------------
	RakNet::RakPeerInterface::DestroyInstance(peer);
	return 0;
}