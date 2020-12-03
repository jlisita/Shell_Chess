#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "player.h"
#include "game.h"
#include "cmd_interface.h"

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
	int counterMove;
}cb;

int initializeBoard(Player* player1, Player* player2);
void printBoard(Color player);
int rankIndexToInt(char c);
int fileIndexToInt(char c);
Piece* getPiece(int i, int j);
Name getName(int i, int j);
int isEmptySquare(int i,int j);
int existSquare(int i, int j);
int isEmptyBetween(int i, int j, int k, int l);
Color getColor(int i, int j);
Name getName(int i, int j);
int setName(int i, int j, Name name);
int canBeEaten(Player* player, Player* adversary, int k, int l, int withoutChess);
int testChess(Player* player, Player* adversary);
int canDoLegalMove(Player* player, Player* adversary);
int testCastling(Player* player, Player* adversary, int i, int j, int k, int l);
int canMovePiece(Player* player, Player* adversary, int i, int j, int k, int l, int* captured1, int* captured2, int withoutChess, char* invalidMessage);
int isCapturingPiece(Color playerColor, int i, int j);
int movePiece(int i, int j, int k, int l);
int castling(int i, int j, int k, int l);
void updatePosition( int counterMove);
int updateBoard(Player* player, int i, int j, int k, int l, int captured1, int captured2);
int promotion(int i, int j);
int promotionOnline(Player* player, int i, int j, int socket);
int capturePiece(Player* player,int k,int l);

#endif
