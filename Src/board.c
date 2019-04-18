#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int canMovePiece(Color player,Square board[8][8], int i, int j, int k, int l)
{
	if(board[i][j].isOccupied)
	{
		Piece* piece = board[i][j].piece;

		if( getColor(i,j) == player)
		{
			if( ( ( isEmptySquare(k,l) || ((getColor(i,j)==WHITE) &&  (getColor(k,l)==BLACK)) )
	           || ( isEmptySquare(k,l) || ((getColor(i,j)==BLACK) &&  (getColor(k,l)==WHITE)) ) ) )
			{
				switch(piece->name)
				{
					case KING:
						return canMoveKing(i,j,k,l);
						break;
					case QUEEN:
						return canMoveQueen(i,j,k,l);
						break;
					case BISHOP:
						return canMoveBishop(i,j,k,l);
						break;
					case ROOK:
						return canMoveRook(i,j,k,l);
						break;
					case KNIGHT:
						return canMoveKnight(i,j,k,l);
						break;
					case PAWN:
						return canMovePawn(i,j,k,l);
						break;
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
	{
		printf("no piece on this square\n");
		return 0;
	}
	return 0;
}



void movePiece(Square board[8][8], int i, int j, int k, int l)
{
	Piece* piece = board[i][j].piece;
	board[k][l].piece = piece;
	board[k][l].isOccupied = 1;
	board[i][j].piece = NULL;
	board[i][j].isOccupied = 0;
}

int isEmptySquare(int i,int j)
{
	return !cb.array[i][j].isOccupied;
}

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
		else if(k-i<0 && l-j>0 )
		{
			for(n=1;n<k-i;n++)
			{
				if(!isEmptySquare(i-n,j+n))
				{
					return 0;
				}
			}
		}
		else if(k-i<0 && l-j<0)
		{
			for(n=1;n<k-i;n++)
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

void initializeBoard()
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(i<2 || i>5)
			{
				cb.array[i][j].isOccupied = 1;
				cb.array[i][j].piece = malloc(sizeof(Piece));
				if(i<2)
				{
					cb.array[i][j].piece->color = WHITE;
					if(i==1)
					{
						cb.array[i][j].piece->name = PAWN;
					}
				}
				else if(i>5)
				{
					cb.array[i][j].piece->color = BLACK;
					if(i==6)
					{
						cb.array[i][j].piece->name = PAWN;
					}

				}
			}
			else
			{
				cb.array[i][j].isOccupied = 0;
				cb.array[i][j].piece = NULL;
			}
		}
	}
	cb.array[0][0].piece->name = cb.array[0][7].piece->name = cb.array[7][0].piece->name = cb.array[7][7].piece->name = ROOK;
	cb.array[0][1].piece->name = cb.array[0][6].piece->name = cb.array[7][1].piece->name = cb.array[7][6].piece->name = KNIGHT;
	cb.array[0][2].piece->name = cb.array[0][5].piece->name = cb.array[7][2].piece->name = cb.array[7][5].piece->name = BISHOP;
	cb.array[0][3].piece->name = cb.array[7][3].piece->name = QUEEN;
	cb.array[0][4].piece->name = cb.array[7][4].piece->name = KING;

}

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


