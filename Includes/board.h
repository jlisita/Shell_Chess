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

typedef struct ChessBoard ChessBoard;
struct ChessBoard
{
	Square array[8][8];
}cb;

void initializeBoard();
void printfBoard(Color player);
void printfPiece(Piece* piece);
int rankIndexToInt(char c);
int fileIndexToInt(char c);
int isEmptySquare(int i,int j);
int isEmptyBetween(int i, int j, int k, int l);
Color getColor(int i, int j);
int canMovePiece(Color player,Square board[8][8], int i, int j, int k, int l);
int canMoveKing(int i, int j, int k, int l);
int canMoveQueen(int i, int j, int k, int l);
int canMoveBishop(int i, int j, int k, int l);
int canMoveRook(int i, int j, int k, int l);
int canMoveKnight(int i, int j, int k, int l);
int canMovePawn(int i, int j, int k, int l);
void movePiece(Square board[8][8], int i, int j, int k, int l);


#endif
