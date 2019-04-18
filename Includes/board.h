#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

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

void initializeBoard();
void printfBoard(Color player);
int rankIndexToInt(char c);
int fileIndexToInt(char c);
int isEmptySquare(int i,int j);
int isEmptyBetween(int i, int j, int k, int l);
Color getColor(int i, int j);
int canMovePiece(Color player,Square board[8][8], int i, int j, int k, int l);
void movePiece(Square board[8][8], int i, int j, int k, int l);


#endif
