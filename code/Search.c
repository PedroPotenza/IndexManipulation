#include "../header.h"

int PrimarySearch(KEY fileData)
{
    FILE* indexFile;
    KEY* readKey;
    int address;
 
	indexFile = fopen("indexPrimaryResult.bin", "r+b");

    int valid = 1;
    int ClientId, MovieId;

    printf("chave Cliente BUSCADA: %d \n", fileData.ClientId);
    printf("chave Filme BUSCADA: %d \n\n", fileData.MovieId);

    while(fread(&ClientId, sizeof(int), 1, indexFile)) {

        fread(&MovieId, sizeof(int), 1, indexFile);

        printf("chave Cliente INTERACAO: %d \n", ClientId);
        printf("chave Filme INTERACAO: %d \n\n", MovieId);

        if(ClientId == fileData.ClientId && MovieId == fileData.MovieId) {
            fread(&address, sizeof(int), 1, indexFile);
            PrintRegister(address);
            return 1;
        } else {
            fseek(indexFile, sizeof(int), SEEK_CUR);
        }
    }

    fclose(indexFile);
    printf("nao foi possivel localizar!\n");
    return 0;
}

