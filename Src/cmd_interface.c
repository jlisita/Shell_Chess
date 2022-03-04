#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "cmd_interface.h"
#include "game.h"

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

int menuGame()
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

int menuCasling(int* ret)
{
	printf("Choose the new piece\n");
	printf("1. Bishop\n");
	printf("2. Knight\n");
	printf("3. Rook\n");
	printf("4. Queen\n");
	if(readInt(ret) == -1)
	{
		return -1;
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
		perror("fgets");
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
		perror("fgets");
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
