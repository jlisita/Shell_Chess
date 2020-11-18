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
int canMoveKing(int i, int j, int k, int l);
int canMoveQueen(int i, int j, int k, int l);
int canMoveBishop(int i, int j, int k, int l);
int canMoveRook(int i, int j, int k, int l);
int canMoveKnight(int i, int j, int k, int l);
int canMovePawn(int i, int j, int k, int l);
int enPassantCapture(int i, int j, int k, int l, int* m, int* n, int* isEnPassantCapture);


#endif
