#include "../header.h"


void printRegister(int rrn) {

    int adress = (rrn * SizeOfREGISTER);

    FILE* dataFile = fopen("Registers.bin", "r+b");
    fseek(dataFile, adress, SEEK_SET);

    REGISTER registerLocal;
    fread(&registerLocal.Id.ClientId, 3, sizeof(char), dataFile);
    fread(&registerLocal.Id.MovieId, 3, sizeof(char), dataFile);
    fread(&registerLocal.ClientName, 50, sizeof(char), dataFile);
    fread(&registerLocal.MovieName, 50, sizeof(char), dataFile);
    fread(&registerLocal.Genre, 50, sizeof(char), dataFile);
    
    printf("Id Cliente: %s\n", registerLocal.Id.ClientId);
    printf("Id Filme: %s\n", registerLocal.Id.MovieId);
    printf("Nome Cliente: %s\n", registerLocal.ClientName);
    printf("Nome Filme: %s\n", registerLocal.MovieName);
    printf("Genero: %s\n\n", registerLocal.Genre);

}

void search(KEY searchData) {

    char keyString[5];
    strcpy(keyString, searchData.ClientId);
    strcat(keyString, searchData.MovieId);

    int key = atoi(keyString);

    int rrnIndex = key % maxRegisters;

    FILE* file = fopen("Index.bin", "r+b");
    fseek(file, rrnIndex * SizeOfINDEXREGISTER, SEEK_SET);

    char keyLocal[5];
    int done = false;
    int try = 0;

    do {
        
        fread(&keyLocal, sizeof(char), 5, file);

        if(keyLocal[0] == '*') { 
            fseek(file, SizeOfINDEXREGISTER-5, SEEK_CUR);    
        }

        if(keyLocal[0] == '_') {
            printf("Nao foi possivel localizar a chave %s!\n", keyString);
            fclose(file);
            return;
        }

        int keyLocalInt = atoi(keyLocal);
        int LocalRRNIndex = keyLocalInt % maxRegisters;

        if(LocalRRNIndex != rrnIndex) {
            printf("Nao foi possivel localizar a chave %s!\n", keyString);
            fclose(file);
            return;
        }

        if(strcmp(keyLocal, keyString) == 0){

                
            printf("Chave %s encontrada, endereco %d, %d acessos!\n", keyString, rrnIndex+try, try);
            int rrn; 
            fread(&rrn, sizeof(int), 1, file);
            printRegister(rrn);
            done = true;
            fclose(file);
            return;
        } 

        fseek(file, sizeof(int), SEEK_CUR);
        try++;
    } while (!done);

    fclose(file);

}