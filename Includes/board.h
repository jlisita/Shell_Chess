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

typedef struct Board Board;
struct Board
{
	Square array[8][8];
	int counterMove;
};

Board* createBoard(Player* player1, Player* player2);
int deleteBoard(Board* board);
int existSquare(int i, int j);
Square* getSquare(Board* board, int i, int j);
int isEmptySquare(Board* board, int i,int j);
Piece* getPiece(Board* board, int i, int j);
Color getColor(Board* board, int i, int j);
Name getName(Board* board, int i, int j);
int setName(Board* board, int i, int j, Name name);
int getCounterMove(Board* board);
void incrementCounterMove(Board* board);
int isEmptyBetween(Board* board, int i, int j, int k, int l);
int canMovePiece(Board* board, Player* player, Player* adversary, int i, int j, int k, int l, int* captured1, int* captured2, int withoutChess, char* invalidMessage);
int testChess(Board* board, Player* player, Player* adversary);
int canBeEaten(Board* board, Player* player, Player* adversary, int k, int l, int withoutChess);
int canDoLegalMove(Board* board, Player* player, Player* adversary);
int isCapturingPiece(Board* board, Color playerColor, int i, int j);
int testCastling(Board* board, Player* player, Player* adversary, int i, int j, int k, int l);
int castling(Board* board, int i, int j, int k, int l);
int movePiece(Board* board, int i, int j, int k, int l);
int promotion(Board* board, int i, int j);
int promotionOnline(Board* board, Player* player, int i, int j, int socket);
int capturePiece(Board* board, Player* player,int k,int l);
void updatePosition(Board* board);
int updateBoard(Board* board, Player* player, int i, int j, int k, int l, int captured1, int captured2);
int rankIndexToInt(char c);
int fileIndexToInt(char c);
void printBoard(Board* board, Color player);

#endif
