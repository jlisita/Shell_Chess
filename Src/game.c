#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "game.h"

#define PORT 1025


int menuMode(int* mode)
{
	printf("Select mode:\n");
	printf(" 1. Local\n");
	printf(" 2. Online\n");
	do
	{
		if(readInt(mode)==-1)
		{
			return -1;
		}
		if(*mode != 1 && *mode != 2)
		{
			printf("Invalid number: enter 1 or 2");
		}

	}while(*mode !=1 && *mode != 2);
	return 0;
}

int menuOnline(int* ret, ListProfil* friends)
{
	printf("Select a friend\n");
	printf(" 0. Add a friend\n");
	printListProfil(friends);
	do
	{
		if(readInt(ret)==-1)
		{
			return -1;
		}
		if(*ret < 0 || *ret > friends->size)
		{
			printf("Invalid number\n");
		}

	}while(*ret < 0 || *ret > friends->size);
	return 0;
}

int menuColor(int* ret)
{
	printf("Select color:\n");
	printf(" 1. White\n");
	printf(" 2. Black\n");
	do
	{
		if(readInt(ret)==-1)
		{
			return -1;
		}
		if(*ret != 1 && *ret != 2)
		{
			printf("Invalid number: enter 1 or 2");
		}

	}while(*ret !=1 && *ret != 2);
	return 0;
}

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
				return -1;
			}
		}
		else
		{
			Profil* myProfil = NULL;
			Profil* adversaryProfil = NULL;
			if((myProfil = malloc(sizeof(Profil))) == NULL)
			{
				return -1;
			}
			if(loadProfil(myProfil) == -1)
			{
				if(errno == 2)
				{
					if(createProfil(myProfil)==-1)
					{
						return -1;
					}
				}
				else
				{
					return -1;
				}
			}
			do
			{
				if(menuOnline(&ret,myProfil->friends) == -1)
				{
					return -1;
				}
				if(ret == 0)
				{
					if(addFriends(myProfil)==-1)
					{
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
	GameData* gameData = malloc(sizeof(GameData));

	if(initGame(gameData) == -1)
	{
		return -1;
	}

	do
	{
		if(turn(gameData->currentPlayer,gameData->recordedMoves)==-1)
		{
			return -1;
		}

		if(!gameData->currentPlayer->abandonment)
		{

			gameData->adversary->isChess = testChess(gameData->adversary);
			if(gameData->adversary->isChess == -1)
			{
				return -1;
			}
			if(gameData->adversary->isChess)
			{
				printf("CHESS!\n");
				gameData->adversary->isMat = testMat(gameData->adversary);
				if(gameData->adversary->isChess == -1)
				{
					return -1;
				}
			}
		}

		gameData->endGame = endOfGame(gameData);

		if(!gameData->endGame)
		{
			if(gameData->nextTurn == gameData->player1->color)
			{
				gameData->currentPlayer = gameData->player1;
				gameData->adversary = gameData->player2;
				gameData->nextTurn = gameData->player2->color;
			}
			else
			{
				gameData->currentPlayer = gameData->player2;
				gameData->adversary = gameData->player1;
				gameData->nextTurn = gameData->player1->color;
			}
		}

	}while(!gameData->endGame);

	free(gameData);

	return 0;
}

int initGame(GameData* gameData)
{
	gameData->player1 = malloc(sizeof(Player));
	gameData->player2 = malloc(sizeof(Player));
	if(initializePlayer(gameData->player1,"White player",WHITE)==-1)
	{
		return -1;
	}
	if(initializePlayer(gameData->player2,"Black player",BLACK)==-1)
	{
		return -1;
	}
	strcpy(gameData->recordedMoves,"");
	gameData->endGame = 0;
	gameData->currentPlayer = gameData->player1;
	gameData->adversary = gameData->player2;
	gameData->nextTurn = BLACK;
	if(initializeBoard(gameData->player1,gameData->player2)==-1)
	{
		return -1;
	}
	return 0;
}

// manage command entry and update game
int turn(Player* player, char* recordedMoves)
{
	int i,j,k,l;
	int captured1=0, captured2=0;
	int validMovement=0;
	printfBoard(player->color);
	printf("%s\n",recordedMoves);
	printf("Pieces captured: ");
	printListPieces(player->capuredPieces);

	do
	{
		int validCommand=0;
		do
		{
			validCommand = getCommand(player);
		}while(validCommand!=1);

		if(player->abandonment)
		{
			return 0;
		}

		validMovement = nextMovement(player, &i, &j, &k, &l, &captured1, &captured2);

	}while(!validMovement);

	cb.counterMove++;
    if(updateBoard(player, i, j, k, l, captured1, captured2)==-1)
    {
    	return -1;
    }

	updateRecordedMoves(player,recordedMoves);

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
int nextMovement(Player* player, int* i, int* j, int* k, int* l, int* captured1, int* captured2)
{
	int validMovement=0;
	char* command = player->command;

	*i = rankIndexToInt((command)[1]);
	*j = fileIndexToInt((command)[0]);
	*k = rankIndexToInt((command)[4]);
	*l = fileIndexToInt((command)[3]);

	player->isCastling = (!(player->isChess) && !(player->hasCastled) && testCastling(player, *i, *j, *k, *l));
	if(!player->isCastling)
	{
		validMovement = canMovePiece(player,*i,*j,*k,*l,captured1,captured2,0,0);
	}

	return validMovement;
}

// Update square of the board and position od the pieces
int updateBoard(Player* player, int i, int j, int k, int l, int captured1, int captured2)
{
	if(!(player->isCastling))
    {
    	if(!isEmptySquare(captured1,captured2))
		{
			if(capturePiece(player,captured1,captured2)==-1)
			{
				return -1;
			}
		}
		movePiece(i,j,k,l);
		promotion(k,l);
    }
    else
    {
    	castling(i,j,k,l);
    	player->isCastling = 0;
    	player->hasCastled = 1;
    }
    updatePosition(cb.counterMove);
    return 0;
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

// Test if the game sould be stop by abandon or Mat
int endOfGame(GameData* gameData)
{
	if(gameData->adversary->isMat)
	{
		printfBoard(gameData->currentPlayer->color);
		printf("End of game %s has won by Mat.\n\n",gameData->currentPlayer->name);
		return 1;
	}
	else if (gameData->currentPlayer->abandonment)
	{
		printf("End of game %s has won by abandonment.\n\n",gameData->adversary->name);
		return 1;
	}
	return 0;
}

int onlineGame(Profil* myProfil, Profil* adversaryProfil,int mode)
{
	Player* player1 = malloc(sizeof(Player));
	Player* player2 = malloc(sizeof(Player));
	char recordedMoves[1000]="";
	int endGame = 0;
	Color myColor;
	Color adversaryColor;
	Player* currentPlayer = NULL;
	Player* nextPlayer = NULL;
	Player* temp;
	int socketServeur, socketClient;
	struct sockaddr_in sinServeur, sinClient;
	char ipServeur[100]="";
	char ipClient[100]="";

	if(mode == 1)
	{
		strcpy(ipServeur, myProfil->IPadress);
		strcpy(ipClient, adversaryProfil->IPadress);
		myColor = WHITE;
		adversaryColor = BLACK;
		currentPlayer = player1;
		nextPlayer = player2;
	}
	else
	{
		strcpy(ipServeur, adversaryProfil->IPadress);
		myColor = BLACK;
		adversaryColor = WHITE;
		currentPlayer = player2;
		nextPlayer = player1;
	}
	if(connexion(ipServeur, ipClient, mode, &socketServeur, &socketClient, &sinServeur, &sinClient) == -1)
	{
		printf("connection failed\n");
		return -1;
	}

	if(initializePlayer(player1,myProfil->name,myColor)==-1)
	{
		return -1;
	}
	if(initializePlayer(player2,adversaryProfil->name,adversaryColor)==-1)
	{
		return -1;
	}
	if(initializeBoard(player1,player2)==-1)
	{
		return -1;
	}

	do
	{
		int i,j,k,l;
		int m=0,n=0;
		int isCastling=0;

		int validCommand=0, validMovement=0;

		printfBoard(myColor);
   		printf("%s\n",recordedMoves);
		printf("Pieces captured: ");
		printListPieces(currentPlayer->capuredPieces);

		if(currentPlayer->name == player1->name)
		{
			do
			{
				do
				{
					printf("%s, enter your move:",currentPlayer->name);
					if(readString(currentPlayer->command,MAXCOMMANDSIZE)==-1)
					{
						return -1;
					}
					validCommand = verifyCommand(currentPlayer);
					if(validCommand == -1)
					{
						return -1;
					}
				}while(validCommand!=1);

				i = rankIndexToInt((currentPlayer->command)[1]);
				j = fileIndexToInt((currentPlayer->command)[0]);
				k = rankIndexToInt((currentPlayer->command)[4]);
				l = fileIndexToInt((currentPlayer->command)[3]);

				isCastling = testCastling(currentPlayer,i,j,k,l);
				if(isCastling)
				{
					validMovement = 1;
				}
				else
				{
					validMovement = canMovePiece(currentPlayer,i,j,k,l,&m,&n,0,0);
				}

			}while(!validMovement);

			if(send(socketClient, currentPlayer->command, 100, 0) == -1)
			{
				perror("send:");
				return -1;
			}
		}
		else
		{
			printf("It is %s's turn\n",currentPlayer->name);
			if(recv(socketClient, currentPlayer->command, 100, 0) == -1)
			{
				perror("recv:");
				return -1;
			}
			i = rankIndexToInt(currentPlayer->command[1]);
			j = fileIndexToInt(currentPlayer->command[0]);
			k = rankIndexToInt(currentPlayer->command[4]);
			l = fileIndexToInt(currentPlayer->command[3]);

			isCastling = testCastling(currentPlayer,i,j,k,l);
			if(isCastling)
			{
				validMovement = 1;
			}
			else
			{
				validMovement = canMovePiece(currentPlayer,i,j,k,l,&m,&n,0,0);
			}
		}

		updateRecordedMoves(currentPlayer,recordedMoves);
		if(!isCastling)
		{
			if(cb.array[m][n].isOccupied)
			{
				if(capturePiece(currentPlayer,m,n)==-1)
				{
					return -1;
				}
			}
			movePiece(i,j,k,l);
		}
		else
		{
			castling(i,j,k,l);
		}

		cb.counterMove++;
		updatePosition(cb.counterMove);
		nextPlayer->isChess = testChess(nextPlayer);
		if(nextPlayer->isChess == -1)
		{
			return -1;
		}
		if(nextPlayer->isChess)
		{
			printf("CHESS!\n");
			nextPlayer->isMat = testMat(nextPlayer);
			if(nextPlayer->isChess == -1)
			{
				return -1;
			}
		}
		if(nextPlayer->isMat)
		{
			printf("MAT!!\n");
			endGame = 1;
		}

		temp = currentPlayer;
		currentPlayer = nextPlayer;
		nextPlayer = temp;

	}while(!endGame);

	printfBoard(nextPlayer->color);

	if(player1->isMat)
	{
		printf("%s has won by Mat.\n",player2->name);
	}
	else
	{
		printf("%s has won by Mat.\n",player1->name);
	}

	if(mode == 1)
	{
		if(close(socketServeur) == -1)
		{
			perror("close:");
		}
	}
	if(close(socketClient) == -1)
	{
		perror("close:");
	}

	return 0;
}

int connexion(char* ipServeur, char*ipClient, int mode, int* socketServeur, int* socketClient, struct sockaddr_in* sinServeur, struct sockaddr_in* sinClient)
{
	char buffer[100];
	if(mode == 1)
	{
		int connected = 0;
		socklen_t addrlen = sizeof(sinClient);

		*socketServeur = socket(AF_INET, SOCK_STREAM,0);
		if(*socketServeur == -1)
		{
			perror("socket:");
			return -1;
		}
		sinServeur->sin_family = AF_INET;
		sinServeur->sin_addr.s_addr = htonl(INADDR_ANY);
		sinServeur->sin_port = htons(PORT);

		if(bind(*socketServeur, (struct sockaddr*)sinServeur, sizeof(*sinServeur)) == -1)
		{
			perror("bind:");
			return -1;
		}

		if(listen(*socketServeur,5) == -1)
		{
			perror("listen:");
			return -1;
		}
		else
		{
			printf("Waiting for connexion\n");
		}
		connected = 0;
		do
		{
			*socketClient =  accept(*socketServeur, (struct sockaddr*)sinClient, (socklen_t*) &addrlen );
			if(*socketClient == -1)
			{
				perror("accept:");
				return -1;
			}
			else if(strcmp(inet_ntoa(sinClient->sin_addr),ipClient) != 0)
			{
				snprintf(buffer,100,"refused");
				if(send(*socketClient, buffer, 100, 0) == -1)
				{
					perror("send:");
					return -1;
				}
				if(close(*socketClient) == -1)
				{
					perror("close:");
				}
			}
			else
			{
				snprintf(buffer,100,"accepted");
				if(send(*socketClient, buffer, 100, 0) == -1)
				{
					perror("send:");
					return -1;
				}
				connected =1;
			}

		}while(!connected);
	}
	else
	{
		int socket_error;
		int nbrTentative;

		sinServeur->sin_family = AF_INET;
		sinServeur->sin_addr.s_addr = inet_addr(ipServeur);
		sinServeur->sin_port = htons(PORT);

		*socketClient = socket(AF_INET, SOCK_STREAM, 0);
		if(*socketClient == -1)
		{
			perror("socket:");
			return -1;
		}

		nbrTentative = 0;
		do
		{
			printf("Connection attempt n° %d/10\n",nbrTentative+1);
			socket_error = connect(*socketClient, (struct sockaddr*)sinServeur,  sizeof(*sinServeur));
			if(socket_error == -1)
			{
				sleep(5);
				*socketClient = socket(AF_INET, SOCK_STREAM, 0);
			}
			nbrTentative++;

		}while(socket_error == -1 && nbrTentative < 10);

		if (socket_error == -1)
		{
			perror("connect:");
			return -1;
		}
		else
		{
			if(recv(*socketClient, buffer, 100, 0) == -1)
			{
				perror("recv:");
				return -1;
			}
			if(strcmp("accepted",buffer)!=0)
			{
				printf("Connection refused\n");
				return -1;
			}
			else
			{
				printf("Connection accepted\n");
			}
		}

	}

	return 0;
}

// store string enter in console
int readString(char* string, int sizeMax)
{
	char* posCR = NULL;
	if(fgets(string,sizeMax,stdin)!=NULL)
	{
		posCR = strchr(string,'\n');
		if(posCR != NULL )
		{
			*posCR = '\0';
		}
		else
		{
			freeBuffer();
		}
		return 0;
	}
	else
	{
		freeBuffer();
		return -1;
	}
}

// store integer entered in console
int readInt(int* nbr )
{
	char string[100]="";
	if(fgets(string,100,stdin)!=NULL)
	{
		*nbr = (int) strtol(string,NULL,10);
		return 0;
	}
	else
	{
		return -1;
	}
}

// remove data from stdin buffer
void freeBuffer()
{
	int c = 0;
	do
	{
		c = getchar();
	}while(c!='\n' && c!=EOF);
}
