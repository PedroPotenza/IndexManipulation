#include "../header.h"

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



