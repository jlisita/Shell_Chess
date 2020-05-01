#ifndef PROFIL_H
#define PROFIL_H

#define MAXSIZESTR 100

typedef struct ListProfil ListProfil;
struct ListProfil;

typedef struct Profil Profil;
struct Profil
{
	char name[MAXSIZESTR];
	char IPadress[MAXSIZESTR];
	ListProfil* friends;
};

typedef struct ElementProfil ElementProfil;
struct ElementProfil
{
	Profil profil;
	ElementProfil* next;
};

struct ListProfil
{
	ElementProfil* first;
	int size;
};

int createProfil(Profil* myProfil);
int loadProfil(Profil* myProfil);
int addFriends(Profil* myProfil);
ListProfil* createListProfil();
int addProfil(ListProfil* list, Profil profil);
Profil* getProfil(ListProfil* list, int n);
int printListProfil(ListProfil* list);

#endif
