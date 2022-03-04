#ifndef PIECE_H
#define PIECE_H

typedef struct Board Board;
struct Board;

typedef enum Name Name;
enum Name
{
	KING,
	QUEEN,
	BISHOP,
	ROOK,
	KNIGHT,
	PAWN
};

typedef enum Color Color;
enum Color
{
	WHITE,
	BLACK
};

typedef struct Piece Piece;
struct Piece
{
	Color color;
	Name name;
	// position of piece for all game turns
	int posI[100]; 
	int posJ[100];
};


int getPosI(Piece* piece, int counterMove);
int getLastPosI(Piece* piece, int counterMove);
int getPosJ(Piece* piece, int counterMove);
void setPosI(Piece* piece, int i, int counterMove);
void setPosJ(Piece* piece, int j, int counterMove);
void printfPiece(Piece* piece);
int hasMoved(Piece* piece, int counterMove);
int kingMovement(int i, int j, int k, int l);
int queenMovement(int i, int j, int k, int l);
int bishopMovement(int i, int j, int k, int l);
int rookMovement(int i, int j, int k, int l);
int knightMovement(int i, int j, int k, int l);
int pawnMovement(int i, int j, int k, int l, Color color, int isCapturing);
int whitePawnStraightMovement(int i, int j, int k, int l);
int whitePawnCaptureMovement(int i, int j, int k, int l);
int blackPawnStraightMovement(int i, int j, int k, int l);
int blackPawnCaptureMovement(int i, int j, int k, int l);
int enPassantCapture(struct Board* board,int i, int j, int k, int l, int* m, int* n, int* isEnPassantCapture);


#endif
