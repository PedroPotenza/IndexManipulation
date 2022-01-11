#include "../header.h"

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

int InsertIndex(REGISTER registerData, int offset)
{
    FILE* indexFile;
    
    if(access("indexPrimaryResult.bin", F_OK ) == 0 ) {
	    indexFile = fopen("indexPrimaryResult.bin", "r+b");
	} else {
	    indexFile = fopen("indexPrimaryResult.bin", "w+b");
	}

    fseek(indexFile, 0, SEEK_END);

    fwrite(&registerData.Id.ClientId, 1, sizeof(int), indexFile);
    fwrite(&registerData.Id.MovieId, 1, sizeof(int), indexFile);
    fwrite(&offset, 1, sizeof(int), indexFile);

    printf("Indice adicionado no final do arquivo de indice!\n");

    fclose(indexFile);
    return 1;
}

int Insert(REGISTER registerData)
{
    if(registerData.Id.ClientId == 0 || registerData.Id.MovieId == 0) {
        printf("Registro nulo nao inserido\n");
        return 0;
    }

    char divider = '#';
    char finalizer = '!';
    
    FILE* resultFile;
    
    if(access("dataResult.bin", F_OK ) == 0 ) {
	    resultFile = fopen("dataResult.bin", "r+b");
	} else {
	    resultFile = fopen("dataResult.bin", "w+b");
	}

    int registerSize = 2 * sizeof(int) + strlen(registerData.ClientName) + strlen(registerData.MovieName) + strlen(registerData.Genre) + 5 * sizeof(char);

    fseek(resultFile, 0, SEEK_END);

    int offset = ftell(resultFile);

    fwrite(&registerSize, 1, sizeof(int), resultFile);
    fwrite(&registerData.Id.ClientId, 1, sizeof(int), resultFile);
    fwrite(&divider, 1, sizeof(divider), resultFile);
    fwrite(&registerData.Id.MovieId, 1, sizeof(int), resultFile);
    fwrite(&divider, 1, sizeof(divider), resultFile);
    fwrite(&registerData.ClientName, 1, strlen(registerData.ClientName), resultFile);
    fwrite(&divider, 1, sizeof(divider), resultFile);
    fwrite(&registerData.MovieName, 1, strlen(registerData.MovieName), resultFile);
    fwrite(&divider, 1, sizeof(divider), resultFile);
    fwrite(&registerData.Genre, 1, strlen(registerData.Genre), resultFile);
    fwrite(&finalizer, 1, sizeof(divider), resultFile);
    printf("Registro adicionado no final do arquivo!\n");

    InsertIndex(registerData, offset);

    fclose(resultFile);
    return 1;
}