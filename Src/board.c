#include <stdio.h>
#include <stdlib.h>
#include "board.h"

// test if the piece can be moved
int canMovePiece(Player* player, int i, int j, int k, int l, int* m, int* n, int isTestChess, int isTestMat)
{
	if( i < 0 || j < 0 || k < 0 || l < 0 || i > 7 || j > 7 || k > 7 || l > 7 )
	{
		exit (-1);
	}
	int allowedMovement = 0;
	int isEnPassantCapture = 0;
	Piece* tempPiece = NULL;
	if(isEmptySquare(i,j))
	{
		if(!isTestChess && !isTestMat)
		{
			printf("No piece on this square\n");
		}
		return 0;
	}

	Piece* piece = cb.array[i][j].piece;
	if( ((getColor(i,j) != player->color) && !isTestChess) || ( (getColor(i,j) == player->color)  && isTestChess))
	{
		if(!isTestChess && !isTestMat)
		{
			printf("Can't move this piece\n");
		}
		return 0;
	}

	if( !isEmptySquare(k,l) && (getColor(i,j) ==  getColor(k,l)) )
	{
		if(!isTestChess && !isTestMat)
		{
			printf("Can't eat your own piece\n");
		}
		return 0;
	}

	switch(piece->name)
	{
		case KING:
			allowedMovement = canMoveKing(i,j,k,l);
			break;
		case QUEEN:
			allowedMovement = canMoveQueen(i,j,k,l);
			break;
		case BISHOP:
			allowedMovement = canMoveBishop(i,j,k,l);
			break;
		case ROOK:
			allowedMovement = canMoveRook(i,j,k,l);
			break;
		case KNIGHT:
			allowedMovement = canMoveKnight(i,j,k,l);
			break;
		case PAWN:
			allowedMovement = (canMovePawn(i,j,k,l) || enPassantCapture(i,j,k,l,m,n, &isEnPassantCapture));
			break;
	}


	if(!allowedMovement)
	{
		if(!isTestChess && !isTestMat)
		{
			printf("This movement is not allowed\n");
		}
		return 0;
	}
	else if(!isTestChess)
	{
		// test if the move lead to chess
		int chess;

		if(!isEnPassantCapture)
		{
			*m = k;
			*n = l;
		}
		tempPiece = cb.array[*m][*n].piece;
		cb.array[*m][*n].piece = NULL;
		cb.array[*m][*n].isOccupied = 0;
		movePiece(i,j,k,l);
		setI(cb.array[k][l].piece, k, cb.counterMove);
		setJ(cb.array[k][l].piece, l, cb.counterMove);
		chess = testChess(player);
		if(chess && !isTestMat)
		{
			printf("This move would lead to chess\n");
		}

		movePiece(k,l,i,j);
		setI(cb.array[i][j].piece, i, cb.counterMove);
		setJ(cb.array[i][j].piece, j, cb.counterMove);
		if(tempPiece!=NULL)
		{
			cb.array[*m][*n].piece = tempPiece;
			cb.array[*m][*n].isOccupied = 1;
		}
		return !chess;
	}
	else
	{
		return 1;
	}
}

// test if the castle movement is possible
int testCastling(Player* player, int i, int j, int k, int l)
{
	if( isEmptySquare(i,j) || isEmptySquare(k,l) || (getColor(i,j) != player->color)  || (getColor(i,j) != player->color)
		|| hasMoved(getPiece(i,j), cb.counterMove) || hasMoved(getPiece(k,l), cb.counterMove) )
	{
		return 0;
	}
	if(getColor(i,j) == WHITE)
	{
		if( i==0 && j==4 && k==0 && (l==0 || l==7 ) )
		{
			if(l==0 && !canBeEaten(player,0,3) && !canBeEaten(player,0,2) && isEmptyBetween(0,4,0,0))
			{
				return 1;
			}
			else if(l==7 && !canBeEaten(player,0,5) && !canBeEaten(player,0,6) && isEmptyBetween(0,5,0,7))
			{
				return 1;
			}
		}
	}
	else
	{
		if(i==7 && j==4 && k==7 && (l==0 || l==7 ) )
		{
			if(l==0 && !canBeEaten(player,7,3) && !canBeEaten(player,7,2) && isEmptyBetween(7,4,7,0))
			{
				return 1;
			}
			else if(l==7 && !canBeEaten(player,7,5) && !canBeEaten(player,7,6) && isEmptyBetween(7,5,7,7))
			{
				return 1;
			}
		}
	}
	return 0;
}


// update the board with new position of the piece
int movePiece(int i, int j, int k, int l)
{
	if( i < 0 || j < 0 || k < 0 || l < 0 || i > 7 || j > 7 || k > 7 || l > 7 )
	{
		return -1;
	}
	Piece* piece = cb.array[i][j].piece;
	cb.array[k][l].piece = piece;
	cb.array[k][l].isOccupied = 1;
	cb.array[i][j].piece = NULL;
	cb.array[i][j].isOccupied = 0;
	return 0;
}

// execute castle movement
int castling(int i, int j, int k, int l)
{
	if( i < 0 || j < 0 || k < 0 || l < 0 || i > 7 || j > 7 || k > 7 || l > 7 )
	{
		return -1;
	}
	if(getColor(i,j) == WHITE)
	{
		if(l==0)
		{
			movePiece(0,4,0,2);
			movePiece(0,0,0,3);
		}
		else
		{
			movePiece(0,4,0,6);
			movePiece(0,7,0,5);
		}
	}
	else
	{
		if(l==0)
		{
			movePiece(7,4,7,2);
			movePiece(7,0,7,3);
		}
		else
		{
			movePiece(7,4,7,6);
			movePiece(7,7,7,5);
		}
	}
	return 0;
}

// return 1 and change piece name if the pawn can be promoted
int promotion(int i, int j)
{
	int selectedPiece = 0;
	if(getName(i,j)!=PAWN || i!=7 )
	{
		return 0;
	}
	else
	{
		do
		{
			printf("Choose the new piece\n");
			printf("1. Bishop\n");
			printf("2. Knight\n");
			printf("3. Rook\n");
			printf("4. Queen\n");
			readInt(&selectedPiece);

		}while(selectedPiece < 1 || selectedPiece > 4 );
		switch(selectedPiece)
		{
			case 1:
			setName(i,j,BISHOP);
			break;
			case 2:
			setName(i,j,KNIGHT);
			break;
			case 3:
			setName(i,j,ROOK);
			break;
			case 4:
			setName(i,j,QUEEN);
		}
		return 1;
	}
}

void updatePosition( int counterMove)
{
	int i, j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(!isEmptySquare(i,j))
			{
				setI(cb.array[i][j].piece, i, counterMove);
				setJ(cb.array[i][j].piece, j, counterMove);
			}
		}
	}
}

// test if the king of player can be captured by the adversary
int testChess(Player* player)
{
	return canBeEaten(player, player->king->i[cb.counterMove], player->king->j[cb.counterMove]);
}

// test if the Piece on the position (k,l) of player can be captured by the adversary
int canBeEaten(Player* player, int k, int l)
{
	int i,j;

	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(canMovePiece(player,i,j,k,l,NULL,NULL,1,0))
			{
				return 1;
			}
		}
	}
	return 0;
}

// test if the king of player can be unavoidably capured by the adversary
int testMat(Player* player)
{
	int i,j,k,l,m=0,n=0;
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
						if(canMovePiece(player,i,j,k,l,&m,&n,0,1))
						{
							return 0;
						}
					}
				}
			}
		}
	}
	return 1;
}

Piece* getPiece(int i, int j)
{
	return cb.array[i][j].piece;
}

Name getName(int i, int j)
{
	return getPiece(i,j)->name;
}

void setName(int i, int j, Name name)
{
	getPiece(i,j)->name = name;
}

// test is the square doesn't contain piece.
int isEmptySquare(int i,int j)
{
	if( i < 0|| j < 0 || i > 7 || j > 7 )
	{
		return -1;
	}
	return !cb.array[i][j].isOccupied;
}

// test if all square between initial position and final position are empty
int isEmptyBetween(int i, int j, int k, int l)
{
	int n,m;
	if( i < 0 || j < 0 || k < 0 || l < 0 || i > 7 || j > 7 || k > 7 || l > 7 )
	{
		return -1;
	}
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
	if( i < 0|| j < 0 || i > 7 || j > 7 )
	{
		return -1;
	}
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
		default:
			i = -1;
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
		default:
			i = -1;
	}
	return i;
}

// initialize game pieces and place them on the board
int initializeBoard(Player* player1, Player* player2)
{
	int i,j;
	// initialize board squares
	cb.counterMove=0;
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

	// place pieces on the board
	for(i=0;i<8;i++)
	{
		if(player1->color == WHITE)
		{
			cb.array[0][i].piece = player1->pieces[i];
			cb.array[7][i].piece = player2->pieces[i];
		}
		else
		{
			cb.array[0][i].piece = player2->pieces[i];
			cb.array[7][i].piece = player1->pieces[i];
		}
	}

	for(i=8;i<16;i++)
	{
		if(player1->color == WHITE)
		{
			cb.array[1][i - 8].piece = player1->pieces[i];
			cb.array[6][i - 8].piece = player2->pieces[i];
		}
		else
		{
			cb.array[1][i - 8].piece = player2->pieces[i];
			cb.array[6][i - 8].piece = player1->pieces[i];
		}

	}

	return 0;
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


