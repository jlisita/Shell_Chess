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
	player1->capuredPieces = createListPiece();
	player2->capuredPieces = createListPiece();
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

void updateCapturePiece(Player* player,int k,int l)
{
	if(cb.array[k][l].isOccupied)
	{
		addPiece(player->capuredPieces, cb.array[k][l].piece);
	}

}

ListPieces* createListPiece()
{
	ListPieces* newList = malloc(sizeof(ListPieces));
	if (newList==NULL)
	{
		exit(EXIT_FAILURE);
	}
	newList->first=NULL;
	newList->size=0;

	return newList;
}

void addPiece(ListPieces* list, Piece* piece)
{
	Element* newElement = malloc(sizeof(Element));
	Element* ptrEl= NULL;
	if(newElement==NULL || list==NULL || piece==NULL)
	{
		exit(EXIT_FAILURE);
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

