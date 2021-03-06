#ifndef HEADER_H
#define HEADER_H

#define true 1;
#define false 0;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "code/Insert.c"
#include "code/Print.c"
#include "code/Util.c"
#include "code/Search.c"

// int inseridos = 0;
// int buscas_primarias = 0;
// int buscas_secundarias = 0;

// typedef struct s_Key {
//     int ClientId;
//     int MovieId;
// } KEY;

// typedef struct s_Register {
//     KEY Id;
//     char ClientName[50];
//     char MovieName[50];
//     char Genre[50];
// } REGISTER;

int Insert(REGISTER registerData);
int PrimarySearch(KEY fileData);

int PrintRegister(int adress);
void fields(char* string);

FILE * readPositions();
void savePosition();

#endif