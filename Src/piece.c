#include <stdio.h>
#include <stdlib.h>
#include "piece.h"
#include "board.h"

int getRaw(Piece* piece, int counterMove)
{
	return piece->raw[counterMove];
}

int getLastRaw(Piece* piece, int counterMove)
{
	int i = piece->raw[counterMove];
	int k;
	for(k=counterMove-1; k>=0; k--)
	{
		if(piece->raw[k]!=i)
		{
			return piece->raw[k];
		}
	}
	return i;
}

int getColumn(Piece* piece, int counterMove)
{
	return piece->column[counterMove];
}

void setRaw(Piece* piece, int i, int counterMove)
{
	piece->raw[counterMove] = i;
}

void setColumn(Piece* piece, int j, int counterMove)
{
	piece->column[counterMove] = j;
}

int hasMoved(Piece* piece, int counterMove)
{
	int k;
	int i = getRaw(piece, 0);
	int j = getColumn(piece, 0);
	for(k=0;k<=counterMove;k++)
	{
		if(getRaw(piece,k)!=i || getColumn(piece,k)!=j)
		{
			return 1;
		}
	}

	return 0;
}

// test if the king can be moved
int kingMovement(int i, int j, int k, int l)
{
	if( ( (k-i==1) &&  ( (j==l) || (l-j==1) || (l-j==-1) ) )
	||  ( (k-i==-1) && ( (j==l) || (l-j==1) || (l-j==-1) ) )
	||  ( (k-i==0) && ( (l-j==1) || (l-j==-1) ) ) )
	{
		return 1;
	}
	return 0;
}

// test if the queen can be moved
int queenMovement(int i, int j, int k, int l)
{
	if( (k-i==l-j) || (k-i==j-l) || (k-i==0) || (l-j==0) )
	{
		return 1;
	}
	return 0;
}

// test if the bishop can be moved
int bishopMovement(int i, int j, int k, int l)
{
	if( (k-i==l-j) || (k-i==j-l) )
	{
		return 1;
	}
	return 0;
}

// test if the rook can be moved
int rookMovement(int i, int j, int k, int l)
{
	if( (k-i==0) || (l-j==0) )
	{
		return 1;
	}
	return 0;
}

// test if the knigth can be moved
int knightMovement(int i, int j, int k, int l)
{

	if  ( ( (k-i==2) && ( (l-j==1) || (l-j==-1) ) ) || ( (k-i==-2) && ( (l-j==1) || (l-j==-1) ) )
     || ( (k-i==1) && ( (l-j==2) || (l-j==-2) ) ) || ( (k-i==-1) && ( (l-j==2) || (l-j==-2) ) ) )
     {
     	return 1;
     }
     return 0;
}

// test if the pawn can be moved
int pawnMovement(int i, int j, int k, int l, Color color, int isCapturing)
{
	if(color == WHITE)
		{
		if(isCapturing && whitePawnCaptureMovement(i, j, k, l))
		{
			return 1;
		}
		else if(!isCapturing && whitePawnStraightMovement(i, j, k, l))
		{
			return 1;
		}
	}
	else
	{
		if(isCapturing && blackPawnCaptureMovement(i, j, k, l))
		{
			return 1;
		}
		else if(!isCapturing && blackPawnStraightMovement(i, j, k, l))
		{
			return 1;
		}
	}
		return 0;
}

int whitePawnStraightMovement(int i, int j, int k, int l)
{
	if( ( (i==1) && (j==l) && (k-i==2))
    || ( (j==l) && (k-i == 1) ) )
	{
		return 1;
	}
	return 0;

}

int whitePawnCaptureMovement(int i, int j, int k, int l)
{
	if( (k==i+1) && ((l==j+1) || (l==j-1)) )
	{
		return 1;
	}
	return 0;
}

int blackPawnStraightMovement(int i, int j, int k, int l)
{
	if( ( (i==6) && (j==l) && (k-i==-2) )
	|| ( (j==l) && (k-i==-1) ) )
	{
		return 1;
	}
	return 0;
}

int blackPawnCaptureMovement(int i, int j, int k, int l)
{
	if( (k==i-1) && ((l==j-1) || (l==j+1)) )
	{
		return 1;
	}
	return 0;
}

int enPassantCapture(Board* board, int i, int j, int k, int l, int* m, int* n, int* isEnPassantCapture)
{
	if(getColor(board,i,j) == WHITE)
	{
	 	if( (k==i+1) && ((l==j+1) || (l==j-1)) && isEmptySquare(board,k,l) && !isEmptySquare(board,k,j) && (getColor(board,k,j)==BLACK) && (getName(board,k,j)==PAWN)
	 		&& (getRaw(getPiece(board,k,j),board->counterMove)==4) && ( getLastRaw(getPiece(board,k,j),board->counterMove)==6))
	 	{
	 		*m = k;
	 		*n = j;
	 		*isEnPassantCapture = 1;
	 		return 1;
	 	}
	}
	else
	{
		if( (k==i-1) && ((l==j+1) || (l==j-1)) && isEmptySquare(board,k,l) && !isEmptySquare(board,k,j) && (getColor(board,k,j)==WHITE) && (getName(board,k,j)==PAWN)
		  && (getRaw(getPiece(board,k,j),board->counterMove) == 3) && ( getLastRaw(getPiece(board,k,j),board->counterMove)==1) )
		{
			*m = k;
	 		*n = j;
	 		*isEnPassantCapture = 1;
			return 1;
		}
	}

	return 0;
}


// print piece using unicode point in console
void printfPiece(Piece* piece)
{
	if(piece->color == WHITE)
		{
			switch(piece->name)
			{
				case KING:
				printf("\u2654");
				break;
				case QUEEN:
				printf("\u2655");
				break;
				case BISHOP:
				printf("\u2657");
				break;
				case ROOK:
				printf("\u2656");
				break;
				case KNIGHT:
				printf("\u2658");
				break;
				case PAWN:
				printf("\u2659");
				break;
			}
		}
		else
		{
			switch(piece->name)
			{
				case KING:
				printf("\u265A");
				break;
				case QUEEN:
				printf("\u265B");
				break;
				case BISHOP:
				printf("\u265D");
				break;
				case ROOK:
				printf("\u265C");
				break;
				case KNIGHT:
				printf("\u265E");
				break;
				case PAWN:
				printf("\u265F");
				break;
			}
		}
}
