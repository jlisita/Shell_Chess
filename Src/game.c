#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"

void game()
{
	Player player1;
	Player player2;
	createPlayers(&player1,&player2);
	int restart = 0;

	do
	{
		newMatch(&player1,&player2);

		printf("Do you want to play again ? (1 for yes/ 0 for no)\n");
		do
		{
			restart = readInt();
			if(restart != 0 && restart != 1)
			{
				printf("Error: (1 for yes/ 0 for no)\n");
			}

		}while(restart != 0 && restart != 1);

	}while(restart);
}

void newMatch(Player* player1,Player* player2)
{
	char recordedMoves[1000]="";
	int endMatch = 0;
	Color nextTurn = WHITE;
	Player* currentPlayer = NULL;
	Player* adversary = NULL;

	initializePlayers(player1,player2);
	initializeBoard(player1,player2);

	do
	{
		if(nextTurn == player1->color)
		{
			currentPlayer = player1;
			adversary = player2;
			nextTurn = player2->color;
		}
		else
		{
			currentPlayer = player2;
			adversary = player1;
			nextTurn = player1->color;
		}

		turn(currentPlayer,recordedMoves);

		adversary->isChess = testChess(adversary);
		if(adversary->isChess)
		{
			printf("CHESS!\n");
			adversary->isMat = testMat(adversary);
		}
		if(adversary->isMat)
		{
			printf("MAT!!\n");
			endMatch = 1;
		}

	}while(!endMatch);

	if(player1->isMat)
	{
		printf("%s has won by Mat.\n",player2->name);
	}
	else
	{
		printf("%s has won by Mat.\n",player1->name);
	}

}

void turn(Player* player, char* recordedMoves)
{
	int i,j,k,l;
	char command[100]="";
	int isValid;
	do
	{
		do
		{
			printfBoard(player->color);
			printf("%s\n",recordedMoves);
			printf("Pieces captured: ");
			printListPieces(player->capuredPieces);
			printf("%s, enter your move:",player->name);
			isValid = readCommand(command);
		}while(!isValid);

		i = rankIndexToInt(command[1]);
		j = fileIndexToInt(command[0]);
		k = rankIndexToInt(command[4]);
		l = fileIndexToInt(command[3]);

		isValid = canMovePiece(player,i,j,k,l,0);

	}while(!isValid);

    updateRecordedMoves(player,recordedMoves,command),
	updateCapturePiece(player,k,l);
	movePiece(i,j,k,l);
}

void updateRecordedMoves(Player* player, char* recordedMoves, char* command)
{
	static int indexMoves = 0;
	if(player->color==WHITE)
	{
		indexMoves++;
		char index[10]="";
		sprintf(index,"%d. ",indexMoves);
		strcat(recordedMoves,index);
	}
	char cmd[10]="";
	sprintf(cmd,"%c%c%c%c ",tolower(command[0]),tolower(command[1]),tolower(command[3]),tolower(command[4]));
	strcat(recordedMoves,cmd);
}

int readCommand(char* command)
{
	char* rankIndex = "12345678";
	char* fileIndex = "abcdefghABCDEFGH";
	if(readString(command)!=0)
	{
		if(strchr(fileIndex, command[0]) != NULL && strchr(rankIndex, command[1])!=NULL && command[2]==' '
			&& strchr(fileIndex, command[3])!=NULL && strchr(rankIndex, command[4])!=NULL)
		{
			return 1;
		}
		else
		{
			printf("Commande invalide\n");
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int readString(char* string)
{
	char* posCR = NULL;
	if(fgets(string,100,stdin)!=NULL)
	{
		posCR = strchr(string,'\n');
		if(posCR != NULL )
		{
			*posCR = '\0';
		}
		else
		{
			freeBuffer();
		}
		return 1;
	}
	else
	{
		freeBuffer();
		return 0;
	}
}

int readInt()
{
	char string[100]="";
	if(fgets(string,100,stdin)!=NULL)
	{
		return (int) strtol(string,NULL,10);

	}
	else
	{
		return 0;
	}
}

void freeBuffer()
{
	int c = 0;
	do
	{
		c = getchar();
	}while(c!='\n' && c!=EOF);
}
