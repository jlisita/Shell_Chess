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
	char name[SIZENAME];
	Color color;
	Piece* king;
	int isChess;
	int isMat;
	ListPieces* capuredPieces;
};


void createPlayers(Player* player1, Player* player2);
int initializePlayers(Player* player1, Player* player2);
int updateCapturePiece(Player* player,int k,int l);
ListPieces* createListPiece();
int addPiece(ListPieces* list, Piece* piece);
void printListPieces(ListPieces* list);

#endif
