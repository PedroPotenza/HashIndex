/*
    Aluno: 
        Pedro Henrique Potenza Fernandes - 201151219
*/
#include "header.h"

/*------------------------------------ Main --------------------------------------*/
int main(int argc, char const *argv[])
{
    
    REGISTER insertData[maxTestCases];
    KEY searchData[maxTestCases];
    KEY removeData[maxTestCases];

    getTestCase(insertData, searchData, removeData);
    readPositions();

    inicializer();

    printf("\n---------- Contador Inicial ----------\n");
    printf("Registros inseridos: %d\n", inserted);
    printf("Buscas efetuadas: %d\n", searched);
    printf("Registros removidos: %d\n\n", removed);

    printf("--------- Menu ---------\n");
    printf(" (1) -> Inserir Registro\n");
    printf(" (2) -> Remover Registro\n");
    printf(" (3) -> Buscar Registro\n");
    printf(" (4) -> Deleta os arquivos\n");
    printf(" (5) -> Sair\n");
    
    int option, repeat = 1;


    while(repeat != 0)
    {
        printf("\n\nOpcao: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:

            insert(insertData[inserted]);
            // printf("Id Cliente: %s\n", insertData[inserted].Id.ClientId);
            // printf("Id Filme: %s\n", insertData[inserted].Id.MovieId);
            // printf("Nome Cliente: %s\n", insertData[inserted].ClientName);
            // printf("Nome Filme: %s\n", insertData[inserted].MovieName);
            // printf("Genero: %s\n\n", insertData[inserted].Genre);
           
            inserted++;
            savePosition();

            break;

        case 2:
            
            // printf("Id Cliente: %s\n", removeData[removed].ClientId);
            // printf("Id Filme: %s\n", removeData[removed].MovieId);

            removed++;
            savePosition();

            break;

        case 3:
            
            // printf("Id Cliente: %s\n", searchData[searched].ClientId);
            // printf("Id Filme: %s\n", searchData[searched].MovieId);

            searched++;
            savePosition();
            
            break;

        case 4:

            reset();
            
            break;

        case 5: 

            savePosition();
            printf("Finalizando...\n\n\n");
            repeat = 0;

            break;
        
        default:
            printf("Opcao invalida.\n");
            break;
        }
    }

    return 0;
}