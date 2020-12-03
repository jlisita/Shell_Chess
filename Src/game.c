#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "game.h"
#include "cmd_interface.h"
#include "socket.h"

int game()
{
	int ret = 0;

	do
	{
		if(menuMode(&ret)==-1)
		{
			return -1;
		}
		if(ret == 1)
		{
			if(localGame(NULL,NULL)==-1)
			{
				fprintf(stderr,"localGame returned error\n");
				return -1;
			}
		}
		else
		{
			Profil* myProfil = NULL;
			Profil* adversaryProfil = NULL;
			if((myProfil = malloc(sizeof(Profil))) == NULL)
			{
				perror("malloc");
				return -1;
			}
			if(loadProfil(myProfil) == -1)
			{
				if(errno == 2)
				{
					if(createProfil(myProfil)==-1)
					{
						fprintf(stderr,"createProfil returned error\n");
						return -1;
					}
				}
				else
				{
					fprintf(stderr,"loadProfil returned error\n");
					return -1;
				}
			}
			do
			{
				if(menuOnline(&ret,myProfil->friends) == -1)
				{
					fprintf(stderr,"menuOnline returned error\n");
					return -1;
				}
				if(ret == 0)
				{
					if(addFriends(myProfil)==-1)
					{
						fprintf(stderr,"addFriends returned error\n");
						return -1;
					}
				}
			}
			while (ret == 0);
			if((adversaryProfil = getProfil(myProfil->friends, ret-1)) == NULL)
			{
				printf("profil invalide");
				return -1;
			}
			if(menuColor(&ret)==-1)
			{
				return -1;
			}
			if(onlineGame(myProfil,adversaryProfil,ret)==-1)
			{
				fprintf(stderr,"onlineGame returned error\n");
				return -1;
			}
		}
		printf("Do you want to quit ? (1 for yes/ 0 for no)\n");
		do
		{
			if(readInt(&ret) == -1)
			{
				return -1;
			}
			if(ret != 0 && ret != 1)
			{
				printf("Error: (1 for yes/ 0 for no)\n");
			}
		}while(ret != 0 && ret != 1);

	}while(ret==0);

	return 0;
}

// initialize board and manage player turn
int localGame()
{
	Game* game = NULL;

	if((game = createGame()) == NULL )
	{
		fprintf(stderr,"createGame returned error\n");
		return -1;
	}

	do
	{
		printGame(game->currentPlayer, game->nextPlayer, game->recordedMoves);

		if(turn(game->currentPlayer,game->nextPlayer,game->recordedMoves) == -1)
		{
			fprintf(stderr,"turn returned error\n");
			return -1;
		}

		game->endOfGame = endOfGame(game->currentPlayer, game->nextPlayer);
		if(game->endOfGame == -1)
		{
			fprintf(stderr,"endOfGame returned error\n");
			return -1;
		}

		if(!game->endOfGame)
		{
			Player* temp = NULL;
			temp = game->currentPlayer;
			game->currentPlayer = game->nextPlayer;
			game->nextPlayer = temp;
			game->currentPlayer->isPlaying = 1;
			game->nextPlayer->isPlaying = 0;
		}

	}while(!game->endOfGame);

	printGame(game->currentPlayer, game->nextPlayer, game->recordedMoves);
	deleteGame(game);
	game = NULL;

	return 0;
}

Game* createGame()
{
	Game* game = malloc(sizeof(Game));
	if(game == NULL)
	{
		perror("malloc");
		return NULL;
	}
	game->player = NULL;
	game->adversary = NULL;
	game->player = createPlayer("White player",WHITE);
	if(game->player == NULL)
	{
		fprintf(stderr,"createPlayer returned error\n");
		return NULL;
	}
	game->adversary = createPlayer("Black player",BLACK);
	if(game->adversary == NULL)
	{
		fprintf(stderr,"createPlayer returned error\n");
		return NULL;
	}
	strcpy(game->recordedMoves,"");
	game->endOfGame = 0;
	game->currentPlayer = game->player;
	game->nextPlayer = game->adversary;

	if(initializeBoard(game->player,game->adversary)==-1)
	{
		fprintf(stderr,"initializeBoard returned error\n");
		return NULL;
	}
	return game;
}

int deleteGame(Game* game)
{
	if(game == NULL)
	{
		return -1;
	}
	deletePlayer(game->player);
	deletePlayer(game->adversary);
	free(game);
	return 0;
}

// manage command entry and update game
int turn(Player* currentPlayer, Player* nextPlayer, char* recordedMoves)
{
	int i,j,k,l;
	int captured1=0, captured2=0;
	int validMovement=0;

	do
	{
		int validCommand=0;
		do
		{
			validCommand = getCommand(currentPlayer);
		}while(validCommand!=1);

		if(currentPlayer->abandonment)
		{
			return 0;
		}

		validMovement = nextMovement(currentPlayer, nextPlayer, &i, &j, &k, &l, &captured1, &captured2);
		if(validMovement == -1)
		{
			fprintf(stderr,"nextMovement returned error\n");
			return -1;
		}

	}while(!validMovement);

	cb.counterMove++;
    if(updateBoard(currentPlayer, i, j, k, l, captured1, captured2)==-1)
    {
    	fprintf(stderr,"updateBoard returned error\n");
    	return -1;
    }
    if(!currentPlayer->isCastling)
    {
    	promotion(k,l);
    }

	updateRecordedMoves(currentPlayer,recordedMoves);

	return 0;
}

int getCommand(Player* player)
{
	int validCommand = 0;
	printf("%s, enter your move:",player->name);
	if(readString(player->command,MAXCOMMANDSIZE)==-1)
	{
		return -1;
	}
	validCommand = verifyCommand(player);
	if(validCommand == 0)
	{
		return 0;
	}
	return 1;
}

// test if command entered is valid
int verifyCommand(Player* player)
{
	char* rankIndex = "12345678";
	char* fileIndex = "abcdefghABCDEFGH";
	char* command = player->command;

	if(strcmp("abandon",command)==0)
	{
		player->abandonment = 1;
		return 1;
	}
	if(strchr(fileIndex, command[0]) != NULL && strchr(rankIndex, command[1])!=NULL && command[2]==' '
		&& strchr(fileIndex, command[3])!=NULL && strchr(rankIndex, command[4])!=NULL)
	{
		return 1;
	}
	else
	{
		printf("Commande invalide\n");
		return 0;
	}
}

// test if the movement asked by the player is possible
int nextMovement(Player* currentPlayer, Player* nextPlayer, int* i, int* j, int* k, int* l, int* captured1, int* captured2)
{
	int validMovement=0;
	char* command = currentPlayer->command;
	char invalidMessage[100];

	*i = rankIndexToInt((command)[1]);
	*j = fileIndexToInt((command)[0]);
	*k = rankIndexToInt((command)[4]);
	*l = fileIndexToInt((command)[3]);

	currentPlayer->isCastling = (!(currentPlayer->isChess) && !(currentPlayer->hasCastled) && testCastling(currentPlayer,nextPlayer, *i, *j, *k, *l));
	if(!currentPlayer->isCastling)
	{
		validMovement = canMovePiece(currentPlayer,nextPlayer,*i,*j,*k,*l,captured1,captured2,1,invalidMessage);
		if(validMovement == -1)
		{
			fprintf(stderr,"canMovePiece returned error\n");
			return -1;
		}
	}
	if(!validMovement)
	{
		printf("%s",invalidMessage);
	}

	return validMovement;
}

// update list of moves during the match
void updateRecordedMoves(Player* player, char* recordedMoves)
{
	static int indexMoves = 0;
	if(player->color==WHITE)
	{
		indexMoves++;
		char index[10]="";
		sprintf(index,"%d. ",indexMoves);
		strcat(recordedMoves,index);
	}
	char cmd[10]="";
	sprintf(cmd,"%c%c%c%c ",tolower(player->command[0]),tolower(player->command[1]),tolower(player->command[3]),tolower(player->command[4]));
	strcat(recordedMoves,cmd);
}

// Update variable isChess, isMat,  and test if the game sould be stop
int endOfGame(Player* currentPlayer, Player* nextPlayer)
{
	if(!currentPlayer->abandonment)
	{
		int legalMove = 0;
		int chess = 0;
		legalMove = canDoLegalMove(nextPlayer,currentPlayer);
		if(legalMove == -1)
		{
			fprintf(stderr,"legalMove returned error\n");
			return -1;
		}
		chess = testChess(currentPlayer,nextPlayer);
		if(chess == -1)
		{
			fprintf(stderr,"testChess returned error\n");
			return -1;
		}

		if(chess)
		{
			printf("CHESS!\n");
			nextPlayer->isChess = 1;
			nextPlayer->isMat = !legalMove;
		}
		else
		{
			nextPlayer->isStalemate = !legalMove;
		}
	}

	if(nextPlayer->isMat)
	{
		printf("End of game %s has won by Mat.\n\n",currentPlayer->name);
		return 1;
	}
	else if (currentPlayer->abandonment)
	{
		printf("End of game %s has won by abandonment.\n\n",nextPlayer->name);
		return 1;
	}
	else if(currentPlayer->isStalemate)
	{
		printf("End of game by stalemate.\n\n");
		return 1;
	}
	return 0;
}

int onlineGame(Profil* myProfil, Profil* adversaryProfil, int mode)
{
	int socketServeur, socketClient;
	struct sockaddr_in sinServeur, sinClient;
	char ipServeur[100]="";
	char ipClient[100]="";
	Player* player = NULL;
	Player* adversary = NULL;
	char recordedMoves[1000]="";
	int gameOver = 0;
	Player* currentPlayer = NULL;
	Player* nextPlayer = NULL;

	if(mode==1)
	{
		strcpy(ipServeur, myProfil->IPadress);
		strcpy(ipClient, adversaryProfil->IPadress);
		if(serverConnection(ipClient, &socketServeur, &socketClient, &sinServeur, &sinClient) == -1)
		{
			printf("connection failed\n");
			return -1;
		}
	}
	else
	{
		strcpy(ipServeur, adversaryProfil->IPadress);
		if(clientConnection(ipServeur, &socketClient, &sinServeur) == -1)
		{
			printf("connection failed\n");
			return -1;
		}
	}


	if(mode == 1)
	{
		player = createPlayer(myProfil->name,WHITE);
		if(player == NULL)
		{
			fprintf(stderr,"createPlayer returned error\n");
			return -1;
		}
		adversary = createPlayer(adversaryProfil->name,BLACK);
		if(adversary == NULL)
		{
			fprintf(stderr,"createPlayer returned error\n");
			return -1;
		}
		currentPlayer = player;
		nextPlayer = adversary;

	}
	else
	{
		player = createPlayer(myProfil->name,BLACK);
		if(player == NULL)
		{
			fprintf(stderr,"createPlayer returned error\n");
			return -1;
		}
		adversary = createPlayer(adversaryProfil->name,WHITE);
		if(adversary == NULL)
		{
			fprintf(stderr,"createPlayer returned error\n");
			return -1;
		}
		currentPlayer = adversary;
		nextPlayer = player;
	}

	if(initializeBoard(player,adversary)==-1)
	{
		fprintf(stderr,"initializeBoard returned error\n");
		return -1;
	}

	do
	{
		printGame(currentPlayer, nextPlayer, recordedMoves);

		if(turnOnline(currentPlayer, nextPlayer, player->isPlaying, recordedMoves, socketClient)==-1)
		{
			fprintf(stderr,"turnOnline returned error\n");
			return -1;
		}

		gameOver = endOfGame(currentPlayer, nextPlayer);
		if(gameOver == -1)
		{
			fprintf(stderr,"turnOnline returned error\n");
			return -1;
		}

		if(!gameOver)
		{
			Player* temp = NULL;
			temp = currentPlayer;
			currentPlayer = nextPlayer;
			nextPlayer = temp;
			currentPlayer->isPlaying = 1;
			nextPlayer->isPlaying = 0;
		}

	}while(!gameOver);

	printGame(currentPlayer, nextPlayer, recordedMoves);
	deletePlayer(player);
	player = NULL;
	deletePlayer(adversary);
	adversary = NULL;

	return 0;
}

int turnOnline(Player* currentPlayer,Player* nextPlayer,  int isPlaying, char* recordedMoves, int socketClient)
{
	int i=0,j=0,k=0,l=0;
	int ret;
	int captured1=0, captured2=0;
	int validMovement=0;

	if(isPlaying)
	{
		printf("It's your turn\n");
		do
		{
			int validCommand = 0;
			do
			{
				validCommand = getCommand(currentPlayer);
			}while(validCommand!=1);

			if(currentPlayer->abandonment)
			{
				return 0;
			}

			validMovement = nextMovement(currentPlayer, nextPlayer, &i, &j, &k, &l, &captured1, &captured2);

		}while(!validMovement);

		if(send(socketClient, currentPlayer->command, 100, 0) == -1)
		{
			perror("send:");
			return -1;
		}
	}
	else
	{
		printf("It's %s turn\n",currentPlayer->name);

		if(recv(socketClient, currentPlayer->command, 100, 0) == -1)
		{
			perror("recv:");
			return -1;
		}

		if(strcmp("abandon",currentPlayer->command)==0)
		{
			currentPlayer->abandonment = 1;
			return 0;
		}

		ret = nextMovement(currentPlayer,nextPlayer, &i, &j, &k, &l, &captured1, &captured2);
		if(ret == -1)
		{
			fprintf(stderr,"nextMovement returned error\n");
		}

	}

	cb.counterMove++;
    if(updateBoard(currentPlayer, i, j, k, l, captured1, captured2)==-1)
    {
    	fprintf(stderr,"updateBoard returned error\n");
    	return -1;
    }
    if(!currentPlayer->isCastling)
    {
    	if(promotionOnline(currentPlayer,k,l,socketClient) == -1)
    	{
    		fprintf(stderr,"nextMovement returned error\n");
    		return -1;
    	}
    }
	updateRecordedMoves(currentPlayer,recordedMoves);

	return 0;
}

 void printGame(Player* currentPlayer, Player* nextPlayer, char* recordedMoves)
 {
 	printf("%s\n",recordedMoves);
	printf("Pieces captured by you: ");
	printListPieces(currentPlayer->capuredPieces);
	printf("Pieces captured by %s: ",nextPlayer->name);
	printListPieces(nextPlayer->capuredPieces);
	printBoard(currentPlayer->color);
 }

