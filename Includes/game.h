#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include <netinet/in.h>
#include "profil.h"

#define MAXCOMMANDSIZE 10


typedef struct PartyData PartyData;
struct PartyData
{
	Player* player1;
	Player* player2;
	char recordedMoves[1000];
	int endMatch;
	Color nextTurn;
	Player* currentPlayer;
	Player* adversary;
};


int menuMode(int* mode);
int menuOnline(int* ret, ListProfil* friends);
int game();
int localParty();
int initParty(PartyData* pData);
int onlineParty(Profil* myProfil, Profil* adversaryProfil,int mode);
int turn(Player* player, char* recordedMoves);
int nextMovement(Player* player, int* i, int* j, int* k, int* l, int* captured1, int*captured2);
int updateBoard(Player* player, int i, int j, int k, int l, int captured1, int captured2);
void updateRecordedMoves(Player* player, char* recordedMoves);
int verifyCommand(char* command);
int connexion(char* ipServeur, char* ipClient, int mode, int* socketServeur, int* socketClient, struct sockaddr_in* sinServeur, struct sockaddr_in* sinClient);
int readString(char* string, int sizeMax);
int readInt();
void freeBuffer();

#endif
