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

typedef enum s_Vetor_Position {

    INSERIDOS = 0,
    BUSCA_P,
    BUSCA_S

} VETOR_POSITION;

int Insert(REGISTER registerData)
{
    if(registerData.Id.ClientId == 0 || registerData.Id.MovieId == 0) {
        printf("Registro nulo não inserido\n");
        return 0;
    }

    char realMarker = '$';
    char divider = '#';

    
    FILE* resultFile;
    
    if( access("dataResult.bin", F_OK ) == 0 ) {
	    resultFile = fopen("dataResult.bin", "r+b");
	} else {
	    resultFile = fopen("dataResult.bin", "w+b");
	}

    int registerSize = 2 * sizeof(int) + strlen(registerData.ClientName) + strlen(registerData.MovieName) + strlen(registerData.Genre) + 4 * sizeof(char);

    int offset;

    fread(&offset, sizeof(int), 1, resultFile);

    int registerAdress = 0; //findAdressToFit(offset, registerSize, resultFile);
    if (registerAdress == -1)
        fseek(resultFile, 0, SEEK_END);
    else {
        fseek(resultFile, registerAdress + sizeof(int) + 1 * sizeof(char), SEEK_SET);
        int newOffset;
        fread(&newOffset, sizeof(int), 1, resultFile);
        rewind(resultFile);
        fwrite(&newOffset, 1, sizeof(int), resultFile);
        fseek(resultFile, registerAdress, SEEK_SET);
    }

    fwrite(&registerSize, 1, sizeof(int), resultFile);
    fwrite(&realMarker, 1, sizeof(char), resultFile);
    fwrite(&registerData.Id.ClientId, 1, sizeof(int), resultFile);
    fwrite(&divider, 1, sizeof(divider), resultFile);
    fwrite(&registerData.Id.MovieId, 1, sizeof(int), resultFile);
    fwrite(&divider, 1, sizeof(divider), resultFile);
    fwrite(&registerData.ClientName, 1, strlen(registerData.ClientName), resultFile);
    fwrite(&divider, 1, sizeof(divider), resultFile);
    fwrite(&registerData.MovieName, 1, strlen(registerData.MovieName), resultFile);
    fwrite(&divider, 1, sizeof(divider), resultFile);
    fwrite(&registerData.Genre, 1, strlen(registerData.Genre), resultFile);

    if(registerAdress != -1)
        printf("Registro adicionado no byte %d do arquivo!", registerAdress);
    else
        printf("Registro adicionado no final do arquivo!");

    fclose(resultFile);
    return 1;
}