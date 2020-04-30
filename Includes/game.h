#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"

#define MAXCOMMANDSIZE 10

int menuMode(int* mode);
int game();
int newMatch();
int turn(Player* player, char* recordedMoves);
void updateRecordedMoves(Player* player, char* recordedMoves, char* command);
int verifyCommand(char* command);
int readString(char* string, int sizeMax);
int readInt();
void freeBuffer();

#endif
