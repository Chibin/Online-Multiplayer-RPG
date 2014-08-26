#ifndef __PACKETMANAGER_
#define __PACKETMANAGER_
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "clientPacketHandler.cpp"
#include <iostream>

class packetManager{
	private:
		//RakNet::server
		clientPacketHandler* clientpackets;
	protected:
		RakNet::Packet *packet;
	public:
		packetManager();
		void init(RakNet::RakPeerInterface *_peer,Player *_player);
		void manager(RakNet::Packet* receivedPacket);
		clientPacketHandler* getClientPacketHandler();

};
#endif
