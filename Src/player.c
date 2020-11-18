#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "game.h"
#include <time.h>

int initializePlayer(Player* player,char* name,Color color)
{
	int i;
	player->color = color;
	strcpy(player->name,name);
	player->isChess = 0;
	player->isMat = 0;
	player->capuredPieces = createListPiece();
	if(player->capuredPieces == NULL )
	{
		return -1;
	}

	for(i=0;i<16;i++)
	{
		player->pieces[i] = NULL;
	}
	// allocate memory for pieces
	for(i=0;i<16;i++)
	{
		player->pieces[i] = malloc(sizeof(Piece));
		if(player->pieces[i] == NULL)
		{
			goto error;
		}
	}

	// initialize pieces
	for(i=0;i<16;i++)
	{
		player->pieces[i]->color = color;
	}

	player->pieces[0]->name = ROOK;
	player->pieces[1]->name = KNIGHT;
	player->pieces[2]->name = BISHOP;
	player->pieces[3]->name = QUEEN;
	player->pieces[4]->name = KING;
	player->pieces[5]->name = BISHOP;
	player->pieces[6]->name = KNIGHT;
	player->pieces[7]->name = ROOK;

	for(i=8;i<16;i++)
	{
		player->pieces[i]->name = PAWN;
	}

	for(i=0;i<8;i++)
	{
		player->pieces[i]->j[0] = i;
		if(color==WHITE)
		{
			player->pieces[i]->i[0] = 0;
		}
		else
		{
			player->pieces[i]->i[0] = 7;
		}
	}

	for(i=8;i<16;i++)
	{
		if(color == WHITE)
		{
			player->pieces[i]->i[0] = 1;
			player->pieces[i]->j[0] = i - 8;
		}
		else
		{
			player->pieces[i]->i[0] = 6;
			player->pieces[i]->j[0] = i - 8;
		}
	}
	player->king = player->pieces[4];
	return 0;

	error:

	for(i=0;i<16;i++)
	{
		free(player->pieces[i]);
	}

	return -1;
}

int updateCapturePiece(Player* player,int k,int l)
{
	if(addPiece(player->capuredPieces, cb.array[k][l].piece)==-1)
	{
		return -1;
	}
	cb.array[k][l].piece = NULL;
	cb.array[k][l].isOccupied = 0;

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

