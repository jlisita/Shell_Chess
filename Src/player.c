#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "game.h"
#include <time.h>

Player* createPlayer(char* name,Color color)
{
	int i;
	Player* player = NULL;

	player = malloc(sizeof(Player));
	if(player == NULL)
	{
		perror("malloc");
		return NULL;
	}
	player->color = color;
	strcpy(player->name,name);
	player->isChess = 0;
	player->isMat = 0;
	player->abandonment = 0;
	player->isStalemate = 0;
	player->isCastling = 0;
	player->hasCastled = 0;
	if(color == WHITE)
	{
		player->isPlaying = 1;
	}
	else
	{
		player->isPlaying = 0;
	}
	strcpy(player->command,"");
	player->capuredPieces = createListPiece();
	if(player->capuredPieces == NULL )
	{
		fprintf(stderr,"createListPiece returned error\n");
		return NULL;
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
			perror("malloc");
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

	return player;

	error:

	for(i=0;i<16;i++)
	{
		free(player->pieces[i]);
	}

	return NULL;
}

int deletePlayer(Player* player)
{
	int i;
	if(player == NULL)
	{
		return -1;
	}
	for(i=0;i<16;i++)
	{
		free(player->pieces[i]);
	}
	free(player->capuredPieces);
	return 1;
}

ListPieces* createListPiece()
{
	ListPieces* newList = malloc(sizeof(ListPieces));
	if (newList==NULL)
	{
		perror("malloc");
		return NULL;
	}
	newList->first=NULL;
	newList->size=0;

	return newList;
}

int deleteListPiece(ListPieces* list)
{
	Element* toRemove = NULL;
	if(list==NULL)
	{
		return -1;
	}
	while(list->size>0)
	{
		toRemove = list->first;
		list->first = list->first->next;
		free(toRemove);
		list->size--;
	}
	free(list);

	return 0;
}

int addPiece(ListPieces* list, Piece* piece)
{
	Element* newElement = malloc(sizeof(Element));
	Element* ptrEl= NULL;
	if(newElement==NULL || list==NULL || piece==NULL)
	{
		perror("malloc");
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

int printListPieces(ListPieces* list)
{
	Element* ptrEl= NULL;
	if(list==NULL)
	{
		return -1;
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
	return 0;

}

