#include "RakPeerInterface.h"
#include "BitStream.h"
#include "serverPacketHandler.cpp"
#include "ServerDataContainer.cpp"
#include <iostream>
class packetManager{
private:
	serverPacketHandler* serverpackets;
	ServerDataContainer* serverData; //retrieved from serverclient
protected:
	RakNet::Packet *packet;
public:
	packetManager(){ }
	void init(RakNet::RakPeerInterface *_peer){
		//packetManager
		serverpackets = new serverPacketHandler(_peer);
	}
	void manager(RakNet::Packet* receivedPacket){
		serverpackets->handler(receivedPacket);
	}
	void setServerData (ServerDataContainer* _serverData){
		if(_serverData == NULL)
			std::cout << "ServerData is NULL" << std::endl;
		serverData = _serverData;
		if(serverpackets != NULL)
			serverpackets->setServerData(serverData);
	}
};