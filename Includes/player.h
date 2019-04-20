#ifndef PLAYER_H
#define PLAYER_H

#include "piece.h"

typedef struct Player Player;
struct Player
{
	char name[100];
	Color color;
	Piece* king;
	int isChess;
	int isMat;
};

void createPlayers(Player* player1, Player* player2);
void initializePlayers(Player* player1, Player* player2);

#endif
