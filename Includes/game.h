#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include <netinet/in.h>
#include "profil.h"

#define MAXCOMMANDSIZE 10


typedef struct GameData GameData;
struct GameData
{
	Player* player1;
	Player* player2;
	char recordedMoves[1000];
	int endGame;
	Color nextTurn;
	Player* currentPlayer;
	Player* adversary;
};


int menuMode(int* mode);
int menuOnline(int* ret, ListProfil* friends);
int game();
int localGame();
int initGame(GameData* gameData);
int onlineGame(Profil* myProfil, Profil* adversaryProfil,int mode);
int turn(Player* player, char* recordedMoves);
int getCommand(Player* player);
int verifyCommand(Player* player);
int nextMovement(Player* player, int* i, int* j, int* k, int* l, int* captured1, int*captured2);
int updateBoard(Player* player, int i, int j, int k, int l, int captured1, int captured2);
int endOfGame(GameData* gameData);
void updateRecordedMoves(Player* player, char* recordedMoves);
int connexion(char* ipServeur, char* ipClient, int mode, int* socketServeur, int* socketClient, struct sockaddr_in* sinServeur, struct sockaddr_in* sinClient);
int readString(char* string, int sizeMax);
int readInt();
void freeBuffer();

#endif
