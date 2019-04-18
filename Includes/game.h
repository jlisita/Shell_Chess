#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"

void chess();
void newGame(Player* player1,Player* player2);
void turn(Player* player);
void freeBuffer();
int readCommand(char* command);

#endif
