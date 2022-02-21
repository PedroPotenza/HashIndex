#include "../header.h"

void writeRegister(REGISTER registerData) {

    FILE* file = fopen("registers.bin", "r+b");
    fseek(file, 0, SEEK_END);

    fwrite(&registerData.Id.ClientId, 3, sizeof(char), file);
    fwrite(&registerData.Id.MovieId, 3, sizeof(char), file);
    fwrite(&registerData.ClientName, 50, sizeof(char), file);
    fwrite(&registerData.MovieName, 50, sizeof(char), file);
    fwrite(&registerData.Genre, 50, sizeof(char), file);

    fclose(file);

}

void insert(REGISTER registerData) {

    char keyString[5];
    strcpy(keyString, registerData.Id.ClientId);
    strcat(keyString, registerData.Id.MovieId);

    int key = atoi(keyString);

    int rrn = key % maxRegisters;


    FILE* fileIndex = fopen("Index.bin", "r+b");
    fseek(fileIndex, rrn * SizeOfINDEXREGISTER, SEEK_SET);

    char free;
    int written = false;
    int trys = 0;

    do {
        fread(&free, sizeof(char), 1, fileIndex);
        if(free == '_') {

            fseek(fileIndex, -1, SEEK_CUR);
            fwrite(&keyString, sizeof(char), 5, fileIndex);
            int newRRN = getNewRRN();
            fwrite(&newRRN, sizeof(int), 1, fileIndex);            
            written = true;

            printf("Chave %s inserida com sucesso!\n", keyString);
            printf("RRN da Chave no arquivo registros: %d\n", newRRN);
            printf("Chave inserida com %d acessos\n", trys);

            writeRegister(registerData);

        } else {

            char keyLocal[5];
            fread(&keyLocal, sizeof(char), 5, fileIndex);

            if(strcmp(keyLocal, keyString) == 0){
                printf("Chave %s Duplicada", keyString);
                return;
            } 

        }
    } while(written == false);

    fclose(fileIndex);
}