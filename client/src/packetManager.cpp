#ifndef __PACKETMANAGER_CPP__
#define __PACKETMANAGER_CPP__
#include "packetManager.h"

packetManager::packetManager(){
}

void packetManager::init(RakNet::RakPeerInterface *_peer,Player *_player=NULL){
	//packetManager
	clientpackets = new clientPacketHandler(_peer);
	if(_player !=NULL) clientpackets->setPlayer(_player);
	else std::cout << "packetManager Init - Player is NULL" << std::endl;
}

void packetManager::manager(RakNet::Packet* receivedPacket){
	clientpackets->handler(receivedPacket);
}

clientPacketHandler* packetManager::getClientPacketHandler(){
	return clientpackets;
}

#endif