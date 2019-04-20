#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "game.h"
#include <time.h>

void createPlayers(Player* player1, Player* player2)
{
	printf("Player 1: Enter your name\n");
	readString(player1->name);
	printf("Player 2: Enter your name\n");
	readString(player2->name);
	player1->king = NULL;
	player2->king = NULL;
}

void initializePlayers(Player* player1, Player* player2)
{
	Color color;
	srand(time(NULL));

	player1->isChess = 0;
	player2->isChess = 0;
	player1->isMat = 0;
	player2->isMat = 0;
	color = (Color)(rand()%2);

	if(color == WHITE)
	{
		player1->color = WHITE;
		player2->color = BLACK;
	}
	else
	{
		player1->color = BLACK;
		player2->color = WHITE;
	}
}
