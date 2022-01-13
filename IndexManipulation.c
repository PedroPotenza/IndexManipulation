#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
    Alunos:
        Gabriel Tetzlaf Mansano - 201150956
        Pedro Henrique Potenza Fernandes - 201151219

*/

/*------------------------------------ Defines --------------------------------------*/
#define true 1;
#define false 0;

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

int Insert(REGISTER registerData);
int PrimarySearch(KEY fileData);
int PrintRegister(int adress);
void fields(char* string);
void savePosition();

FILE * readPositions();

/*------------------------------------ Global --------------------------------------*/
int inseridos = 0;
int buscas_primarias = 0;
int buscas_secundarias = 0;

/*------------------------------------ Util --------------------------------------*/
FILE * fileOpenRead(char * filename) {
	FILE *file = fopen(filename, "rb");
	
	if(file == NULL) {
		printf("O arquivo %s nao pode ser aberto.", filename);
		exit(1);
	}
	
	return file;	
}

FILE * readPositions() {
	char * filename = "position.bin";
	int ZERO = 0;
	
	if(access(filename, F_OK ) == 0)
		return fopen(filename, "r+b");
		
	FILE * file = fopen(filename, "w+b");

	fwrite(&ZERO, sizeof(int), 1, file);
	fwrite(&ZERO, sizeof(int), 1, file);
	fwrite(&ZERO, sizeof(int), 1, file);
	
	return file;
}

void savePosition() {

    char * filename = "position.bin";
	
	FILE * file = fopen(filename, "w+b");

	fwrite(&inseridos, sizeof(int), 1, file);
	fwrite(&buscas_primarias, sizeof(int), 1, file);
	fwrite(&buscas_secundarias, sizeof(int), 1, file);

	fclose(file);
}

/*------------------------------------ Insert --------------------------------------*/
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

/*------------------------------------ Search --------------------------------------*/
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
/*------------------------------------ Print --------------------------------------*/
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

/*------------------------------------ Main --------------------------------------*/
int main(int argc, char const *argv[])
{
    
    //carrega em memoria principal o vetor insere
    FILE* file = fileOpenRead("insere.bin");

    REGISTER* insertData;
    int insertSize = 15;

    insertData = (REGISTER*) malloc(insertSize * sizeof(REGISTER));

    fread(insertData, sizeof(REGISTER), insertSize, file);
    fclose(file);

    file = fileOpenRead("busca_p.bin");

    KEY* buscaPrimariaData;
    int buscaPrimariaSize = 15;

    buscaPrimariaData = (KEY*) malloc(buscaPrimariaSize * sizeof(KEY));

    fread(buscaPrimariaData, sizeof(KEY), buscaPrimariaSize, file);
    fclose(file);

    file = fileOpenRead("busca_s.bin");

    int buscaSecundariaSize = 15;
    char buscaSecundariaData[buscaSecundariaSize][50];

    for (int i = 0; i < buscaSecundariaSize; i++)
    {
        fread(buscaSecundariaData[i], sizeof(char), 50, file);    
    }
    
    fclose(file);

    file = readPositions();

    fread(&inseridos, sizeof(int), 1, file);
    fread(&buscas_primarias, sizeof(int), 1, file);
    fread(&buscas_secundarias, sizeof(int), 1, file);

    printf("\n---------- Contador Inicial ----------\n");
    printf("\tRegistros inseridos: %d\n", inseridos);
    printf("\tBuscas Primarias: %d\n", buscas_primarias);
    printf("\tBuscas Secundarias: %d\n\n", buscas_secundarias);

    fclose(file);

    printf("--------- Menu ---------\n");
    printf(" (1) -> Inserir Registro\n");
    printf(" (2) -> Busca Primaria\n");
    printf(" (3) -> Busca Secundaria\n");
    printf(" (4) -> Sair\n");
    
    int option, repeat = 1, inserted = 0, found = 0;
    while(repeat != 0)
    {
        printf("\n\nOpcao: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            
            inserted = Insert(insertData[inseridos]);
            // if(inserted == 1){
            inseridos++;
            savePosition();
            // }
            break;

        case 2:
            found = PrimarySearch(buscaPrimariaData[buscas_primarias]);
            
            buscas_primarias++;
            savePosition();
            
            break;

        case 3:
            found = SecondarySearch(buscaSecundariaData[buscas_secundarias]);
            
            buscas_secundarias++;
            savePosition();
            
            break;

        case 4:
            savePosition();
            printf("Finalizando...\n\n\n");
            repeat = 0;
            break;
        
        default:
            break;
        }
    }

    return 0;
}