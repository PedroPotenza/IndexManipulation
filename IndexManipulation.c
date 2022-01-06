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

    //carrega em memoria principal o vetor busca_p
    FILE* file = fileOpenRead("busca_p.bin");

    KEY* buscaPrimariaData;
    int buscaPrimariaSize = 15;

    buscaPrimariaData = (KEY*) malloc(buscaPrimariaSize * sizeof(KEY));

    fread(buscaPrimariaData, sizeof(KEY), buscaPrimariaSize, file);
    fclose(file);

    //carrega em memoria principal o vetor busca_s

    /*
        entender como é para receber isso
    */

    // FILE* file = fileOpenRead("insere.bin");

    // char* buscaSecundariaData;
    // int buscaSecundariaSize = 15;

    // buscaSecundariaData = (REGISTER*) malloc(insertSize * sizeof(REGISTER));

    // fread(buscaSecundariaData, sizeof(REGISTER), insertSize, file);
    // fclose(file);
    

    file = readPositions();
    
    int inseridos = 0;
    int buscas_primarias = 0;
    int buscas_secundarias = 0;

    fread(&inseridos, sizeof(short int), 1, file);
    fread(&buscas_primarias, sizeof(short int), 1, file);
    fread(&buscas_secundarias, sizeof(short int), 1, file);

    printf("\n---------- Contador Inicial ----------\n");
    printf("\tRegistros inseridos: %hd\n", inseridos);
    printf("\tBuscas Primarias: %hd\n", buscas_primarias);
    printf("\tBuscas Secundarias: %hd\n\n", buscas_secundarias);

    fclose(file);

    printf("--------- Menu ---------\n");
    printf(" (1) -> Inserir Registro\n");
    printf(" (2) -> Busca Primaria\n");
    printf(" (3) -> Busca Secundaria\n");
    printf(" (4) -> Sair\n");
    
    int option, repeat, inserted = true;
    while(repeat)
    {

        printf("\n\nOpcao: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            
            inserted = Insert(insertData[inseridos]);
            
            break;

        case 2:
            
            //busca primaria
            
            break;

        case 3:
            
            //busca secundaria

            break;

        case 4:
            printf("Finalizando...\n\n\n");
            repeat = false;
            break;
        
        default:
            break;
        }
    }

    return 0;
}
