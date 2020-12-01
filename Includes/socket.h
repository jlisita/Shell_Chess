#ifndef SOCKET_H
#define SOCKET_H

#define PORT 1025

int serverConnection(char*ipClient, int* socketServeur, int* socketClient, struct sockaddr_in* sinServeur, struct sockaddr_in* sinClient);
int clientConnection(char* ipServeur, int* socketClient, struct sockaddr_in* sinServeur);


#endif