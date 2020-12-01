#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socket.h"

int serverConnection(char*ipClient, int* socketServeur, int* socketClient, struct sockaddr_in* sinServeur, struct sockaddr_in* sinClient)
{
	char buffer[100];

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
	return 0;

}

int clientConnection(char* ipServeur, int* socketClient, struct sockaddr_in* sinServeur)
{
	char buffer[100];
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
	return 0;
}

