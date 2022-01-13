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

typedef struct s_IndexKey {
    KEY Id;
    int offset;

} INDEX_KEY;

typedef struct s_IndexName {
    char movieName[50];
    int offset;

} INDEX_NAME;

INDEX_KEY vetorIndicePrincipal[15];
INDEX_NAME vetorIndiceSecundario[15];
INDEX_KEY vetorIndiceAuxiliar[15];

int countIndicePrincipal = 0;
int countIndiceSecundario = 0;
int countIndiceAuxiliar = 0;

char valid = '$';

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
    FILE* indexFileAux;

    char readMovieName[50]; 
    
    int achou = 0;
    int readMovieAuxiliarOffset = 0;
    int readMovieSecondaryOffset = 0;
    int carlinhos = 0;
    int endListOffset = -1;

    if(access("indexSecondaryResult.bin", F_OK ) == 0 ) {
	    indexFile = fopen("indexSecondaryResult.bin", "r+b");
	} else {
	    indexFile = fopen("indexSecondaryResult.bin", "w+b");
	}

    if(access("indexAuxiliarResult.bin", F_OK ) == 0 ) {
	    indexFileAux = fopen("indexAuxiliarResult.bin", "r+b");
	} else {
	    indexFileAux = fopen("indexAuxiliarResult.bin", "w+b");
	}


    while(fread(&readMovieName, sizeof(char), 50, indexFile)) {

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
        //Move os arquivos pro final
        fseek(indexFile, 0, SEEK_END);
        fseek(indexFileAux, 0, SEEK_END);

        //Salva o offset do aux
        readMovieAuxiliarOffset = ftell(indexFileAux);

        //Escreve os dados no aux
        fwrite(&registerData.Id.ClientId, 1, sizeof(int), indexFileAux);
        fwrite(&registerData.Id.MovieId, 1, sizeof(int), indexFileAux);
        fwrite(&endListOffset, 1, sizeof(int), indexFileAux);
        printf("Indice adicionado no final do arquivo de indice auxiliar!\n");
        
        //Escreve os dados no secondary
        fwrite(&registerData.MovieName, 1, sizeof(readMovieName), indexFile);
        fwrite(&readMovieAuxiliarOffset, 1, sizeof(int), indexFile);
        printf("Indice adicionado no final do arquivo de indice secundario!\n");

    } else {
        fseek(indexFileAux, 0, SEEK_END);
        carlinhos = ftell(indexFileAux);

        fwrite(&registerData.Id.ClientId, 1, sizeof(int), indexFileAux);
        fwrite(&registerData.Id.MovieId, 1, sizeof(int), indexFileAux);
        fwrite(&readMovieAuxiliarOffset, 1, sizeof(int), indexFileAux);
        printf("Indice adicionado no final do arquivo de indice auxiliar!\n");

        fseek(indexFile, readMovieSecondaryOffset + 50, SEEK_SET);
        fwrite(&carlinhos, 1, sizeof(int), indexFile);

        printf("Offset Atualizado!\n");
    }

    fclose(indexFile);
    fclose(indexFileAux);
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

    char invalid = '*';
    
    FILE* resultFile;
    
    if(access("dataResult.bin", F_OK ) == 0 ) {
	    resultFile = fopen("dataResult.bin", "r+b");
	} else {
	    resultFile = fopen("dataResult.bin", "w+b");
        fwrite(&invalid, 1, sizeof(char), resultFile);
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

    vetorIndicePrincipal[countIndicePrincipal].Id.ClientId = registerData.Id.ClientId;
    vetorIndicePrincipal[countIndicePrincipal].Id.MovieId = registerData.Id.MovieId;
    vetorIndicePrincipal[countIndicePrincipal].offset = offset;
    countIndicePrincipal++;

    printf("Indice adicionado no final do vetor de indice primario!\n");
    
    int achou = 0;
    int old_offset;

    for (int i = 0; i < countIndiceSecundario; i++) {
        if(strcmp(vetorIndiceSecundario[i].movieName, registerData.MovieName) == 0) {
            achou = 1;
            old_offset = vetorIndiceSecundario[i].offset;
            
            vetorIndiceAuxiliar[countIndiceAuxiliar].Id.ClientId = registerData.Id.ClientId;
            vetorIndiceAuxiliar[countIndiceAuxiliar].Id.MovieId = registerData.Id.MovieId;
            vetorIndiceAuxiliar[countIndiceAuxiliar].offset = old_offset;
            printf("Indice adicionado no final do vetor de indice auxiliar!\n");

            vetorIndiceSecundario[i].offset = countIndiceAuxiliar;
            printf("Offset Atualizado!\n");
            
            break; 
        }
    }

    if(achou == 0) {
        vetorIndiceAuxiliar[countIndiceAuxiliar].Id.ClientId = registerData.Id.ClientId;
        vetorIndiceAuxiliar[countIndiceAuxiliar].Id.MovieId = registerData.Id.MovieId;
        vetorIndiceAuxiliar[countIndiceAuxiliar].offset = -1;

        printf("Indice adicionado no final do arquivo de indice auxiliar!\n");

        strcpy(vetorIndiceSecundario[countIndiceSecundario].movieName, registerData.MovieName);
        vetorIndiceSecundario[countIndiceSecundario].offset = countIndiceAuxiliar;

        printf("Indice adicionado no final do arquivo de indice secundario!\n");

        countIndiceSecundario++;

    }

    countIndiceAuxiliar++;
    
    //InsertPrimaryIndex(registerData, offset);
    //InsertSecondaryIndex(registerData);

    fclose(resultFile);
    return 1;
}

void RefreshFiles() {
    FILE* resultFile;
    FILE* primaryIndexFile;
    FILE* secondaryIndexFile;
    FILE* auxiliarIndexFile;
    
    if(access("dataResult.bin", F_OK ) == 0 ) {
	    resultFile = fopen("dataResult.bin", "r+b");
        fwrite(&valid, 1, sizeof(char), resultFile);
    }
    fclose(resultFile);

    if(access("indexPrimaryResult.bin", F_OK ) == 0 ) {
	    primaryIndexFile = fopen("indexPrimaryResult.bin", "r+b");
	} else {
        primaryIndexFile = fopen("indexPrimaryResult.bin", "w+b");
    }

    for(int i = 0; i < countIndicePrincipal; i++) {
        fwrite(&vetorIndicePrincipal[i], sizeof(vetorIndicePrincipal[i]), 1, primaryIndexFile);
    }
    fclose(primaryIndexFile);
    
    if(access("indexSecondaryResult.bin", F_OK ) == 0 ) {
	    secondaryIndexFile = fopen("indexSecondaryResult.bin", "r+b");
	} else {
        secondaryIndexFile = fopen("indexSecondaryResult.bin", "w+b");
    }
    for(int i = 0; i < countIndiceSecundario; i++) {
        fwrite(&vetorIndiceSecundario[i], sizeof(vetorIndiceSecundario[i]), 1, secondaryIndexFile);
    }
    fclose(secondaryIndexFile);
    
    if(access("indexAuxiliarResult.bin", F_OK ) == 0 ) {
	    auxiliarIndexFile = fopen("indexAuxiliarResult.bin", "r+b");
	} else {
        auxiliarIndexFile = fopen("indexAuxiliarResult.bin", "w+b");
    }
    for(int i = 0; i < countIndiceAuxiliar; i++) {
        fwrite(&vetorIndiceAuxiliar[i], sizeof(vetorIndiceAuxiliar[i]), 1, auxiliarIndexFile);
    }
    fclose(auxiliarIndexFile);
}