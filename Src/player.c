#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include <time.h>

void initializePlayer(Player* player1, Player* player2)
{
	printf("Player 1: Enter your name\n");
	scanf("%s",player1->name);
	printf("Player 2: Enter your name\n");
	scanf("%s",player2->name);
}

void setColor(Player* player1, Player* player2)
{
	Color color;
	srand(time(NULL));

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
