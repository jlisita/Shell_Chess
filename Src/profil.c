#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
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

	file = fopen("myprofile.txt","w+");
	if(file == NULL)
	{
		perror("fopen");
		goto error;
	}
	fprintf(file,"%s\t%s\n",myProfil->name, myProfil->IPadress);
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
		if(errno != 2)
		{
			perror("fopen");
		}
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

Profil* getProfil(ListProfil* list, int n)
{
	if(n >= list->size || list == NULL)
	{
		return NULL;
	}
	int i = 0;
	ElementProfil* ptrEl= NULL;
	ptrEl = list->first;
	if(list->size == 1)
	{
		return &(list->first->profil);
	}
	while(i<n)
	{
		printf(" %d. %s\tIP: %s\n",i,ptrEl->profil.name, ptrEl->profil.IPadress);
		ptrEl = ptrEl->next;
		i++;
	}
	return &(ptrEl->profil);
}

int printListProfil(ListProfil* list)
{
	ElementProfil* ptrEl= NULL;
	if(list==NULL)
	{
		return -1;
	}
	else
	{
		int i =1;
		ptrEl = list->first;
		while(ptrEl!=NULL)
		{
			printf(" %d. %s\tIP: %s\n",i,ptrEl->profil.name, ptrEl->profil.IPadress);
			ptrEl = ptrEl->next;
			i++;
		}
		printf("\n");
	}
	return 0;
}
