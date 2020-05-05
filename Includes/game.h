#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include <netinet/in.h>
#include "profil.h"

#define MAXCOMMANDSIZE 10

int menuMode(int* mode);
int menuOnline(int* ret, ListProfil* friends);
int game();
int localParty();
int onlineParty(Profil* myProfil, Profil* adversaryProfil,int mode);
int turn(Player* player, char* recordedMoves);
void updateRecordedMoves(Player* player, char* recordedMoves, char* command);
int verifyCommand(char* command);
int connexion(char* ipServeur, char* ipClient, int mode, int* socketServeur, int* socketClient, struct sockaddr_in* sinServeur, struct sockaddr_in* sinClient);
int readString(char* string, int sizeMax);
int readInt();
void freeBuffer();

#endif
