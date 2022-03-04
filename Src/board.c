#include <stdio.h>
#include <stdlib.h>
#include "board.h"

// initialize game pieces and place them on the board
Board* createBoard(Player* player1, Player* player2)
{
	int i,j;
	Board* board = malloc(sizeof(Board));
	if (board == NULL)
	{
		return NULL;
	}

	// initialize board squares
	board->counterMove=0;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(i<2 || i>5)
			{
				board->array[i][j].isOccupied = 1;
			}
			else
			{
				board->array[i][j].isOccupied = 0;
				board->array[i][j].piece = NULL;
			}
		}
	}

	// place pieces on the board
	for(i=0;i<8;i++)
	{
		if(player1->color == WHITE)
		{
			board->array[0][i].piece = player1->pieces[i];
			board->array[7][i].piece = player2->pieces[i];
		}
		else
		{
			board->array[0][i].piece = player2->pieces[i];
			board->array[7][i].piece = player1->pieces[i];
		}
	}

	for(i=8;i<16;i++)
	{
		if(player1->color == WHITE)
		{
			board->array[1][i - 8].piece = player1->pieces[i];
			board->array[6][i - 8].piece = player2->pieces[i];
		}
		else
		{
			board->array[1][i - 8].piece = player2->pieces[i];
			board->array[6][i - 8].piece = player1->pieces[i];
		}

	}

	return board;
}

int deleteBoard(Board* board)
{
	if(board == NULL)
	{
		return -1;
	}
	free(board);
	return 0;
}

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

Square* getSquare(Board* board, int i, int j)
{
	if(!existSquare(i,j))
	{
		return NULL;
	}
	return &board->array[i][j];
}

// test is the square doesn't contain piece.
int isEmptySquare(Board* board, int i, int j)
{
	if(!existSquare(i,j))
	{
		return -1;
	}
	return !getSquare(board,i,j)->isOccupied;
}

Piece* getPiece(Board* board, int i, int j)
{
	if(!existSquare(i,j) || isEmptySquare(board,i,j))
	{
		return NULL;
	}
	return getSquare(board,i,j)->piece;
}

Color getColor(Board* board, int i, int j)
{
	if( !existSquare(i,j) || isEmptySquare(board,i,j) )
	{
		return -1;
	}
	return getPiece(board,i,j)->color;
}

Name getName(Board* board, int i, int j)
{
	if(!existSquare(i,j) || isEmptySquare(board,i,j))
	{
		return -1;
	}
	return getPiece(board,i,j)->name;
}

int setName(Board* board, int i, int j, Name name)
{
	if(!existSquare(i,j) || isEmptySquare(board,i,j))
	{
		return -1;
	}
	getPiece(board,i,j)->name = name;
	return 0;
}

int getCounterMove(Board* board)
{
	return board->counterMove;
}

void incrementCounterMove(Board* board)
{
	board->counterMove++;
}

// test if all square between initial position and final position are empty
int isEmptyBetween(Board* board, int i, int j, int k, int l)
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
				if(!isEmptySquare(board,i,n))
				{
					return 0;
				}
			}
		}
		else // from right to left
		{
			for(n=j-1;n>l;n--)
			{
				if(!isEmptySquare(board,i,n))
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
				if(!isEmptySquare(board,m,j))
				{
					return 0;
				}
			}
		}
		else // from up to down
		{
			for(m=i-1;m>k;m--)
			{
				if(!isEmptySquare(board,m,j))
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
				if(!isEmptySquare(board,i+n,j+n))
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
				if(!isEmptySquare(board,i+n,j-n))
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
				if(!isEmptySquare(board,i-n,j+n))
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
				if(!isEmptySquare(board,i-n,j-n))
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

// test if the piece i,j of player can be moved to k,l
int canMovePiece(Board* board, Player* player, Player* adversary, int i, int j, int k, int l, int* captured1, int* captured2, int testLeadToChess, char* invalidMessage)
{
	int allowedMovement = 0;
	int isEnPassantCapture = 0;
	Piece* tempPiece = NULL;

	if(!existSquare(i,j) || !existSquare(k,l))
	{
		return -1;
	}
	if(isEmptySquare(board,i,j))
	{
		if(invalidMessage!=NULL)
		{
			sprintf(invalidMessage,"No piece on this square\n");
		}
		return 0;
	}

	Piece* piece = getPiece(board,i,j);
	// test if the piece belongs to the player
	if( getColor(board,i,j) != player->color )
	{
		if(invalidMessage!=NULL)
		{
			sprintf(invalidMessage,"Can't move an adversary's piece\n");
		}
		return 0;
	}
	// Test if no piece is blocking player's movement
	if(piece->name != KNIGHT && !isEmptyBetween(board,i,j,k,l))
	{
		if(invalidMessage!=NULL)
		{
			sprintf(invalidMessage,"A piece is blocking the movement\n");
		}
		return 0;
	}
	// Test is the captured piece does not belong to the player
	if( !isEmptySquare(board,k,l) && (getColor(board,i,j) ==  getColor(board,k,l)) )
	{
		if(invalidMessage!=NULL)
		{
			sprintf(invalidMessage,"Can't eat your own piece\n");
		}
		return 0;
	}
	// test if movement is allowed depending on the type of piece
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
			allowedMovement = ( pawnMovement(i,j,k,l,piece->color,isCapturingPiece(board,piece->color,k,l))
				|| enPassantCapture(board,i,j,k,l,captured1,captured2, &isEnPassantCapture));
			break;
	}

	if(!allowedMovement)
	{
		if(invalidMessage!=NULL)
		{
			sprintf(invalidMessage,"This movement is not allowed\n");
		}
		return 0;
	}
	else if(testLeadToChess)
	{
		// test if the move i,j -> k,l would lead player to chess
		int chess;

		if(!isEnPassantCapture)
		{
			*captured1 = k;
			*captured2 = l;
		}
		// save capture piece in temporary pointeur
		tempPiece = board->array[*captured1][*captured2].piece;
		// upload position of pieces
		board->array[*captured1][*captured2].piece = NULL;
		board->array[*captured1][*captured2].isOccupied = 0;
		movePiece(board,i,j,k,l);
		setPosI(board->array[k][l].piece, k, board->counterMove);
		setPosJ(board->array[k][l].piece, l, board->counterMove);
		chess = testChess(board,adversary,player);
		if(chess == -1)
		{
			fprintf(stderr,"testChess returned error\n");
			return -1;
		}
		if(chess && invalidMessage!=NULL)
		{
			sprintf(invalidMessage,"This move would lead to chess\n");
		}
		// restore position of pieces after test
		movePiece(board,k,l,i,j);
		setPosI(board->array[i][j].piece, i, board->counterMove);
		setPosJ(board->array[i][j].piece, j, board->counterMove);
		if(tempPiece!=NULL)
		{
			board->array[*captured1][*captured2].piece = tempPiece;
			board->array[*captured1][*captured2].isOccupied = 1;
		}
		return !chess;
	}
	else
	{
		return 1;
	}
}

// test if the king of adversary can be captured by a piece of player
int testChess(Board* board, Player* player, Player* adversary)
{
	int ret;

	ret = canBeEaten(board,adversary,player,adversary->king->posI[board->counterMove], adversary->king->posJ[board->counterMove],0);
	if(ret == -1)
	{
		fprintf(stderr,"canBeEaten returned error\n");
		return -1;
	}
	return ret;
}

// test if the piece on the position (k,l) of player can be captured by the adversary
int canBeEaten(Board* board, Player* player,Player* adversary, int k, int l, int testChess)
{
	int i,j,ret;
	if(!existSquare(k,l) || isEmptySquare(board,k,l))
	{
		return -1;
	}
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			ret = canMovePiece(board,adversary,player,i,j,k,l,NULL,NULL,testChess,NULL);
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
	return 0;
}

// test if the king of player can be unavoidably capured by the adversary regardless the next movement
int canDoLegalMove(Board* board, Player* player, Player* adversary)
{
	int i,j,k,l,m=0,n=0;
	int ret;
	Color color = player->color;

	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(board->array[i][j].isOccupied && (board->array[i][j].piece->color == color))
			{
				for(k=0;k<8;k++)
				{
					for(l=0;l<8;l++)
					{
						ret = canMovePiece(board,player,adversary,i,j,k,l,&m,&n,1,NULL);
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

// test is the case is occupied by a piece of the other player
int isCapturingPiece(Board* board, Color playerColor, int i, int j)
{
	if( !isEmptySquare(board,i,j) && (getColor(board,i,j) != playerColor) )
	{
		return 1;
	}
	return 0;
}

// test if player can execute the castle movement
int testCastling(Board* board, Player* player, Player* adversary, int i, int j, int k, int l)
{
	if( isEmptySquare(board,i,j) || isEmptySquare(board,k,l) || (getColor(board,i,j) != player->color)  || (getColor(board,i,j) != player->color)
		|| hasMoved(getPiece(board,i,j), board->counterMove) || hasMoved(getPiece(board,k,l), board->counterMove) )
	{
		return 0;
	}
	if(getColor(board,i,j) == WHITE)
	{
		if( i==0 && j==4 && k==0 && (l==0 || l==7 ) )
		{
			if(l==0 && !canBeEaten(board,player,adversary,0,3,0) && !canBeEaten(board,player,adversary,0,2,0) && isEmptyBetween(board,0,4,0,0))
			{
				return 1;
			}
			else if(l==7 && !canBeEaten(board,player,adversary,0,5,0) && !canBeEaten(board,player,adversary,0,6,0) && isEmptyBetween(board,0,5,0,7))
			{
				return 1;
			}
		}
	}
	else
	{
		if(i==7 && j==4 && k==7 && (l==0 || l==7 ) )
		{
			if(l==0 && !canBeEaten(board,player,adversary,7,3,0) && !canBeEaten(board,player,adversary,7,2,0) && isEmptyBetween(board,7,4,7,0))
			{
				return 1;
			}
			else if(l==7 && !canBeEaten(board,player,adversary,7,5,0) && !canBeEaten(board,player,adversary,7,6,0) && isEmptyBetween(board,7,5,7,7))
			{
				return 1;
			}
		}
	}
	return 0;
}

// execute castle movement
int castling(Board* board, int i, int j, int k, int l)
{
	if(!existSquare(i,j) || !existSquare(k,l))
	{
		return -1;
	}
	if(getColor(board,i,j) == WHITE)
	{
		if(l==0)
		{
			movePiece(board,0,4,0,2);
			movePiece(board,0,0,0,3);
		}
		else
		{
			movePiece(board,0,4,0,6);
			movePiece(board,0,7,0,5);
		}
	}
	else
	{
		if(l==0)
		{
			movePiece(board,7,4,7,2);
			movePiece(board,7,0,7,3);
		}
		else
		{
			movePiece(board,7,4,7,6);
			movePiece(board,7,7,7,5);
		}
	}
	return 0;
}

// update the board with new position of the piece
int movePiece(Board* board,int i, int j, int k, int l)
{
	if(!existSquare(i,j) || !existSquare(k,l) || isEmptySquare(board,i,j))
	{
		return -1;
	}
	Piece* piece = board->array[i][j].piece;
	board->array[k][l].piece = piece;
	board->array[k][l].isOccupied = 1;
	board->array[i][j].piece = NULL;
	board->array[i][j].isOccupied = 0;

	return 0;
}

// return 1 and change piece name if the pawn can be promoted
int promotion(Board* board, int i, int j)
{
	if(!existSquare(i,j))
	{
		return -1;
	}
	int selectedPiece = 0;
	if(getName(board,i,j)!=PAWN || (i!=7 && getColor(board,i,j)== WHITE) || (i!=1 && getColor(board,i,j)== BLACK))
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
			setName(board,i,j,BISHOP);
			break;
			case 2:
			setName(board,i,j,KNIGHT);
			break;
			case 3:
			setName(board,i,j,ROOK);
			break;
			case 4:
			setName(board,i,j,QUEEN);
		}
		return 1;
	}
}

int promotionOnline(Board* board, Player* player, int i, int j, int socket)
{
	if(!existSquare(i,j))
	{
		return -1;
	}
	int selectedPiece = 0;
	char buffer[10] ="";
	if(getName(board,i,j)!=PAWN || (i!=7 && getColor(board,i,j)== WHITE) || (i!=1 && getColor(board,i,j)== BLACK))
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
		setName(board,i,j,BISHOP);
		break;
		case 2:
		setName(board,i,j,KNIGHT);
		break;
		case 3:
		setName(board,i,j,ROOK);
		break;
		case 4:
		setName(board,i,j,QUEEN);
	}
	return 1;
}

// add the piece to the captured piece list of the player and update the board
int capturePiece(Board* board,Player* player,int k,int l)
{
	if(!existSquare(k,l) || isEmptySquare(board,k,l) || addPiece(player->capuredPieces, board->array[k][l].piece)==-1)
	{
		return -1;
	}
	board->array[k][l].piece = NULL;
	board->array[k][l].isOccupied = 0;

	return 0;
}

// update the new position of all pieces according to position on board
void updatePosition(Board* board)
{
	int i, j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(!isEmptySquare(board,i,j))
			{
				setPosI(board->array[i][j].piece, i, board->counterMove);
				setPosJ(board->array[i][j].piece, j, board->counterMove);
			}
		}
	}
}

// Update square of the board and position od the pieces
int updateBoard(Board* board, Player* player, int i, int j, int k, int l, int captured1, int captured2)
{
	if(!existSquare(i,j) || !existSquare(k,l) || isEmptySquare(board,i,j))
	{
		return -1;
	}
	if(!(player->isCastling))
    {
    	if(!isEmptySquare(board,captured1,captured2))
		{
			if(capturePiece(board,player,captured1,captured2)==-1)
			{
				fprintf(stderr,"capturePiece returned error\n");
				return -1;
			}
		}
		if(movePiece(board,i,j,k,l) == -1)
		{
			fprintf(stderr,"movePiece returned error\n");
			return -1;
		}
    }
    else
    {
    	if(castling(board,i,j,k,l) == -1)
    	{
    		fprintf(stderr,"castling returned error\n");
    		return -1;
    	}
    	player->isCastling = 0;
    	player->hasCastled = 1;
    }
    updatePosition(board);
    return 0;
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

// print board in console
void printBoard(Board* board,Color player)
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
				if(board->array[i][j].isOccupied)
				{
					printfPiece(board->array[i][j].piece);
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
				if(board->array[i][j].isOccupied)
				{
					printfPiece(board->array[i][j].piece);
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


