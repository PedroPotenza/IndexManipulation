#include "../header.h"
void fields(char* string)
{

    char* idk;

    idk = strtok(string, "#");
    printf("Nome do Cliente: %s\n", idk);

    idk = strtok(NULL,"#");
    printf("Nome do Filme: %s\n\n", idk);

    idk = strtok(NULL,"|");
    printf("Genero: %s\n\n", idk);
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

    stringSize = registerSize - 4 * sizeof(int) + 2 * sizeof(char);

    char* string;
    fread(&string, sizeof(char), stringSize, resultFile);

    fields(string); 

    fclose(resultFile);
    return 0;
}

int PrimarySearch(KEY fileData)
{
    FILE* indexFile;
    KEY* readKey;
    int address;
    
    if(access("indexPrimaryResult.bin", F_OK ) == 0 ) {
	    indexFile = fopen("indexPrimaryResult.bin", "r+b");
	} else {
	    indexFile = fopen("indexPrimaryResult.bin", "w+b");
	}

    fseek(indexFile, 0, SEEK_SET);

    while(fread(readKey, sizeof(KEY), 1, indexFile)) {
        if(readKey->ClientId == fileData.ClientId && readKey->MovieId == fileData.MovieId) {
            fread(&address, sizeof(int), 1, indexFile);
            return 1;
        } else {
            fseek(indexFile, sizeof(int), SEEK_CUR);
        }
    }

    fclose(indexFile);
    PrintRegister(address);
    return 0;
}

