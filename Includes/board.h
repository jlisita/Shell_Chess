#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "player.h"

typedef struct Square Square;
struct Square
{
	int isOccupied;
	Piece* piece;
};

typedef struct ChessBoard ChessBoard;
struct ChessBoard
{
	Square array[8][8];
}cb;

void initializeBoard(Player* player1, Player* player2);
void printfBoard(Color player);
int rankIndexToInt(char c);
int fileIndexToInt(char c);
int isEmptySquare(int i,int j);
int isEmptyBetween(int i, int j, int k, int l);
Color getColor(int i, int j);
int testChess(Player* player);
int testMat(Player* Player);
int canMovePiece(Player* player, int i, int j, int k, int l, int testChess, int isTestMat);
void movePiece(int i, int j, int k, int l);


#endif
