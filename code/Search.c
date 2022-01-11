#include "../header.h"

void fields(char* string)
{

    char* idk;

    idk = strtok(string, "#");
    printf("Nome do Cliente: %s\n", idk);

    idk = strtok(NULL,"#");
    printf("Nome do Filme: %s\n", idk);

    idk = strtok(NULL,"!");
    printf("Genero: %s\n", idk);
}

int PrintRegister(int adress)
{
    FILE* resultFile;
    REGISTER* readRegister;
    int registerSize, stringSize;
    
    if(access("dataResult.bin", F_OK ) == 0 ) {
	    resultFile = fopen("dataResult.bin", "r+b");
	} else {
	    resultFile = fopen("dataResult.bin", "w+b");
	}

    fseek(resultFile, adress, SEEK_SET);
    fread(&registerSize, sizeof(int), 1, resultFile);
   
    int DataInt;
    fread(&DataInt, sizeof(int), 1, resultFile);
    printf("Codigo do Cliente: %d\n", DataInt);
    fseek(resultFile, 1, SEEK_CUR);

    fread(&DataInt, sizeof(int), 1, resultFile);
    printf("Codigo do Filme: %d\n", DataInt);
    fseek(resultFile, 1, SEEK_CUR);

    stringSize = registerSize - 2 * sizeof(int) - 2 * sizeof(char);

    char string[stringSize];
    fread(&string, stringSize, 1, resultFile);
    fields(string); 

    fclose(resultFile);
    return 0;
}

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

