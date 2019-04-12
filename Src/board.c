#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int canMovePiece(Color player,Square board[8][8], int i, int j, int k, int l)
{
	if(board[i][j].isOccupied)
	{
		Piece* piece = board[i][j].piece;

		if( getColor(board,i,j) == player)
		{
			if( ( ( isEmpty(board,k,l) || ((getColor(board,i,j)==WHITE) &&  (getColor(board,k,l)==BLACK)) )
	           || ( isEmpty(board,k,l) || ((getColor(board,i,j)==BLACK) &&  (getColor(board,k,l)==WHITE)) ) ) )
			{
				switch(piece->name)
				{
					case KING:
						return canMoveKing(board,i,j,k,l);
						break;
					case QUEEN:
						return canMoveQueen(board,i,j,k,l);
						break;
					case BISHOP:
						return canMoveBishop(board,i,j,k,l);
						break;
					case ROOK:
						return canMoveRook(board,i,j,k,l);
						break;
					case KNIGHT:
						return canMoveKnight(board,i,j,k,l);
						break;
					case PAWN:
						return canMovePawn(board,i,j,k,l);
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

int canMoveKing(Square board[8][8], int i, int j, int k, int l)
{
	if( ( (k-i==1) &&  ( (j==l) || (l-j==1) || (l-j==-1) ) )
	||  ( (k-i==-1) && ( (j==l) || (l-j==1) || (l-j==-1) ) )
	||  ( (k-i==0) && ( (l-j==1) || (l-j==-1) ) ) )
	{
		return 1;
	}
	return 0;
}

int canMoveQueen(Square board[8][8], int i, int j, int k, int l)
{
	if(isEmptyBetween(board,i,j,k,l)
		&& ( (k-i==l-j) || (k-i==j-l) || (k-i==0) || (l-j==0) ) )
	{
		return 1;
	}
	return 0;
}

int canMoveBishop(Square board[8][8], int i, int j, int k, int l)
{
	if(isEmptyBetween(board,i,j,k,l)
	&&  ( (k-i==l-j) || (k-i==j-l) ) )
	{
		return 1;
	}
	return 0;
}

int canMoveRook(Square board[8][8], int i, int j, int k, int l)
{
	if(isEmptyBetween(board,i,j,k,l)
	&&  ( (k-i==0) || (l-j==0) ))
	{
		return 1;
	}
	return 0;
}

int canMoveKnight(Square board[8][8], int i, int j, int k, int l)
{

	if  ( ( (k-i==2) && ( (l-j==1) || (l-j==-1) ) ) || ( (k-i==-2) && ( (l-j==1) || (l-j==-1) ) )
     || ( (k-i==1) && ( (l-j==2) || (l-j==-2) ) ) || ( (k-i==-1) && ( (l-j==2) || (l-j==-2) ) ) )
     {
     	return 1;
     }
     return 0;
}

int canMovePawn(Square board[8][8], int i, int j, int k, int l)
{
	if(getColor(board,i,j) == WHITE)
	{
		if( ( (i==1) && (j==l) && (k-i==2) && isEmpty(board,i+1,l) && isEmpty(board,k,l) )
	    || ( (j==l) && (k-i == 1) && isEmpty(board,k,l) )
	    || ( (k==i+1) && ((l==j+1) || (l==j-1)) && !isEmpty(board,k,l) && (getColor(board,k,l)==BLACK) ) )
		{
			return 1;
		}
	}
	else
	{
		if( ( (i==6) && (j==l) && (k-i==-2) && isEmpty(board,i-1,l) && isEmpty(board,k,l) )
		|| ( (j==l) && (k-i==-1) && isEmpty(board,k,l) )
		|| ( (k==i-1) && ((l==j-1) || (l==j+1)) && !isEmpty(board,k,l) && (getColor(board,k,l)==BLACK) ) )
		{
			return 1;
		}
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

int isEmpty(Square board[8][8], int i,int j)
{
	return !board[i][j].isOccupied;
}

int isEmptyBetween(Square board[8][8], int i, int j, int k, int l)
{
	int n,m;
	if(k-i==0) // horizontal move
	{
		if(l-j>0) // from left to right
		{
			for(n=j+1;n<l;n++)
			{
				if(!isEmpty(board,i,n))
				{
					return 0;
				}
			}
		}
		else // from right to left
		{
			for(n=j-1;n>l;n--)
			{
				if(!isEmpty(board,i,n))
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
				if(!isEmpty(board,m,j))
				{
					return 0;
				}
			}
		}
		else // from up to down
		{
			for(m=i-1;m>k;m--)
			{
				if(!isEmpty(board,m,j))
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
				if(!isEmpty(board,i+n,j+n))
				{
					return 0;
				}
			}
		}
		else if(k-i>0 && l-j<0)
		{
			for(n=1;n<k-i;n++)
			{
				if(!isEmpty(board,i+n,j-n))
				{
					return 0;
				}
			}
		}
		else if(k-i<0 && l-j>0 )
		{
			for(n=1;n<k-i;n++)
			{
				if(!isEmpty(board,i-n,j+n))
				{
					return 0;
				}
			}
		}
		else if(k-i<0 && l-j<0)
		{
			for(n=1;n<k-i;n++)
			{
				if(!isEmpty(board,i-n,j-n))
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

Color getColor(Square board[8][8], int i, int j)
{
	return board[i][j].piece->color;
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

void initializeBoard(Square board[8][8])
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(i<2 || i>5)
			{
				board[i][j].isOccupied = 1;
				board[i][j].piece = malloc(sizeof(Piece));
				if(i<2)
				{
					board[i][j].piece->color = WHITE;
					if(i==1)
					{
						board[i][j].piece->name = PAWN;
					}
				}
				else if(i>5)
				{
					board[i][j].piece->color = BLACK;
					if(i==6)
					{
						board[i][j].piece->name = PAWN;
					}

				}
			}
			else
			{
				board[i][j].isOccupied = 0;
				board[i][j].piece = NULL;
			}
		}
	}
	board[0][0].piece->name = board[0][7].piece->name = board[7][0].piece->name = board[7][7].piece->name = ROOK;
	board[0][1].piece->name = board[0][6].piece->name = board[7][1].piece->name = board[7][6].piece->name = KNIGHT;
	board[0][2].piece->name = board[0][5].piece->name = board[7][2].piece->name = board[7][5].piece->name = BISHOP;
	board[0][3].piece->name = board[7][3].piece->name = QUEEN;
	board[0][4].piece->name = board[7][4].piece->name = KING;

}

void printfBoard(Square board[8][8],Color player)
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
				if(board[i][j].isOccupied)
				{
					printfPiece(board[i][j].piece);
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
				if(board[i][j].isOccupied)
				{
					printfPiece(board[i][j].piece);
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

void printfPiece(Piece* piece)
{
	if(piece->color == WHITE)
		{
			switch(piece->name)
			{
				case KING:
				printf("\u2654  | ");
				break;
				case QUEEN:
				printf("\u2655  | ");
				break;
				case BISHOP:
				printf("\u2657  | ");
				break;
				case ROOK:
				printf("\u2656  | ");
				break;
				case KNIGHT:
				printf("\u2658  | ");
				break;
				case PAWN:
				printf("\u2659  | ");
				break;

			}
		}
		else
		{
			switch(piece->name)
			{
				case KING:
				printf("\u265A  | ");
				break;
				case QUEEN:
				printf("\u265B  | ");
				break;
				case BISHOP:
				printf("\u265D  | ");
				break;
				case ROOK:
				printf("\u265C  | ");
				break;
				case KNIGHT:
				printf("\u265E  | ");
				break;
				case PAWN:
				printf("\u265F  | ");
				break;

			}
		}
}
