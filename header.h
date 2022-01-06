#ifndef HEADER_H
#define HEADER_H

#define true 1;
#define false 0;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "code/Insert.c"
#include "code/Util.c"

typedef struct s_Key {
    int ClientId;
    int MovieId;
} KEY;

typedef struct s_Register {
    KEY Id;
    char ClientName[50];
    char MovieName[50];
    char Genre[50];
} REGISTER;

typedef enum s_Vetor_Position {

    INSERIDOS = 0,
    BUSCA_P,
    BUSCA_S

} VETOR_POSITION;

int Insert(REGISTER registerData);
FILE * readPositions();
void increasePosition(VETOR_POSITION vetor);


#endif