#ifndef CMD_INTERFACE_H
#define CMD_INTERFACE_H

#include "profil.h"

int menuMode(int* mode);
int menuOnline(int* ret, ListProfil* friends);
int menuColor(int* ret);
int menuGame();
int menuCasling(int* ret);
int readString(char* string, int sizeMax);
int readInt();
void freeBuffer();

#endif