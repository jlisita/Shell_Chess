#include <stdio.h>
#include <stdlib.h>
#include "board.h"


// test if the board contain the square (i,j)
int existSquare(int i, int j)
{
	if( i < 0 || j < 0 ||  i > 7 || j > 7 )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

// test if the piece can be moved
int canMovePiece(Player* player, int i, int j, int k, int l, int* captured1, int* captured2, int isTestChess, int isTestMat)
{
	int allowedMovement = 0;
	int isEnPassantCapture = 0;
	Piece* tempPiece = NULL;

	if(!existSquare(i,j) || !existSquare(k,l))
	{
		return -1;
	}
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

	if(piece->name != KNIGHT && !isEmptyBetween(i,j,k,l))
	{
		if(!isTestChess && !isTestMat)
		{
			printf("A piece is blocking the movement\n");
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
			allowedMovement = kingMovement(i,j,k,l);
			break;
		case QUEEN:
			allowedMovement = queenMovement(i,j,k,l);
			break;
		case BISHOP:
			allowedMovement = bishopMovement(i,j,k,l);
			break;
		case ROOK:
			allowedMovement = rookMovement(i,j,k,l);
			break;
		case KNIGHT:
			allowedMovement = knightMovement(i,j,k,l);
			break;
		case PAWN:
			allowedMovement = ( pawnMovement(i,j,k,l,piece->color,isCapturingPiece(piece->color,k,l))
				|| enPassantCapture(i,j,k,l,captured1,captured2, &isEnPassantCapture));
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
			*captured1 = k;
			*captured2 = l;
		}
		tempPiece = cb.array[*captured1][*captured2].piece;
		cb.array[*captured1][*captured2].piece = NULL;
		cb.array[*captured1][*captured2].isOccupied = 0;
		movePiece(i,j,k,l);
		setI(cb.array[k][l].piece, k, cb.counterMove);
		setJ(cb.array[k][l].piece, l, cb.counterMove);
		chess = testChess(player);
		if(chess == -1)
		{
			fprintf(stderr,"testChess returned error\n");
			return -1;
		}
		if(chess && !isTestMat)
		{
			printf("This move would lead to chess\n");
		}

		movePiece(k,l,i,j);
		setI(cb.array[i][j].piece, i, cb.counterMove);
		setJ(cb.array[i][j].piece, j, cb.counterMove);
		if(tempPiece!=NULL)
		{
			cb.array[*captured1][*captured2].piece = tempPiece;
			cb.array[*captured1][*captured2].isOccupied = 1;
		}
		return !chess;
	}
	else
	{
		return 1;
	}
}

// test is the case is occupied by a piece of the other player
int isCapturingPiece(Color playerColor, int i, int j)
{
	if( !isEmptySquare(i,j) && (getColor(i,j) != playerColor) )
	{
		return 1;
	}
	return 0;
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
	if(!existSquare(i,j) || !existSquare(k,l) || isEmptySquare(i,j))
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
	if(!existSquare(i,j) || !existSquare(k,l))
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
	if(!existSquare(i,j))
	{
		return -1;
	}
	int selectedPiece = 0;
	if(getName(i,j)!=PAWN || (i!=7 && getColor(i,j)== WHITE) || (i!=1 && getColor(i,j)== BLACK))
	{
		return 0;
	}
	else
	{
		do
		{
			menuCasling(&selectedPiece);

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

int promotionOnline(Player* player, int i, int j, int socket)
{
	if(!existSquare(i,j))
	{
		return -1;
	}
	int selectedPiece = 0;
	char buffer[10] ="";
	if(getName(i,j)!=PAWN || (i!=7 && getColor(i,j)== WHITE) || (i!=1 && getColor(i,j)== BLACK))
	{
		return 0;
	}
	if(player->isPlaying)
	{
		do
		{
			menuCasling(&selectedPiece);

		}while(selectedPiece < 1 || selectedPiece > 4 );

		sprintf(buffer,"%d",selectedPiece);

		if(send(socket, buffer, sizeof(buffer), 0) == -1)
		{
			perror("send:");
			return -1;
		}
	}
	else
	{
		if(recv(socket, buffer, sizeof(buffer), 0) == -1)
		{
			perror("recv:");
			return -1;
		}
		selectedPiece = (int)strtol(buffer,NULL,10);

	}
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

// add the piece to the captured piece list of the player and update the board
int capturePiece(Player* player,int k,int l)
{
	if(!existSquare(k,l) || isEmptySquare(k,l) || addPiece(player->capuredPieces, cb.array[k][l].piece)==-1)
	{
		return -1;
	}
	cb.array[k][l].piece = NULL;
	cb.array[k][l].isOccupied = 0;

	return 0;
}

// update the new position of all pieces according to position on board
void updatePosition(int counterMove)
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

// Update square of the board and position od the pieces
int updateBoard(Player* player, int i, int j, int k, int l, int captured1, int captured2)
{
	if(!existSquare(i,j) || !existSquare(k,l) || isEmptySquare(i,j))
	{
		return -1;
	}
	if(!(player->isCastling))
    {
    	if(!isEmptySquare(captured1,captured2))
		{
			if(capturePiece(player,captured1,captured2)==-1)
			{
				fprintf(stderr,"capturePiece returned error\n");
				return -1;
			}
		}
		if(movePiece(i,j,k,l) == -1)
		{
			fprintf(stderr,"movePiece returned error\n");
			return -1;
		}
    }
    else
    {
    	if(castling(i,j,k,l) == -1)
    	{
    		fprintf(stderr,"castling returned error\n");
    		return -1;
    	}
    	player->isCastling = 0;
    	player->hasCastled = 1;
    }
    updatePosition(cb.counterMove);
    return 0;
}

// test if the king of player can be captured by the adversary
int testChess(Player* player)
{
	printf("test chess\n");
	int ret;

	ret = canBeEaten(player, player->king->i[cb.counterMove], player->king->j[cb.counterMove]);
	if(ret == -1)
	{
		fprintf(stderr,"canBeEaten returned error\n");
		return -1;
	}
	return ret;
}

// test if the Piece on the position (k,l) of player can be captured by the adversary
int canBeEaten(Player* player, int k, int l)
{
	int i,j,ret;
	if(!existSquare(k,l) || isEmptySquare(k,l))
	{
		return -1;
	}
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			ret = canMovePiece(player,i,j,k,l,NULL,NULL,1,0);
			if(ret == -1)
			{
				fprintf(stderr,"canMovePiece returned error\n");
				return -1;
			}
			else if(ret == 1)
			{
				printf("can move %d %d to %d %d\n",i,j,k,l);
				return 1;
			}
		}
	}
	return 0;
}

// test if the king of player can be unavoidably capured by the adversary
int canDoLegalMove(Player* player)
{
	int i,j,k,l,m=0,n=0;
	int ret;
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
						ret = canMovePiece(player,i,j,k,l,&m,&n,0,1);
						if(ret == -1)
						{
							fprintf(stderr,"canMovePiece returned error\n");
							return -1;
						}
						else if(ret == 1)
						{
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

Piece* getPiece(int i, int j)
{
	if(!existSquare(i,j) || isEmptySquare(i,j))
	{
		return NULL;
	}
	return cb.array[i][j].piece;
}

Name getName(int i, int j)
{
	if(!existSquare(i,j) && isEmptySquare(i,j))
	{
		return -1;
	}
	return getPiece(i,j)->name;
}

int setName(int i, int j, Name name)
{
	if(!existSquare(i,j) && isEmptySquare(i,j))
	{
		return -1;
	}
	getPiece(i,j)->name = name;
	return 0;
}

// test is the square doesn't contain piece.
int isEmptySquare(int i,int j)
{
	if(!existSquare(i,j))
	{
		return -1;
	}
	return !cb.array[i][j].isOccupied;
}

// test if all square between initial position and final position are empty
int isEmptyBetween(int i, int j, int k, int l)
{
	if(!existSquare(i,j) || !existSquare(k,l))
	{
		return -1;
	}
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
	if( !existSquare(i,j) || isEmptySquare(i,j) )
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
void printBoard(Color player)
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


