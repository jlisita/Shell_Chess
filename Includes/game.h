#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"

void game();
void newMatch(Player* player1,Player* player2);
void turn(Player* player, char* recordedMoves);
void updateRecordedMoves(Player* player, char* recordedMoves, char* command);
int verifyCommand(char* command);
int readString(char* string);
int readInt();
void freeBuffer();

#endif
