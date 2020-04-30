#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "game.h"
#include "profil.h"

int createProfil(Profil* myProfil)
{
	FILE* file = NULL;

	printf("Saisir votre prénom:");
	readString(myProfil->name, MAXSIZESTR);
	printf("Saisir l'adresse IP:");
	readString(myProfil->IPadress, MAXSIZESTR);
	if((myProfil->friends = createListProfil()) == NULL)
	{
		goto error;
	}

	file = fopen("myprofile.txt","w");
	if(file == NULL)
	{
		perror("fopen");
		goto error;
	}
	fprintf(file,"%s\t%s",myProfil->name, myProfil->IPadress);
	if(errno)
	{
		perror("fprintf");
		goto error;
	}

	fclose(file);
	return 0;

	error:
	if(file!=NULL)
		fclose(file);
	return -1;
}

int loadProfil(Profil* myProfil)
{
	FILE* file = NULL;
	char name[MAXSIZESTR] ="";
	char IPadress[MAXSIZESTR]="";
	int ret = 0;
	Profil friend = {"","",NULL};

	file = fopen("myprofile.txt", "r");
	if(file == NULL)
	{
		perror("fopen");
		goto error;
	}
	fscanf(file,"%s\t%s\n",myProfil->name,myProfil->IPadress);
	if((myProfil->friends = createListProfil()) == NULL)
	{
		goto error;
	}
	do
	{
		ret = fscanf(file,"%s\t%s\n",name,IPadress);
		if(ret != EOF)
		{
			strcpy(friend.name,name);
			strcpy(friend.IPadress,IPadress);
			if(addProfil(myProfil->friends, friend)==-1)
			{
				perror("addprofil:");
				goto error;
			}
		}
	}while(ret != EOF);
	fclose(file);
	return 0;

	error:
	if(file!=NULL)
		fclose(file);
	return -1;
}

int addFriends(Profil* myProfil)
{
	FILE* file = NULL;
	Profil friend = {"","",NULL};

	file = fopen("myprofile.txt", "a");
	if(file == NULL)
	{
		perror("fopen");
		goto error;
	}

	printf("Saisir le nom:");
	readString(friend.name, MAXSIZESTR);
	printf("Saisir l'adresse IP:");
	readString(friend.IPadress, MAXSIZESTR);
	addProfil(myProfil->friends,friend);
	fprintf(file,"%s\t%s\n",friend.name,friend.IPadress);

	fclose(file);
	return 0;

	error:
	if(file!=NULL)
		fclose(file);
	return -1;
}

ListProfil* createListProfil()
{
	ListProfil* newList = malloc(sizeof(*newList));
	if (newList == NULL)
	{
		return NULL;
	}
	newList->first=NULL;
	newList->size=0;

	return newList;
}

int addProfil(ListProfil* list, Profil profil)
{
	ElementProfil* newElement = malloc(sizeof(ElementProfil));
	ElementProfil* ptrEl= NULL;
	if(newElement==NULL || list==NULL)
	{
		return -1;
	}
	newElement->profil=profil;
	newElement->next=NULL;


	if(list->first==NULL)
	{
		list->first=newElement;

	}
	else
	{
		ptrEl = list->first;
		while(ptrEl->next!=NULL)
		{
			ptrEl = ptrEl->next;
		}
		ptrEl->next=newElement;
	}
	list->size++;
	return 0;
}

void printListProfil(ListProfil* list)
{
	ElementProfil* ptrEl= NULL;
	if(list==NULL)
	{
		exit(EXIT_FAILURE);
	}
	else
	{

		ptrEl = list->first;
		while(ptrEl!=NULL)
		{
			printf("name = %s, IP adress = %s\n",ptrEl->profil.name, ptrEl->profil.IPadress);
			ptrEl = ptrEl->next;
		}
		printf("\n");
	}

}
