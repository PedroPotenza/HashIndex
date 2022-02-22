#include "../header.h"

void removeIndex(KEY removeData) {

    char keyString[5];
    strcpy(keyString, removeData.ClientId);
    strcat(keyString, removeData.MovieId);

    int key = atoi(keyString);

    int rrnIndex = key % maxRegisters;

    FILE* file = fopen("Index.bin", "r+b");
    fseek(file, rrnIndex * SizeOfINDEXREGISTER, SEEK_SET);
    int done = false;
    char keyLocal[5];
    char removeMark = '*';

    do {

        fread(&keyLocal, sizeof(char), 5, file);

        if(keyLocal[0] == '*') { 
            fseek(file, SizeOfINDEXREGISTER-5, SEEK_CUR);
            continue;    
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

            fseek(file, -5, SEEK_CUR);
            for (int i = 0; i < SizeOfINDEXREGISTER; i++)
            {
                fwrite(&removeMark, sizeof(char), 1, file);
            }
            
            
            printf("Chave %s removida!", keyString);
            done = true;
        } 

        fseek(file, sizeof(int), SEEK_CUR);
    } while (!done);

    fclose(file);

}