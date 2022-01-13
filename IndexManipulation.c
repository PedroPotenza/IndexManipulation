/*
    Alunos:
        Gabriel Tetzlaf Mansano - 201150956
        Pedro Henrique Potenza Fernandes - 201151219

*/

#include "header.h"
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
