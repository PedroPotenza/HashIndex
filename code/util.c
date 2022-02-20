#include "../header.h"

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
    remove("Registers.bin");
    printf("Arquivos resetados!\n");
    
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