#ifndef __PACKETHANDLER__
#define __PACKETHANDLER__
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "Player.cpp"

//global variable for messages
	enum GameMessages{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM+1,
		REQUEST_TO_CREATE_PLAYER = ID_USER_PACKET_ENUM+2,
		REQUEST_TO_CREATE_PLAYER_SUCCESS =  ID_USER_PACKET_ENUM+3,
		REQUEST_FOR_PLAYER_TO_MOVE = ID_USER_PACKET_ENUM+4,
		UPDATE_PLAYER_POSITION =  ID_USER_PACKET_ENUM+8
	};
class packetHandler{
private:
	//RakNet::server
protected:
	//=======
	// Entity Info
	//=======
	Player *player;
	RakNet::Packet *packet;
	RakNet::RakPeerInterface *peer;
public:
	packetHandler(RakNet::Packet *receivedPacket = NULL){
		packet = receivedPacket;
	}
	void setPlayer(Player* _player){
		player = _player;
	}
	Player *getPlayer(){
		return player;
	}
	virtual void handler(RakNet::Packet *receivedPacket)  = 0;
};
#endif