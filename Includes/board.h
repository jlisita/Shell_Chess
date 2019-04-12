#ifndef BOARD_H
#define BOARD_H

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
	BLACK,
	WHITE
};

typedef struct Piece Piece;
struct Piece
{
	Color color;
	Name name;
};

typedef struct Square Square;
struct Square
{
	int isOccupied;
	Piece* piece;
};

void initializeBoard(Square board[8][8]);
void printfBoard(Square board[8][8], Color player);
void printfPiece(Piece* piece);
int rankIndexToInt(char c);
int fileIndexToInt(char c);
int isEmpty(Square board[8][8],int i,int j);
int isEmptyBetween(Square board[8][8],int i, int j, int k, int l);
Color getColor(Square board[8][8],int i, int j);
int canMovePiece(Color player,Square board[8][8], int i, int j, int k, int l);
int canMoveKing(Square board[8][8], int i, int j, int k, int l);
int canMoveQueen(Square board[8][8], int i, int j, int k, int l);
int canMoveBishop(Square board[8][8], int i, int j, int k, int l);
int canMoveRook(Square board[8][8], int i, int j, int k, int l);
int canMoveKnight(Square board[8][8], int i, int j, int k, int l);
int canMovePawn(Square board[8][8], int i, int j, int k, int l);
void movePiece(Square board[8][8], int i, int j, int k, int l);


#endif
