#ifndef PLAYER_H
#define PLAYER_H

#include "piece.h"

#define SIZENAME 100

typedef struct Element Element;
struct Element
{
	Piece* value;
	Element* next;
};

typedef struct ListPieces ListPieces;
struct ListPieces
{
	Element* first;
	int size;
};

typedef struct Player Player;
struct Player
{
	Color color;
	Piece* king;
	char name[SIZENAME];
	int isChess;
	int isMat;
	int abandonment;
	int isCastling;
	int hasCastled;
	Piece* pieces[16];
	ListPieces* capuredPieces;
	char command[10];
};

int initializePlayer(Player* player, char* name,Color color);
ListPieces* createListPiece();
int addPiece(ListPieces* list, Piece* piece);
void printListPieces(ListPieces* list);

#endif
