#include "RakPeerInterface.h"
#include "BitStream.h"
#include "serverPacketHandler.cpp"
#include "clientPacketHandler.cpp"
#include "ServerDataContainer.cpp"
#include <iostream>
class packetManager{
private:
	//RakNet::server
	serverPacketHandler* serverpackets;
	clientPacketHandler* clientpackets;
	ServerDataContainer* serverData; //retrieved from serverclient
protected:
	RakNet::Packet *packet;
public:
	packetManager(){
	}
	void init(RakNet::RakPeerInterface *_peer,bool isServer=false,Player *_player=NULL){
		//packetManager
		if (isServer)
			serverpackets = new serverPacketHandler(_peer);
		else{
			clientpackets = new clientPacketHandler(_peer);
			if(_player !=NULL) clientpackets->setPlayer(_player);
			else std::cout << "Player is NULL" << std::endl;
		}
	}
	void manager(RakNet::Packet* receivedPacket,bool isServer){
		if (isServer)
			serverpackets->handler(receivedPacket);
		else
			clientpackets->handler(receivedPacket);
	}
	clientPacketHandler* getClientPacketHandler(){
		return clientpackets;
	}
	void setServerData (ServerDataContainer* _serverData){
		if(_serverData == NULL)
			std::cout << "ServerData is NULL" << std::endl;
		serverData = _serverData;
		if(serverpackets != NULL)
			serverpackets->setServerData(serverData);
	}
};