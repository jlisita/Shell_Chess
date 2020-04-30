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
	Piece* pieces[16];
	ListPieces* capuredPieces;
};

int initializePlayer(Player* player,Color color);
int updateCapturePiece(Player* player,int k,int l);
ListPieces* createListPiece();
int addPiece(ListPieces* list, Piece* piece);
void printListPieces(ListPieces* list);

#endif
