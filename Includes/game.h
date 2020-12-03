#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include <netinet/in.h>
#include "profil.h"

#define MAXCOMMANDSIZE 10


typedef struct Game Game;
struct Game
{
	Player* player;
	Player* adversary;
	char recordedMoves[1000];
	int endOfGame;
	Player* currentPlayer;
	Player* nextPlayer;
};

int localGame();
Game* createGame();
int deleteGame(Game* game);
int turn(Player* currentPlayer, Player* nextPlayer, char* recordedMoves);
int getCommand(Player* player);
int verifyCommand(Player* player);
int nextMovement(Player* currentPlayer, Player* nextPlayer,int* i, int* j, int* k, int* l, int* captured1, int*captured2);
int endOfGame(Player* currentPlayer, Player* nextPlayer);
void updateRecordedMoves(Player* player, char* recordedMoves);
int onlineGame(Profil* myProfil, Profil* adversaryProfil, int mode);
int turnOnline(Player* currentPlayer, Player* nextPlayer, int isPlaying, char* recordedMoves, int socketClient);
int connexion(char* ipServeur, char* ipClient, int mode, int* socketServeur, int* socketClient, struct sockaddr_in* sinServeur, struct sockaddr_in* sinClient);
void printGame(Player* player, Player* adversary, char* recorderMoves);

#endif
