#ifndef PIECE_H
#define PIECE_H

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
	int i[100];
	int j[100];
};


int getI(Piece* piece, int counterMove);
int getLastI(Piece* piece, int counterMove);
int getJ(Piece* piece, int counterMove);
void setI(Piece* piece, int i, int counterMove);
void setJ(Piece* piece, int j, int counterMove);
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
int enPassantCapture(int i, int j, int k, int l, int* m, int* n, int* isEnPassantCapture);


#endif
