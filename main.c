/*
    Aluno: 
        Pedro Henrique Potenza Fernandes - 201151219
*/
#define true 1
#define false 0
#define SizeOfREGISTER 156
#define SizeOfINDEXREGISTER 9
#define maxTestCases 15
#define maxRegisters 13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int inserted = 0;
int searched = 0;
int removed  = 0;

typedef struct s_Key {
    char ClientId[3];
    char MovieId[3];
} KEY;

typedef struct s_Register {
    KEY Id;
    char ClientName[50];
    char MovieName[50];
    char Genre[50];
} REGISTER;

typedef struct s_IndexRegister {
    char Id[5];
    int rrn;
} INDEXREGISTER;

void inicializer() {

    if(access("Index.bin", F_OK ) == 0) 
    {
        return;
    } 
    else 
    {
        FILE* file = fopen("Index.bin", "w+b");
        char underline = '_';
        for (int i = 0; i < maxRegisters * SizeOfINDEXREGISTER; i++)
        {
            fwrite(&underline, sizeof(char), 1, file);
        }
	    fclose(file);
    }
}

FILE * fileOpenRead(char * filename) {
	FILE *file = fopen(filename, "rb");
	
	if(file == NULL) {
		printf("O arquivo %s nao pode ser aberto.", filename);
		exit(1);
	}
	
	return file;	
}

void savePosition() {

    char * filename = "position.bin";
	
	FILE * file = fopen(filename, "w+b");

	fwrite(&inserted, sizeof(int), 1, file);
	fwrite(&searched, sizeof(int), 1, file);
	fwrite(&removed, sizeof(int), 1, file);

	fclose(file);
}

void reset() {
    
    remove("Index.bin");
    remove("Positions.bin");
    FILE* file = fopen("Registers.bin", "w+b");
    fclose(file);
    printf("Arquivos resetados!\n");

    inicializer();
    
    inserted = 0;
    searched = 0;
    removed = 0;
}

void readPositions() {

    FILE * file;
	
	if(access("position.bin", F_OK ) == 0) 
    {
        file = fopen("position.bin", "r+b");

        fread(&inserted, sizeof(int), 1, file);
        fread(&searched, sizeof(int), 1, file);
        fread(&removed, sizeof(int), 1, file);

    } 
    else 
    {
        file = fopen("position.bin", "w+b");

        int ZERO = 0;
	    fwrite(&ZERO, sizeof(int), 1, file);
	    fwrite(&ZERO, sizeof(int), 1, file);
	    fwrite(&ZERO, sizeof(int), 1, file);
    }
		
}

void getTestCase(REGISTER* insertData, KEY* searchData, KEY* removeData)
{

    //carrega em memoria principal o vetor insere
    FILE* file = fileOpenRead("CasoDeTeste/insere.bin");
    fread(insertData, sizeof(REGISTER), maxTestCases, file);
    fclose(file);

    //carrega em memoria principal o vetor busca
    file = fileOpenRead("CasoDeTeste/busca.bin");
    fread(searchData, sizeof(KEY), maxTestCases, file);
    fclose(file);

    //carrega em memoria principal o vetor remove
    file = fileOpenRead("CasoDeTeste/remove.bin");
    fread(removeData, sizeof(KEY), maxTestCases, file);
    fclose(file);

}

int getNewRRN() {

    FILE* file = fopen("Registers.bin", "r+b");
    fseek(file, 0, SEEK_END);

    int adress = ftell(file);

    return adress/SizeOfREGISTER;

}

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
            fseek(fileIndex, -1, SEEK_CUR);

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
                        fseek(fileIndex, (rrnIndex + saveTryNumber) * SizeOfINDEXREGISTER, SEEK_SET);
                    } else {
                        fseek(fileIndex, -5, SEEK_CUR);
                    }

                    fwrite(&keyString, sizeof(char), 5, fileIndex);
                    int newRRN = getNewRRN();
                    fwrite(&newRRN, sizeof(int), 1, fileIndex);            
                    written = true;

                    printf("Chave %s inserida com sucesso!\n", keyString);
                    printf("Endereco no Index: %d\n", rrnIndex);
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
            removeIndex(removeData[removed]);

            removed++;
            savePosition();

            break;

        case 3:
            
            // printf("Id Cliente: %s\n", searchData[searched].ClientId);
            // printf("Id Filme: %s\n", searchData[searched].MovieId);
            search(searchData[searched]);

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