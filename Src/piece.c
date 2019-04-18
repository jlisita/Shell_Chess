#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int canMoveKing(int i, int j, int k, int l)
{
	if( ( (k-i==1) &&  ( (j==l) || (l-j==1) || (l-j==-1) ) )
	||  ( (k-i==-1) && ( (j==l) || (l-j==1) || (l-j==-1) ) )
	||  ( (k-i==0) && ( (l-j==1) || (l-j==-1) ) ) )
	{
		return 1;
	}
	return 0;
}

int canMoveQueen(int i, int j, int k, int l)
{
	if(isEmptyBetween(i,j,k,l)
		&& ( (k-i==l-j) || (k-i==j-l) || (k-i==0) || (l-j==0) ) )
	{
		return 1;
	}
	return 0;
}

int canMoveBishop(int i, int j, int k, int l)
{
	if(isEmptyBetween(i,j,k,l)
	&&  ( (k-i==l-j) || (k-i==j-l) ) )
	{
		return 1;
	}
	return 0;
}

int canMoveRook(int i, int j, int k, int l)
{
	if(isEmptyBetween(i,j,k,l)
	&&  ( (k-i==0) || (l-j==0) ))
	{
		return 1;
	}
	return 0;
}

int canMoveKnight(int i, int j, int k, int l)
{

	if  ( ( (k-i==2) && ( (l-j==1) || (l-j==-1) ) ) || ( (k-i==-2) && ( (l-j==1) || (l-j==-1) ) )
     || ( (k-i==1) && ( (l-j==2) || (l-j==-2) ) ) || ( (k-i==-1) && ( (l-j==2) || (l-j==-2) ) ) )
     {
     	return 1;
     }
     return 0;
}

int canMovePawn(int i, int j, int k, int l)
{
	if(getColor(i,j) == WHITE)
	{
		if( ( (i==1) && (j==l) && (k-i==2) && isEmptySquare(i+1,l) && isEmptySquare(k,l) )
	    || ( (j==l) && (k-i == 1) && isEmptySquare(k,l) )
	    || ( (k==i+1) && ((l==j+1) || (l==j-1)) && !isEmptySquare(k,l) && (getColor(k,l)==BLACK) ) )
		{
			return 1;
		}
	}
	else
	{
		if( ( (i==6) && (j==l) && (k-i==-2) && isEmptySquare(i-1,l) && isEmptySquare(k,l) )
		|| ( (j==l) && (k-i==-1) && isEmptySquare(k,l) )
		|| ( (k==i-1) && ((l==j-1) || (l==j+1)) && !isEmptySquare(k,l) && (getColor(k,l)==BLACK) ) )
		{
			return 1;
		}
	}
	return 0;

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
