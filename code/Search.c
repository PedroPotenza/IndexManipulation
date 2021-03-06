#include "../header.h"

int PrimarySearch(KEY fileData)
{
    FILE* indexFile;
    KEY* readKey;
    int address;
 
	indexFile = fopen("indexPrimaryResult.bin", "r+b");

    int ClientId, MovieId;

    while(fread(&ClientId, sizeof(int), 1, indexFile)) {

        fread(&MovieId, sizeof(int), 1, indexFile);

        if(ClientId == fileData.ClientId && MovieId == fileData.MovieId) {
            fread(&address, sizeof(int), 1, indexFile);
            PrintRegister(address);
            return 1;
        } else {
            fseek(indexFile, sizeof(int), SEEK_CUR);
        }
    }

    fclose(indexFile);
    printf("Nao foi possivel localizar!\n");
    return 0;
}

int SecondarySearch(char movieName[50])
{
    FILE* secondaryIndexFile;
    FILE* auxiliarIndexFile;

    KEY* readKey;
    int address;
    int nextAddress = 0;
    int achou = 0;
 
	secondaryIndexFile = fopen("indexSecondaryResult.bin", "r+b");
    auxiliarIndexFile = fopen("indexAuxiliarResult.bin", "r+b");

    int valid = 1;
    char MovieNameInteration[50];

    while(fread(&MovieNameInteration, sizeof(char), 50, secondaryIndexFile)) {

        if(!strcmp(MovieNameInteration, movieName)) {
            achou = 1;
            fread(&address, sizeof(int), 1, secondaryIndexFile);
                        
            nextAddress = address;

            while(nextAddress != -1) {
                fseek(auxiliarIndexFile, nextAddress, SEEK_SET);

                fread(&readKey->ClientId, sizeof(int), 1, auxiliarIndexFile);
                fread(&readKey->MovieId, sizeof(int), 1, auxiliarIndexFile);
                fread(&nextAddress, sizeof(int), 1, auxiliarIndexFile);

                printf("Codigo do Cliente: %d\n", readKey->ClientId);
                printf("Codigo do Filme: %d\n", readKey->MovieId);
                printf("Nome do Filme: %s\n\n", movieName);

            }          
        } else {
            fseek(secondaryIndexFile, sizeof(int), SEEK_CUR);
        }
    }

    fclose(secondaryIndexFile);
    fclose(auxiliarIndexFile);
    if (achou == 1) {
        return 1;
    } else {
        printf("Nao foi possivel localizar!\n");
        return 0;
    }
}
