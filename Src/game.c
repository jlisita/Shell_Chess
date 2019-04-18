#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void chess()
{
	Player player1;
	Player player2;
	initializePlayer(&player1,&player2);
	newGame(&player1,&player2);

}

void newGame(Player* player1,Player* player2)
{
	int endMatch = 0;
	Player* lastTurn = (player1->color == WHITE) ? player2 : player1;
	initializeBoard();
	setColor(player1,player2);

	do
	{
		if(lastTurn == player1)
		{
			turn(player2);
			lastTurn = player2;
		}
		else
		{
			turn(player1);
			lastTurn = player1;
		}

	}while(!endMatch);

}

void turn(Player* player)
{
	int i,j,k,l;
	char command[7]="";
	int isValid;
	do
	{
		do
		{
			printfBoard(player->color);
			printf("%s saisir votre deplacement:",player->name);
			isValid = readCommand(command);
		}while(!isValid);

		i = rankIndexToInt(command[1]);
		j = fileIndexToInt(command[0]);
		k = rankIndexToInt(command[4]);
		l = fileIndexToInt(command[3]);

		isValid = canMovePiece(player->color,i,j,k,l);

	}while(!isValid);
	movePiece(i,j,k,l);
}

void freeBuffer()
{
	int c = 0;
	do
	{
		c = getchar();
	}while(c!='\n' && c!=EOF);

}

int readCommand(char* command)
{
	char* rankIndex = "12345678";
	char* fileIndex = "abcdefghABCDEFGH";
	if(fgets(command,6,stdin) != NULL)
	{
		if(strchr(fileIndex, command[0]) != NULL && strchr(rankIndex, command[1])!=NULL && command[2]==' '
			&& strchr(fileIndex, command[3])!=NULL && strchr(rankIndex, command[4])!=NULL)
		{
			freeBuffer();
			return 1;
		}
		else
		{
			freeBuffer();
			printf("commande invadide\n");
			return 0;
		}
	}
	else
	{
		freeBuffer();
		return 0;
	}
}
