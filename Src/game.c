#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"


void game()
{
	Square board[8][8];
	initializeBoard(board);

	do
	{
		printfBoard(board,WHITE);
		printf("Joueur blanc\n");
		turn(WHITE,board);
		printfBoard(board,BLACK);
		printf("Joueur noire\n");
		turn(BLACK,board);

	}while(1);

}

void turn(Color player,Square board[8][8])
{
	int i,j,k,l;
	char command[7]="";
	int isValid;
	do
	{
		do
		{
			printf("Saisir votre deplacement:");
			isValid = readCommand(command);
		}while(!isValid);

		i = rankIndexToInt(command[1]);
		j = fileIndexToInt(command[0]);
		k = rankIndexToInt(command[4]);
		l = fileIndexToInt(command[3]);

		isValid = canMovePiece(player,board,i,j,k,l);

	}while(!isValid);
	movePiece(board,i,j,k,l);
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
