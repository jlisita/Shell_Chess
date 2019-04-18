#ifndef PLAYER_H
#define PLAYER_H

#include "piece.h"

typedef struct Player Player;
struct Player
{
	char name[100];
	Color color;
};

void initializePlayer(Player* player1, Player* player2);
void setColor(Player* player1, Player* player2);

#endif
