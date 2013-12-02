#include "packetHandler.cpp"
#include "ServerDataContainer.cpp"
#include "Player.cpp"
#include <stdio.h>
class serverPacketHandler : public packetHandler{
	private:
		ServerDataContainer *serverData;
	protected:
		/**
		 * Inherited variables
		 * RakNet::Packet *packet;
		 */
	public:
		serverPacketHandler(RakNet::RakPeerInterface *_peer){
			this->peer=_peer;
		}
		void setServerData(ServerDataContainer *_serverData){
			if(_serverData != NULL) serverData = _serverData;
			else std::cout << "Server Data is Null." << std::endl;
		}
		void handler(RakNet::Packet *receivedPacket){
			printf("Packets for server detected!!!\n");
			switch (receivedPacket->data[0]){
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					printf("Another client has disconnected.\n"); break;
				case ID_REMOTE_CONNECTION_LOST:
					printf("Another client has lost the connection.\n"); break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					printf("Another client has connected.\n"); break;
				case ID_CONNECTION_REQUEST_ACCEPTED:		
					break;
				case ID_NEW_INCOMING_CONNECTION:
					printf("A connection is incoming.\n"); break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf("The server is full.\n"); break;
				case ID_ALREADY_CONNECTED:
					printf("I'M ALREADY CONNECTED.\n"); break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n"); break;
				case ID_CONNECTION_LOST:
					printf("A client lost connection.\n"); break;
				case ID_GAME_MESSAGE_1:
					{
						RakNet::RakString rs;
						RakNet::BitStream bsIn(receivedPacket->data,receivedPacket->length,false);
						bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
						bsIn.Read(rs);
						printf("%s\n",rs.C_String());
						//If you put a specific adress it will not send to that one even with broadcast boolean set to true.
						//RakNet::UNASSIGNED_SYSTEM_ADDRESS will be able to broad cast it to everyone, if broadcast is set to true.
						//peer->Send(&bsIn,HIGH_PRIORITY,RELIABLE_ORDERED,0,receivedPacket->systemAddress,true);	
						peer->Send(&bsIn,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);	
					}
					break;
				case REQUEST_TO_CREATE_PLAYER:
					{
						RakNet::RakString rs;
						RakNet::BitStream bsIn(receivedPacket->data,receivedPacket->length,false);
						bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
						bsIn.Read(rs);
						printf("%s\n",rs.C_String());
						Player newPlayer;
						newPlayer.playerInit(rs.C_String());
						if(serverData != NULL)
							serverData->addNewPlayer(newPlayer);
						else
							printf("SERVERDATA IS NULL");
						RakNet::BitStream bsOut;
						bsOut.Write((RakNet::MessageID)REQUEST_TO_CREATE_PLAYER_SUCCESS);
						printf("Character creation success!");
						Player *temp;
						printf("%d",sizeof(temp));
						temp = serverData->getPlayer(newPlayer.getName());
						printf("%s \n",serverData->getPlayer(newPlayer.getName())->getName());
						printf("Temp: %s \n",temp->getName());
						bsOut.Write(temp->getName().c_str());
						bsOut.Write(temp->getSpritePath().c_str());
						peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,receivedPacket->systemAddress,false);	
					}
					break;
				case REQUEST_FOR_PLAYER_TO_MOVE:
					{
						RakNet::RakString rs;
						RakNet::BitStream bsIn(receivedPacket->data,receivedPacket->length,false);
						bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
						//Order: up down left right
						bool up, down, left, right;
						unsigned short strlength;
						char* playerName;
						bsIn.Read(strlength); bsIn.Read(playerName,strlength); bsIn.Read(up); bsIn.Read(down); bsIn.Read(left); bsIn.Read(right); 
						printf("test %s!!!\n",playerName);
						Player *modifyPlayer = serverData->getPlayer(playerName);
						if(modifyPlayer != NULL){
							if(up) { printf("UP"); modifyPlayer->move(0,-3);}
							if(down) { printf("down");modifyPlayer->move(0,3);}
							if(left) { printf("left");modifyPlayer->move(-3,-0);}
							if(right) { printf("right");modifyPlayer->move(3,0);}
							printf("%f %f\n",modifyPlayer->getPosition().x,modifyPlayer->getPosition().y);
							//Sending an update of player position
							RakNet::BitStream bsOut;
							bsOut.Write((RakNet::MessageID)UPDATE_PLAYER_POSITION);
							bsOut.Write((float)modifyPlayer->getPosition().x);
							bsOut.Write((float)modifyPlayer->getPosition().y);
							peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,receivedPacket->systemAddress,false);	
						}
					}
					break;
				default:
					printf("Message with identifier %i has arrived.\n", receivedPacket->data[0]);
					//default
					break;
			}
		}
};
