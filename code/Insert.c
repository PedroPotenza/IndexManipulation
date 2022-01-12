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

int InsertPrimaryIndex(REGISTER registerData, int offset)
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

    printf("Indice adicionado no final do arquivo de indice primario!\n");

    fclose(indexFile);
    return 1;
}

int InsertSecondaryIndex(REGISTER registerData)
{
    FILE* indexFile;
    char readMovieName[50]; 
    int achou = 0;
    int readMovieAuxiliarOffset = 0;
    int readMovieSecondaryOffset = 0;
    int testeOffset = 99;
    int testeAuxOffset = 11;

    if(access("indexSecondaryResult.bin", F_OK ) == 0 ) {
	    indexFile = fopen("indexSecondaryResult.bin", "r+b");
	} else {
	    indexFile = fopen("indexSecondaryResult.bin", "w+b");
	}

    //printf("\nNome Filme BUSCADO: %s \n", registerData.MovieName);

    while(fread(&readMovieName, sizeof(char), 50, indexFile)) {
        //printf("nome Filme INTERACAO: %s \n", readMovieName);

        if (!strcmp(readMovieName, registerData.MovieName)) {
            achou = 1;
            fread(&readMovieAuxiliarOffset, sizeof(int), 1, indexFile);
            fseek(indexFile, -(sizeof(int) + sizeof(readMovieName)), SEEK_CUR);
            readMovieSecondaryOffset = ftell(indexFile);
            break;
    
        } else {
            fseek(indexFile, sizeof(int), SEEK_CUR);
        }
    }

    if (achou == 0) {
        fseek(indexFile, 0, SEEK_END);
        fwrite(&registerData.MovieName, 1, sizeof(readMovieName), indexFile);
        fwrite(&testeOffset, 1, sizeof(int), indexFile);

        printf("Indice adicionado no final do arquivo de indice secundario!\n");
    } else {
        fseek(indexFile, readMovieSecondaryOffset + 50, SEEK_SET);
        fwrite(&testeAuxOffset, 1, sizeof(int), indexFile);

        printf("Offset Atualizado!\n");
    }

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

    InsertPrimaryIndex(registerData, offset);
    InsertSecondaryIndex(registerData);

    fclose(resultFile);
    return 1;
}