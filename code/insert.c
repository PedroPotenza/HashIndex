#include "../header.h"

void writeRegister(REGISTER registerData) {

    FILE* file = fopen("Registers.bin", "r+b");
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

    int rrnIndex = key % maxRegisters;

    FILE* fileIndex = fopen("Index.bin", "r+b");
    fseek(fileIndex, rrnIndex * SizeOfINDEXREGISTER, SEEK_SET);

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
            printf("Endereco no Index: %d\n", rrnIndex);
            printf("RRN da Chave no arquivo registros: %d\n", newRRN);
            printf("Chave inserida com %d acessos\n", trys);

            writeRegister(registerData);

        } else {

            printf("Colisao\n");

            

            int trying = true;
            int existRemovedInTheStack = false;
            int saveTryNumber = 0;
            int rotate = false;
            fseek(fileIndex, -1, SEEK_CUR);

            if(rrnIndex == 12) {
                rotate = true;
                fseek(fileIndex, 0, SEEK_SET);
                trys++;
            }

            while(trying) {
    
                char keyLocal[5];
                fread(&keyLocal, sizeof(char), 5, fileIndex);

                trys++;

                int keyLocalInt = atoi(keyLocal);
                int rrnIndexLocal = keyLocalInt % maxRegisters;

                if(keyLocal[0] == '*') {

                    if(existRemovedInTheStack == false){ 
                      existRemovedInTheStack = true;
                      saveTryNumber = trys-1;  
                    }

                    fseek(fileIndex, sizeof(int), SEEK_CUR);
                    continue;

                }
                
                if(keyLocal[0] == '_'){

                    if(existRemovedInTheStack == true){
                        
                        if(rotate)
                            fseek(fileIndex, (saveTryNumber-1) * SizeOfINDEXREGISTER, SEEK_SET);
                        else
                            fseek(fileIndex, (rrnIndex + saveTryNumber) * SizeOfINDEXREGISTER, SEEK_SET);
                    } else {
                        fseek(fileIndex, -5, SEEK_CUR);
                    }

                    fwrite(&keyString, sizeof(char), 5, fileIndex);
                    int newRRN = getNewRRN();
                    fwrite(&newRRN, sizeof(int), 1, fileIndex);            
                    written = true;

                    printf("Chave %s inserida com sucesso!\n", keyString);
                    if(rotate)
                        if(existRemovedInTheStack)
                            printf("Endereco no Index: %d\n", saveTryNumber-1);
                        else 
                            printf("Endereco no Index: %d\n", trys-2);
                    else
                        printf("Endereco no Index: %d\n", rrnIndex+(trys-1));
                    printf("RRN da Chave no arquivo registros: %d\n", newRRN);
                   
                   if(existRemovedInTheStack == true) { 
                        printf("Chave inserida com %d acessos\n", saveTryNumber);
                   } else {
                        printf("Chave inserida com %d acessos\n", trys-1);
                   }

                    writeRegister(registerData);
                    written = true;
                    fclose(fileIndex);
                    return;

                }

                if(rrnIndexLocal != rrnIndex) {
                    fseek(fileIndex, sizeof(int), SEEK_CUR);
                    continue;
                }
   
                if(strcmp(keyLocal, keyString) == 0){
                    printf("Chave %s Duplicada", keyString);
                    fclose(fileIndex);
                    return;
                } 

                fseek(fileIndex, sizeof(int), SEEK_CUR);
            }   
        }
    } while(written == false);

    fclose(fileIndex);
}