#include "packetHandler.cpp"
#include "BitStream.h"
#include <stdio.h>
#include <iostream>
#include <vector>

#define MAX_CLIENTS 10
#define SERVER_PORT 60000
using namespace RakNet;
class clientPacketHandler : public packetHandler{
	protected:
		/**
		 * Inherited variables
		 * RakNet::Packet *packet;
		 */
	public:
		std::vector<std::string> *chatlog;
		clientPacketHandler(RakNet::RakPeerInterface *_peer){
			this->peer=_peer;
	}
		void setChatlog(std::vector<std::string> *_chatlog){
			chatlog =_chatlog;
		}
		void handler(RakNet::Packet *receivedPacket){
			printf("Packets for client detected!!!\n");
			switch (receivedPacket->data[0])
			{
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					printf("Another client has disconnected.\n");
					break;
				case ID_REMOTE_CONNECTION_LOST:
					printf("Another client has lost the connection.\n");
					break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					printf("Another client has connected.\n");
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					{
						printf("Our connection request has been accepted.\n");
						//peer->CloseConnection(RakNet::AddressOrGUID(packet),true,'\000',LOW_PRIORITY);
						//Use a BitStream to write a custom user message
						//Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
						BitStream bsOut;
						bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
						std::string loginphrase;
						loginphrase = player->getName()+" has logged in.";
						bsOut.Write(loginphrase.c_str());
						std::cout << receivedPacket->systemAddress.ToString() << std::endl;
						peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,receivedPacket->systemAddress,false);
						BitStream userRequest;
						userRequest.Write((RakNet::MessageID)REQUEST_TO_CREATE_PLAYER);
						userRequest.Write(player->getName().c_str());
						peer->Send(&userRequest,HIGH_PRIORITY,RELIABLE_ORDERED,0,receivedPacket->systemAddress,false);
					}
					break;					
				case ID_NEW_INCOMING_CONNECTION:
					printf("A connection is incoming.\n");
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf("The server is full.\n");
					break;
				case ID_ALREADY_CONNECTED:
					printf("I'M ALREADY CONNECTED.\n");
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("We have been disconnected.\n");
					break;
				case ID_CONNECTION_LOST:
					printf("Connection lost.\n");
					break;
				case ID_GAME_MESSAGE_1:
					{
						RakNet::RakString rs;
						RakNet::BitStream bsIn(receivedPacket->data,receivedPacket->length,false);
						bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
						bsIn.Read(rs);
						printf("%s\n",rs.C_String());
						chatlog->push_back(rs.C_String());
					}
					break;
				case REQUEST_TO_CREATE_PLAYER_SUCCESS:
					{
						RakNet::RakString rs;
						RakNet::BitStream bsIn(receivedPacket->data,receivedPacket->length,false);
						bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
						bsIn.Read(rs);
						std::cout << "I Successfully received my character " << rs.C_String() << " !" << std::endl;
						bsIn.Read(rs);
						printf("Image path is %s!\n",rs.C_String());
						player->loadTexture(rs.C_String());
					}
					break;
				case UPDATE_PLAYER_POSITION:
					{
						printf("I received the packet for updating the position!\n");
						RakNet::BitStream bsIn(receivedPacket->data,receivedPacket->length,false);
						bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
						float posx, posy;
						bsIn.Read(posx); bsIn.Read(posy);
						//printf("New position: %f %f\n",posx,posy);
						player->setPosition(posx,posy);
					}
					break;
				default:
					printf("Message with identifier %i has arrived.\n", receivedPacket->data[0]);
					//somehow send it back to the serverclient...
					break;
			}
		}
};
