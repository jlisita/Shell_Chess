#ifndef PLAYER_H
#define PLAYER_H

#include "piece.h"

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
	char name[100];
	Color color;
	Piece* king;
	int isChess;
	int isMat;
	ListPieces* capuredPieces;
};


void createPlayers(Player* player1, Player* player2);
void initializePlayers(Player* player1, Player* player2);
void updateCapturePiece(Player* player,int k,int l);
ListPieces* createListPiece();
void addPiece(ListPieces* list, Piece* piece);
void printListPieces(ListPieces* list);

#endif
