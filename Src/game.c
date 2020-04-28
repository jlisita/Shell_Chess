#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"

// initialize player and launch new match
int game()
{
	int restart = 0;
	Player player1;
	Player player2;
	createPlayers(&player1,&player2);

	do
	{
		if(newMatch(&player1,&player2)==-1)
		{
			return -1;
		}

		printf("Do you want to play again ? (1 for yes/ 0 for no)\n");
		do
		{
			if(readInt(&restart) == -1)
			{
				return -1;
			}
			if(restart != 0 && restart != 1)
			{
				printf("Error: (1 for yes/ 0 for no)\n");
			}

		}while(restart != 0 && restart != 1);

	}while(restart);
	return 0;
}

// initialize board and manage player turn
int newMatch(Player* player1,Player* player2)
{
	char recordedMoves[1000]="";
	int endMatch = 0;
	Color nextTurn = WHITE;
	Player* currentPlayer = NULL;
	Player* adversary = NULL;

	if(initializePlayers(player1,player2)==-1)
	{
		return -1;
	}
	if(initializeBoard(player1,player2)==-1)
	{
		return -1;
	}

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

		if(turn(currentPlayer,recordedMoves)==-1)
		{
			return -1;
		}

		adversary->isChess = testChess(adversary);
		if(adversary->isChess == -1)
		{
			return -1;
		}
		if(adversary->isChess)
		{
			printf("CHESS!\n");
			adversary->isMat = testMat(adversary);
			if(adversary->isChess == -1)
			{
				return -1;
			}
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
	return 0;
}

// manage command entry and update game
int turn(Player* player, char* recordedMoves)
{
	int i,j,k,l;
	char command[10]="";
	int validCommand=0, validMovement=0;

	printfBoard(player->color);
	printf("%s\n",recordedMoves);
	printf("Pieces captured: ");
	printListPieces(player->capuredPieces);

	do
	{
		do
		{
			printf("%s, enter your move:",player->name);
			validCommand = verifyCommand(command);
			if(validCommand == -1)
			{
				return -1;
			}
		}while(validCommand!=1);

		i = rankIndexToInt(command[1]);
		j = fileIndexToInt(command[0]);
		k = rankIndexToInt(command[4]);
		l = fileIndexToInt(command[3]);

		validMovement = canMovePiece(player,i,j,k,l,0,0);

	}while(!validMovement);

    updateRecordedMoves(player,recordedMoves,command);
	if(updateCapturePiece(player,k,l)==-1)
	{
		return -1;
	}
	movePiece(i,j,k,l);
	return 0;
}

// update list of moves during the match
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


// test if command entered is valid
int verifyCommand(char* command)
{
	char* rankIndex = "12345678";
	char* fileIndex = "abcdefghABCDEFGH";
	if(readString(command,MAXCOMMANDSIZE)==-1)
	{
		return -1;
	}
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

// store string enter in console
int readString(char* string, int sizeMax)
{
	char* posCR = NULL;
	if(fgets(string,sizeMax,stdin)!=NULL)
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
		return 0;
	}
	else
	{
		freeBuffer();
		return -1;
	}
}

// store integer entered in console
int readInt(int* nbr )
{
	char string[100]="";
	if(fgets(string,100,stdin)!=NULL)
	{
		*nbr = (int) strtol(string,NULL,10);
		return 0;
	}
	else
	{
		return -1;
	}
}

// remove data from stdin buffer
void freeBuffer()
{
	int c = 0;
	do
	{
		c = getchar();
	}while(c!='\n' && c!=EOF);
}
