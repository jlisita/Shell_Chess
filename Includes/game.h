#ifndef GAME_H
#define GAME_H

#include "board.h"

void game();
void turn(Color Player,Square board[8][8]);
void freeBuffer();
int readCommand(char* command);

#endif
