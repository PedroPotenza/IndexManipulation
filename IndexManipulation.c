#define true 1;
#define false 0;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
    Alunos:
        Gabriel Tetzlaf Mansano - 201150956
        Pedro Henrique Potenza Fernandes - 201151219

*/

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

int Insert(REGISTER registerData);
int PrimarySearch(KEY fileData);

int PrintRegister(int adress);
void fields(char* string);

FILE * readPositions();
void savePosition();

void LoadVetors();
void CreateIndexs(); 

INDEX_KEY vetorIndicePrincipal[15];
INDEX_NAME vetorIndiceSecundario[15];
INDEX_KEY vetorIndiceAuxiliar[15];

int countIndicePrincipal = 0;
int countIndiceSecundario = 0;
int countIndiceAuxiliar = 0;

char valid = '$';

int inseridos = 0;
int buscas_primarias = 0;
int buscas_secundarias = 0;

/*-------------------------------------- Utils --------------------------------------*/
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

void LoadVetors() {

    FILE* primaryIndexFile;
    FILE* secondaryIndexFile;
    FILE* auxiliarIndexFile;

    if(access("indexPrimaryResult.bin", F_OK ) == 0 ) {
	    primaryIndexFile = fopen("indexPrimaryResult.bin", "r+b");
	} else {
        primaryIndexFile = fopen("indexPrimaryResult.bin", "w+b");
    }

	INDEX_KEY aux;
	INDEX_NAME auxName;

    while(fread(&aux, sizeof(INDEX_KEY), 1, primaryIndexFile)) {
		vetorIndicePrincipal[countIndicePrincipal] = aux;
		countIndicePrincipal++;
    }
    fclose(primaryIndexFile);


	if(access("indexSecondaryResult.bin", F_OK ) == 0 ) {
	    secondaryIndexFile = fopen("indexSecondaryResult.bin", "r+b");
	} else {
        secondaryIndexFile = fopen("indexSecondaryResult.bin", "w+b");
    }

    while(fread(&auxName, sizeof(INDEX_NAME), 1, secondaryIndexFile)) {
		vetorIndiceSecundario[countIndiceSecundario] = auxName;
		countIndiceSecundario++;
    }
    fclose(secondaryIndexFile);


	if(access("indexAuxiliarResult.bin", F_OK ) == 0 ) {
	    auxiliarIndexFile = fopen("indexAuxiliarResult.bin", "r+b");
	} else {
        auxiliarIndexFile = fopen("indexAuxiliarResult.bin", "w+b");
    }

	while(fread(&aux, sizeof(INDEX_KEY), 1, auxiliarIndexFile)) {
		vetorIndiceAuxiliar[countIndiceAuxiliar] = aux;
		countIndiceAuxiliar++;
    }
    fclose(auxiliarIndexFile);
    
	printf("Carregou os vetores! \n");

}

void swap(INDEX_KEY *xp, INDEX_KEY *yp)
{
    INDEX_KEY temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(INDEX_KEY arr[], int n)
{
    int i, j, min_idx;

    for (i = 0; i < n-1; i++)
    {
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j].Id.ClientId < arr[min_idx].Id.ClientId)
            min_idx = j;

        swap(&arr[min_idx], &arr[i]);
    }

	for (i = 0; i < n-1; i++)
	{
		min_idx = i;

		for (j = i+1; j < n; j++)
          if (arr[j].Id.ClientId == arr[min_idx].Id.ClientId)
		  	if(arr[j].Id.MovieId < arr[min_idx].Id.MovieId)
            	min_idx = j;

        swap(&arr[min_idx], &arr[i]);
	}
	
}

void selectionNameSort(INDEX_NAME arr[], int n)
{
    int i, j, min_idx;
	INDEX_NAME aux;

    for(i = 0; i < n; i++) {
		for(j = 0; j < n - 1 - i; j++) {

			if(strcmp(arr[j].movieName, arr[j+1].movieName) > 0) {
				aux = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = aux;
			}
			
		}
	}
	
}

void CreateIndexs() {
	printf("Recriando os indices e ja carrega os vetores!\n");

	FILE* dataResult;

	if(access("dataResult.bin", F_OK ) == 0 ) {
	    dataResult = fopen("dataResult.bin", "r+b");
		fseek(dataResult, 1, SEEK_SET);
    }

	int offset;
	int registerSize, stringSize, countRegisters = 0;

	REGISTER registerData;

	while(fread(&registerSize, sizeof(int), 1, dataResult)) {

		fseek(dataResult, -sizeof(int), SEEK_CUR);
		offset = ftell(dataResult);

		vetorIndicePrincipal[countIndicePrincipal].offset = offset;

		fseek(dataResult, sizeof(int), SEEK_CUR);

		fread(&registerData.Id.ClientId, sizeof(int), 1, dataResult);
		fseek(dataResult, sizeof(char), SEEK_CUR);

		//adicionar a informacao no indice primario 
		vetorIndicePrincipal[countIndicePrincipal].Id.ClientId = registerData.Id.ClientId;
		//adicionar a informacao no indice auxiliar
		vetorIndiceAuxiliar[countIndiceAuxiliar].Id.ClientId = registerData.Id.ClientId;

		fread(&registerData.Id.MovieId, sizeof(int), 1, dataResult);
		fseek(dataResult, sizeof(char), SEEK_CUR);

		//adicionar a informacao no indice primario 
		vetorIndicePrincipal[countIndicePrincipal].Id.MovieId = registerData.Id.MovieId;
		//adicionar a informacao no indice auxiliar
		vetorIndiceAuxiliar[countIndiceAuxiliar].Id.MovieId = registerData.Id.MovieId;

		stringSize = registerSize - 2 * sizeof(int) - 2 * sizeof(char);

		char string[stringSize];
    	fread(&string, stringSize, 1, dataResult);

		char* idk;

		idk = strtok(string, "#");
		strcpy(registerData.ClientName, idk);

		idk = strtok(NULL,"#");
		strcpy(registerData.MovieName, idk);

		int achou = 0;
		int old_offset;

		for (int i = 0; i < countIndiceSecundario; i++) {
			if(strcmp(vetorIndiceSecundario[i].movieName, registerData.MovieName) == 0) {
				achou = 1;
				old_offset = vetorIndiceSecundario[i].offset;
            
				vetorIndiceAuxiliar[countIndiceAuxiliar].offset = old_offset;

				break;
			}
		}

		if(achou == 0) {
			vetorIndiceAuxiliar[countIndiceAuxiliar].offset = -1;

			 strcpy(vetorIndiceSecundario[countIndiceSecundario].movieName, registerData.MovieName);
        	vetorIndiceSecundario[countIndiceSecundario].offset = countIndiceAuxiliar;

			countIndiceSecundario++;
		}

		idk = strtok(NULL,"!");
		strcpy(registerData.Genre, idk);

		countIndicePrincipal++;
		countIndiceAuxiliar++;

	}

    fclose(dataResult);

	// ===================== ORDENA OS VETORES ========================= //

	selectionSort(vetorIndicePrincipal, countIndicePrincipal);
	selectionNameSort(vetorIndiceSecundario, countIndiceSecundario);

	// ===================== ESCREVE OS INDEXES ======================== //

	FILE* primaryIndexFile;
    FILE* secondaryIndexFile;
    FILE* auxiliarIndexFile;

    primaryIndexFile = fopen("indexPrimaryResult.bin", "w+b");
    
	for (int i = 0; i < countIndicePrincipal; i++)
	{
		fwrite(&vetorIndicePrincipal[i], 1, sizeof(INDEX_KEY), primaryIndexFile);
	}

    fclose(primaryIndexFile);


	secondaryIndexFile = fopen("indexSecondaryResult.bin", "w+b");
    
	for (int i = 0; i < countIndiceSecundario; i++)
	{
		fwrite(&vetorIndiceSecundario[i], 1, sizeof(INDEX_NAME), secondaryIndexFile);
	}

    fclose(secondaryIndexFile);


	auxiliarIndexFile = fopen("indexPrimaryResult.bin", "w+b");
    
	for (int i = 0; i < countIndiceAuxiliar; i++)
	{
		fwrite(&vetorIndiceAuxiliar[i], 1, sizeof(INDEX_KEY), auxiliarIndexFile);
	}

    fclose(auxiliarIndexFile);

}

/*-------------------------------------- Insert --------------------------------------*/
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
	}

    fwrite(&invalid, 1, sizeof(char), resultFile);

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

/*-------------------------------------- Search --------------------------------------*/
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

int PrimarySearchV2(KEY fileData)
{
    for (int i = 0; i < countIndicePrincipal; i++) {
        if(fileData.ClientId == vetorIndicePrincipal[i].Id.ClientId && fileData.MovieId == vetorIndicePrincipal[i].Id.MovieId){
            PrintRegister(vetorIndicePrincipal[i].offset);

            return 1;
        }
    }

    printf("Nao foi possivel localizar!\n");
    return 0;
}

int SecondarySearch(char movieName[50])
{
    
    KEY* readKey;
    int address;
    int nextAddress = 0;
    int nextOffset = 0;
    int achou = 0;

    int valid = 1;
    char MovieNameInteration[50];

    for (int i = 0; i < countIndiceSecundario; i++)
    {
        if(!strcmp(vetorIndiceSecundario[i].movieName, movieName)) {
            achou = 1;
            
            nextOffset = vetorIndiceSecundario[i].offset;

            while(nextOffset != -1) {
                printf("Codigo do Cliente: %d\n", vetorIndiceAuxiliar[nextOffset].Id.ClientId);
                printf("Codigo do Filme: %d\n", vetorIndiceAuxiliar[nextOffset].Id.MovieId);
                printf("Nome do Filme: %s\n\n", movieName);

                nextOffset = vetorIndiceAuxiliar[nextOffset].offset;
            }
        }
    }

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

    const int buscaSecundariaSize = 15;
    char buscaSecundariaData[buscaSecundariaSize][50];

    for (int i = 0; i < buscaSecundariaSize; i++)
    {
        fread(buscaSecundariaData[i], sizeof(char), 50, file);    
    }
    
    fclose(file);


    FILE* resultFile;
    char valid = '$';
    char invalid = '*';

    if(access("dataResult.bin", F_OK ) == 0 ) {
	    resultFile = fopen("dataResult.bin", "r+b");
    } else {
       resultFile = fopen("dataResult.bin", "w+b");
       fwrite(&valid, 1, sizeof(char), resultFile); 
       rewind(resultFile);
    }

    char marker;
    fread(&marker, sizeof(char), 1, resultFile);

    if(marker == valid)
        LoadVetors();

    if(marker == invalid)
        CreateIndexs();

    fclose(resultFile);


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
    printf(" (4) -> Recriar e ordenar os indices\n");
    printf(" (5) -> Sair\n");
    
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
            found = PrimarySearchV2(buscaPrimariaData[buscas_primarias]);
            
            buscas_primarias++;
            savePosition();
            
            break;

        case 3:
            found = SecondarySearch(buscaSecundariaData[buscas_secundarias]);
            
            buscas_secundarias++;
            savePosition();
            
            break;

        case 4:
            
            CreateIndexs();
            
            break;

        case 5:
            savePosition();
            RefreshFiles();

            printf("Finalizando...\n\n\n");
            repeat = 0;
            break;
        
        default:
            break;
        }
    }

    return 0;
}
