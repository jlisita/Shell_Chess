#include <stdio.h>
#include <stdlib.h>
#include "board.h"

// test if the piece can be moved
int canMovePiece(Player* player, int i, int j, int k, int l, int isTestChess, int isTestMat)
{
	if( i < 0 || j < 0 || k < 0 || l < 0 || i > 7 || j > 7 || k > 7 || l > 7 )
	{
		return -1;
	}
	int allowedMovement = 0, avoidChess = 0;
	Piece* tempPiece = NULL;
	if(isEmptySquare(i,j))
	{
		printf("No piece on this square\n");
		return 0;
	}

	Piece* piece = cb.array[i][j].piece;
	if( ((getColor(i,j) != player->color) && !isTestChess) || ( (getColor(i,j) == player->color)  && isTestChess))
	{
		printf("Can't move this piece\n");
		return 0;
	}

	if( (!isEmptySquare(k,l) && getColor(i,j) == WHITE && getColor(k,l) == WHITE)
		|| (!isEmptySquare(k,l) && getColor(i,j) == BLACK && getColor(k,l) == BLACK) )
	{
		printf("Can't eat your own piece\n");
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
			allowedMovement = canMovePawn(i,j,k,l);
			break;
	}

	if(allowedMovement==0)
	{
		if(!isTestChess && !isTestMat)
		{
			printf("This movement is not allowed\n");
		}
		return 0;
	}
	else if(!isTestChess)
	{
		int ret;
		tempPiece = cb.array[k][l].piece;
		movePiece(i,j,k,l);
		ret = testChess(player);
		if(ret == -1)
		{
			return -1;
		}
		else
		{
			avoidChess = !ret;
		}
		if(!avoidChess && !isTestMat)
		{
			printf("This move would lead to chess\n");
		}

		movePiece(k,l,i,j);
		if(tempPiece!=NULL)
		{
			cb.array[k][l].piece = tempPiece;
			cb.array[k][l].isOccupied = 1;
		}
		return avoidChess;
	}
	else
	{
		return 1;
	}
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
	cb.array[k][l].piece->i=k;
	cb.array[k][l].piece->j=l;
	cb.array[k][l].isOccupied = 1;
	cb.array[i][j].piece = NULL;
	cb.array[i][j].isOccupied = 0;
	return 0;
}

// test if the king of player can be captured by the adversary
int testChess(Player* player)
{
	int i,j,ret;
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
			if(cb.array[i][j].isOccupied && cb.array[i][j].piece->color == color2
				&& (ret=canMovePiece(player,i,j,player->king->i,player->king->j,1,0)))
			{
				if(ret==-1)
				{
					return -1;
				}
				else
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

// test if the king of player can be unavoidably capured by the adversary
int testMat(Player* player)
{
	int i,j,k,l,ret;
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
						if((!cb.array[k][l].isOccupied
							|| (cb.array[k][l].isOccupied && cb.array[k][l].piece->color != color))
							&& (ret=canMovePiece(player,i,j,k,l,0,1)))
						{
							if(ret==-1)
							{
								return-1;
							}
							else
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

	Piece *whitePieces[16];
	Piece *blackPieces[16];

	for(i=0;i<16;i++)
	{
		whitePieces[i] = NULL;
		blackPieces[i] = NULL;
	}

	// allocate memory for pieces
	for(i=0;i<16;i++)
	{
		whitePieces[i] = malloc(sizeof(Piece));
		blackPieces[i] = malloc(sizeof(Piece));
		if(whitePieces[i] == NULL)
		{
			goto error;
		}
	}

	// initialize pieces
	for(i=0;i<16;i++)
	{
		whitePieces[i]->color = WHITE;
		blackPieces[i]->color = BLACK;
	}

	whitePieces[0]->name = ROOK;
	whitePieces[1]->name = KNIGHT;
	whitePieces[2]->name = BISHOP;
	whitePieces[3]->name = QUEEN;
	whitePieces[4]->name = KING;
	whitePieces[5]->name = BISHOP;
	whitePieces[6]->name = KNIGHT;
	whitePieces[7]->name = ROOK;

	blackPieces[0]->name = ROOK;
	blackPieces[1]->name = KNIGHT;
	blackPieces[2]->name = BISHOP;
	blackPieces[3]->name = QUEEN;
	blackPieces[4]->name = KING;
	blackPieces[5]->name = BISHOP;
	blackPieces[6]->name = KNIGHT;
	blackPieces[7]->name = ROOK;

	for(i=8;i<16;i++)
	{
		whitePieces[i]->name = PAWN;
		blackPieces[i]->name = PAWN;
	}

	for(i=0;i<8;i++)
	{
		whitePieces[i]->i = 0;
		whitePieces[i]->j = i;

		blackPieces[i]->i = 7;
		blackPieces[i]->j = i;
	}

	for(i=8;i<16;i++)
	{
		whitePieces[i]->i = 1;
		whitePieces[i]->j = i - 8;

		whitePieces[i]->i = 6;
		whitePieces[i]->j = i - 8;
	}

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

	// place pieces on the board
	for(i=0;i<8;i++)
	{
		cb.array[0][i].piece = whitePieces[i];
		cb.array[7][i].piece = blackPieces[i];

	}

	for(i=8;i<16;i++)
	{
		cb.array[1][i - 8].piece = whitePieces[i];
		cb.array[6][i - 8].piece = blackPieces[i];
	}

	if(player1->color == WHITE)
	{
		player1->king = whitePieces[4];
		player2->king = blackPieces[4];
	}
	else
	{
		player1->king = blackPieces[4];
		player2->king = whitePieces[4];
	}


	return 0;

	error:

	for(i=0;i<16;i++)
	{
		free(whitePieces[i]);
		free(blackPieces[i]);
	}

	return -1;
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


