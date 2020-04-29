#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "game.h"
#include <time.h>

void createPlayers(Player* player1, Player* player2)
{
	printf("Player 1: Enter your name\n");
	readString(player1->name,SIZENAME);
	printf("Player 2: Enter your name\n");
	readString(player2->name,SIZENAME);
	player1->king = NULL;
	player2->king = NULL;
}

int initializePlayers(Player* player1, Player* player2)
{
	Color color;
	srand(time(NULL));

	player1->isChess = 0;
	player2->isChess = 0;
	player1->isMat = 0;
	player2->isMat = 0;
	player1->capuredPieces = createListPiece();
	player2->capuredPieces = createListPiece();
	if(player1->capuredPieces == NULL || player2->capuredPieces == NULL )
	{
		return -1;
	}
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
	return 0;
}

int updateCapturePiece(Player* player,int k,int l)
{
	if(cb.array[k][l].isOccupied)
	{
		if(addPiece(player->capuredPieces, cb.array[k][l].piece)==-1)
		{
			return -1;
		}
	}
	return 0;
}

ListPieces* createListPiece()
{
	ListPieces* newList = malloc(sizeof(ListPieces));
	if (newList==NULL)
	{
		return NULL;
	}
	newList->first=NULL;
	newList->size=0;

	return newList;
}

int addPiece(ListPieces* list, Piece* piece)
{
	Element* newElement = malloc(sizeof(Element));
	Element* ptrEl= NULL;
	if(newElement==NULL || list==NULL || piece==NULL)
	{
		return -1;
	}
	newElement->value=piece;
	newElement->next=NULL;


	if(list->first==NULL)
	{
		list->first=newElement;

	}
	else
	{
		ptrEl = list->first;
		while(ptrEl->next!=NULL)
		{
			ptrEl = ptrEl->next;
		}
		ptrEl->next=newElement;
	}
	list->size++;
	return 0;
}

void printListPieces(ListPieces* list)
{
	Element* ptrEl= NULL;
	if(list==NULL)
	{
		exit(EXIT_FAILURE);
	}
	else
	{

		ptrEl = list->first;
		while(ptrEl!=NULL)
		{
			printfPiece(ptrEl->value);
			printf(" ");
			ptrEl = ptrEl->next;
		}
		printf("\n");
	}

}

