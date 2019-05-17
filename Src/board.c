#include <stdio.h>
#include <stdlib.h>
#include "board.h"

// test if the piece can be moved
int canMovePiece(Player* player, int i, int j, int k, int l, int isTestChess)
{
	int b = 0;
	Piece* tempPiece = NULL;
	if(!isEmptySquare(i,j))
	{
		Piece* piece = cb.array[i][j].piece;
		if( ((getColor(i,j) == player->color) && !isTestChess) || ( (getColor(i,j) != player->color)  && isTestChess))
		{
			if( ( ( isEmptySquare(k,l) || ((getColor(i,j)==WHITE) &&  (getColor(k,l)==BLACK)) )
	           || ( isEmptySquare(k,l) || ((getColor(i,j)==BLACK) &&  (getColor(k,l)==WHITE)) ) ) )
			{
				switch(piece->name)
				{
					case KING:
						b = canMoveKing(i,j,k,l);
						break;
					case QUEEN:
						b = canMoveQueen(i,j,k,l);
						break;
					case BISHOP:
						b = canMoveBishop(i,j,k,l);
						break;
					case ROOK:
						b = canMoveRook(i,j,k,l);
						break;
					case KNIGHT:
						b = canMoveKnight(i,j,k,l);
						break;
					case PAWN:
						b = canMovePawn(i,j,k,l);
						break;
				}

				if(b==1 && !isTestChess)
				{
					tempPiece = cb.array[k][l].piece;
					movePiece(i,j,k,l);
					if(testChess(player))
					{
						if(cb.array[k][l].piece->color != player->color)
						{
							printf("this move would lead to chess\n");
						}
						b = 0;
					}
					movePiece(k,l,i,j);
					if(tempPiece!=NULL)
					{
						cb.array[k][l].piece = tempPiece;
						cb.array[k][l].isOccupied = 1;
					}
				}
			}
			else
			{
				printf("can't eat your own piece\n");
				return 0;
			}

		}
		else
		{
			printf("can't move this piece\n");
			return 0;
		}

	}
	else
	{
		printf("no piece on this square\n");
		return 0;
	}
	if (b==0 && !isTestChess)
	{
		printf("This movement is not allowed\n");
	}
	return b;
}


// update the board with new position of the piece
void movePiece(int i, int j, int k, int l)
{
	Piece* piece = cb.array[i][j].piece;
	cb.array[k][l].piece = piece;
	cb.array[k][l].piece->i=k;
	cb.array[k][l].piece->j=l;
	cb.array[k][l].isOccupied = 1;
	cb.array[i][j].piece = NULL;
	cb.array[i][j].isOccupied = 0;
}

// test if the king of player can be captured by the adversary
int testChess(Player* player)
{
	int i,j;
	Color color2;
	if(player->color == WHITE)
	{
		color2 = BLACK;
	}
	else
	{
		color2 = WHITE;
	}
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(cb.array[i][j].isOccupied)
			{
				if(cb.array[i][j].piece->color == color2)
				{
					if(canMovePiece(player,i,j,player->king->i,player->king->j,1))
					{
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

// test if the king of player can be unavoidably capured by the adversary
int testMat(Player* player)
{
	int i,j,k,l;
	Color color = player->color;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(cb.array[i][j].isOccupied && (cb.array[i][j].piece->color == color))
			{
				for(k=0;k<8;k++)
				{
					for(l=0;l<8;l++)
					{
						if(!cb.array[k][l].isOccupied || (cb.array[k][l].isOccupied && cb.array[k][l].piece->color != color))
						{
							if(canMovePiece(player,i,j,k,l,0))
							{
								return 0;
							}
						}
					}
				}
			}
		}
	}
	return 1;

}

// test is the square doesn't contain piece.
int isEmptySquare(int i,int j)
{
	return !cb.array[i][j].isOccupied;
}

// test if all square between initial position and final position are empty
int isEmptyBetween(int i, int j, int k, int l)
{
	int n,m;
	if(k-i==0) // horizontal move
	{
		if(l-j>0) // from left to right
		{
			for(n=j+1;n<l;n++)
			{
				if(!isEmptySquare(i,n))
				{
					return 0;
				}
			}
		}
		else // from right to left
		{
			for(n=j-1;n>l;n--)
			{
				if(!isEmptySquare(i,n))
				{
					return 0;
				}
			}
		}

	}
	else if(l-j==0) // vertical move
	{
		if(k-i>0) // from down to up
		{
			for(m=i+1;m<k;m++)
			{
				if(!isEmptySquare(m,j))
				{
					return 0;
				}
			}
		}
		else // from up to down
		{
			for(m=i-1;m>k;m--)
			{
				if(!isEmptySquare(m,j))
				{
					return 0;
				}
			}
		}

	}
	else // diagonal move
	{
		// from down/left to up/right
		if(k-i>0 && l-j>0)
		{
			for(n=1;n<k-i;n++)
			{
				if(!isEmptySquare(i+n,j+n))
				{
					return 0;
				}
			}
		}
		// from down/right to up/left
		else if(k-i>0 && l-j<0)
		{
			for(n=1;n<k-i;n++)
			{
				if(!isEmptySquare(i+n,j-n))
				{
					return 0;
				}
			}
		}
		// from up/left to down/right
		else if(k-i<0 && l-j>0 )
		{
			for(n=1;n<abs(k-i);n++)
			{
				if(!isEmptySquare(i-n,j+n))
				{
					return 0;
				}
			}
		}
		// from up/right to down/left
		else if(k-i<0 && l-j<0)
		{
			for(n=1;n<abs(k-i);n++)
			{
				if(!isEmptySquare(i-n,j-n))
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

Color getColor(int i, int j)
{
	return cb.array[i][j].piece->color;
}

// convert rank name to array columun index
int rankIndexToInt(char c)
{
	int i;
	switch(c)
	{
		case '1':
			i=0;
			break;
		case '2':
			i=1;
			break;
		case '3':
			i=2;
			break;
		case '4':
			i=3;
			break;
		case '5':
			i=4;
			break;
		case '6':
			i=5;
			break;
		case '7':
			i=6;
			break;
		case '8':
			i=7;
			break;
	}
	return i;
}

// convert file name to array line index
int fileIndexToInt(char c)
{
	int i;
	switch(c)
	{
		case 'a': case 'A':
			i=0;
			break;
		case 'b': case 'B':
			i=1;
			break;
		case 'c': case 'C':
			i=2;
			break;
		case 'd': case 'D':
			i=3;
			break;
		case 'e': case 'E':
			i=4;
			break;
		case 'f': case 'F':
			i=5;
			break;
		case 'g': case 'G':
			i=6;
			break;
		case 'h': case 'H':
			i=7;
			break;
	}
	return i;
}

// initialize game pieces and place them on the board
void initializeBoard(Player* player1, Player* player2)
{
	int i,j;


	Piece* whiteKing = malloc(sizeof(Piece));
	whiteKing->color = WHITE;
	whiteKing->name = KING;
	whiteKing->i= 0;
	whiteKing->j= 4;
	Piece* whiteQueen = malloc(sizeof(Piece));
	whiteQueen->color = WHITE;
	whiteQueen->name = QUEEN;
	whiteQueen->i= 0;
	whiteQueen->j= 3;
	Piece* whiteRook1 = malloc(sizeof(Piece));
	whiteRook1->color = WHITE;
	whiteRook1->name = ROOK;
	whiteRook1->i= 0;
	whiteRook1->j= 0;
	Piece* whiteRook2 = malloc(sizeof(Piece));
	whiteRook2->color = WHITE;
	whiteRook2->name = ROOK;
	whiteRook2->i= 0;
	whiteRook2->j= 7;
	Piece* whiteKnight1 = malloc(sizeof(Piece));
	whiteKnight1->color = WHITE;
	whiteKnight1->name = KNIGHT;
	whiteKnight1->i= 0;
	whiteKnight1->j= 1;
	Piece* whiteKnight2 = malloc(sizeof(Piece));
	whiteKnight2->color = WHITE;
	whiteKnight2->name = KNIGHT;
	whiteKnight2->i= 0;
	whiteKnight2->j= 6;
	Piece* whiteBishop1 = malloc(sizeof(Piece));
	whiteBishop1->color = WHITE;
	whiteBishop1->name = BISHOP;
	whiteBishop1->i= 0;
	whiteBishop1->j= 2;
	Piece* whiteBishop2 = malloc(sizeof(Piece));
	whiteBishop2->color = WHITE;
	whiteBishop2->name = BISHOP;
	whiteBishop2->i= 0;
	whiteBishop2->j= 5;
	Piece* whitePawn1 = malloc(sizeof(Piece));
	whitePawn1->color = WHITE;
	whitePawn1->name = PAWN;
	whitePawn1->i= 1;
	whitePawn1->j= 0;
	Piece* whitePawn2 = malloc(sizeof(Piece));
	whitePawn2->color = WHITE;
	whitePawn2->name = PAWN;
	whitePawn2->i= 1;
	whitePawn2->j= 1;
	Piece* whitePawn3 = malloc(sizeof(Piece));
	whitePawn3->color = WHITE;
	whitePawn3->name = PAWN;
	whitePawn3->i= 1;
	whitePawn3->j= 2;
	Piece* whitePawn4 = malloc(sizeof(Piece));
	whitePawn4->color = WHITE;
	whitePawn4->name = PAWN;
	whitePawn4->i= 1;
	whitePawn4->j= 3;
	Piece* whitePawn5 = malloc(sizeof(Piece));
	whitePawn5->color = WHITE;
	whitePawn5->name = PAWN;
	whitePawn5->i= 1;
	whitePawn5->j= 4;
	Piece* whitePawn6 = malloc(sizeof(Piece));
	whitePawn6->color = WHITE;
	whitePawn6->name = PAWN;
	whitePawn6->i= 1;
	whitePawn6->j= 5;
	Piece* whitePawn7 = malloc(sizeof(Piece));
	whitePawn7->color = WHITE;
	whitePawn7->name = PAWN;
	whitePawn7->i= 1;
	whitePawn7->j= 6;
	Piece* whitePawn8 = malloc(sizeof(Piece));
	whitePawn8->color = WHITE;
	whitePawn8->name = PAWN;
	whitePawn8->i= 1;
	whitePawn8->j= 7;

	Piece* blackKing = malloc(sizeof(Piece));
	blackKing->color = BLACK;
	blackKing->name = KING;
	blackKing->i= 7;
	blackKing->j= 4;
	Piece* blackQueen = malloc(sizeof(Piece));
	blackQueen->color = BLACK;
	blackQueen->name = QUEEN;
	blackQueen->i= 7;
	blackQueen->j= 3;
	Piece* blackRook1 = malloc(sizeof(Piece));
	blackRook1->color = BLACK;
	blackRook1->name = ROOK;
	blackRook1->i= 7;
	blackRook1->j= 0;
	Piece* blackRook2 = malloc(sizeof(Piece));
	blackRook2->color = BLACK;
	blackRook2->name = ROOK;
	blackRook2->i= 7;
	blackRook2->j= 7;
	Piece* blackKnight1 = malloc(sizeof(Piece));
	blackKnight1->color = BLACK;
	blackKnight1->name = KNIGHT;
	blackKnight1->i= 7;
	blackKnight1->j= 1;
	Piece* blackKnight2 = malloc(sizeof(Piece));
	blackKnight2->color = BLACK;
	blackKnight2->name = KNIGHT;
	blackKnight2->i= 7;
	blackKnight2->j= 6;
	Piece* blackBishop1 = malloc(sizeof(Piece));
	blackBishop1->color = BLACK;
	blackBishop1->name = BISHOP;
	blackBishop1->i= 7;
	blackBishop1->j= 2;
	Piece* blackBishop2 = malloc(sizeof(Piece));
	blackBishop2->color = BLACK;
	blackBishop2->name = BISHOP;
	blackBishop2->i= 7;
	blackBishop2->j= 5;
	Piece* blackPawn1 = malloc(sizeof(Piece));
	blackPawn1->color = BLACK;
	blackPawn1->name = PAWN;
	blackPawn1->i= 6;
	blackPawn1->j= 0;
	Piece* blackPawn2 = malloc(sizeof(Piece));
	blackPawn2->color = BLACK;
	blackPawn2->name = PAWN;
	blackPawn2->i= 6;
	blackPawn2->j= 1;
	Piece* blackPawn3 = malloc(sizeof(Piece));
	blackPawn3->color = BLACK;
	blackPawn3->name = PAWN;
	blackPawn3->i= 6;
	blackPawn3->j= 2;
	Piece* blackPawn4 = malloc(sizeof(Piece));
	blackPawn4->color = BLACK;
	blackPawn4->name = PAWN;
	blackPawn4->i= 6;
	blackPawn4->j= 3;
	Piece* blackPawn5 = malloc(sizeof(Piece));
	blackPawn5->color = BLACK;
	blackPawn5->name = PAWN;
	blackPawn5->i= 6;
	blackPawn5->j= 4;
	Piece* blackPawn6 = malloc(sizeof(Piece));
	blackPawn6->color = BLACK;
	blackPawn6->name = PAWN;
	blackPawn6->i= 6;
	blackPawn6->j= 5;
	Piece* blackPawn7 = malloc(sizeof(Piece));
	blackPawn7->color = BLACK;
	blackPawn7->name = PAWN;
	blackPawn7->i= 6;
	blackPawn7->j= 6;
	Piece* blackPawn8 = malloc(sizeof(Piece));
	blackPawn8->color = BLACK;
	blackPawn8->name = PAWN;
	blackPawn8->i= 6;
	blackPawn8->j= 7;

	// initialize board squares
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(i<2 || i>5)
			{
				cb.array[i][j].isOccupied = 1;
			}
			else
			{
				cb.array[i][j].isOccupied = 0;
				cb.array[i][j].piece = NULL;
			}
		}
	}

	cb.array[0][0].piece = whiteRook1;
	cb.array[0][7].piece = whiteRook2;
	cb.array[7][0].piece = blackRook1;
	cb.array[7][7].piece = blackRook2;
	cb.array[0][1].piece = whiteKnight1;
	cb.array[0][6].piece = whiteKnight2;
	cb.array[7][1].piece = blackKnight1;
	cb.array[7][6].piece = blackKnight2;
	cb.array[0][2].piece = whiteBishop1;
	cb.array[0][5].piece = whiteBishop2;
	cb.array[7][2].piece = blackBishop1;
	cb.array[7][5].piece = blackBishop2;
	cb.array[0][3].piece = whiteQueen;
	cb.array[7][3].piece = blackQueen;
	cb.array[0][4].piece = whiteKing;
	cb.array[7][4].piece = blackKing;
	cb.array[1][0].piece = whitePawn1;
	cb.array[1][1].piece = whitePawn2;
	cb.array[1][2].piece = whitePawn3;
	cb.array[1][3].piece = whitePawn4;
	cb.array[1][4].piece = whitePawn5;
	cb.array[1][5].piece = whitePawn6;
	cb.array[1][6].piece = whitePawn7;
	cb.array[1][7].piece = whitePawn8;
	cb.array[6][0].piece = blackPawn1;
	cb.array[6][1].piece = blackPawn2;
	cb.array[6][2].piece = blackPawn3;
	cb.array[6][3].piece = blackPawn4;
	cb.array[6][4].piece = blackPawn5;
	cb.array[6][5].piece = blackPawn6;
	cb.array[6][6].piece = blackPawn7;
	cb.array[6][7].piece = blackPawn8;


	if(player1->color == WHITE)
	{
		player1->king = whiteKing;
		player2->king = blackKing;
	}
	else
	{
		player1->king = blackKing;
		player2->king = whiteKing;
	}

}

// print board in console
void printfBoard(Color player)
{

	int i,j;
	char rankIndex[8] = {'1','2','3','4','5','6','7','8'};
	char filesIndex[8] = {'a','b','c','d','e','f','g','h'};
	if (player == WHITE)
	{
		printf("\n  |----|----|----|----|----|----|----|----|\n");
		for(i=7;i>=0;i--)
		{
			printf("%c | ",rankIndex[i]);
			for(j=0;j<8;j++)
			{
				if(cb.array[i][j].isOccupied)
				{
					printfPiece(cb.array[i][j].piece);
					printf("  | ");
				}
				else
				{
					printf("   | ");
				}
			}
			printf("\n  |----|----|----|----|----|----|----|----|\n");

		}
		printf("    ");
		for(i=0;i<8;i++)
		{
			printf("%c    ",filesIndex[i]);
		}
		printf("\n");
	}
	else
	{
		printf("\n  |----|----|----|----|----|----|----|----|\n");
		for(i=0;i<8;i++)
		{
			printf("%c | ",rankIndex[i]);
			for(j=7;j>=0;j--)
			{
				if(cb.array[i][j].isOccupied)
				{
					printfPiece(cb.array[i][j].piece);
					printf("  | ");
				}
				else
				{
					printf("   | ");
				}
			}
			printf("\n  |----|----|----|----|----|----|----|----|\n");

		}
		printf("    ");
		for(i=7;i>=0;i--)
		{
			printf("%c    ",filesIndex[i]);
		}
		printf("\n");
	}
}


