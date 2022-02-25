#include<stdio.h>
#include<conio.h>
#include<string.h>

int main() {
    FILE *fd;
    
    //////////////////////////////   
    struct CliF {
        char CodCli[3];
        char CodF[3];
        char NomeCli[50];
        char NomeF[50];
        char Genero[50];
    } vet[5] = { {"00", "12", "Nome-00", "Filme-12", "Gen-12"},
                 {"00", "25", "Nome-00", "Filme-25", "Gen-25"},
                 {"00", "38", "Nome-00", "Filme-38", "Gen-38"},
                 {"00", "38", "Nome-00", "Filme-38", "Gen-38"},
                 {"00", "51", "Nome-00", "Filme-51", "Gen-51"},
			   };
    
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct busca {
        char CodCli[3];
        char CodF[3];
    } vet_b[7] = { {"00","25"},
                   {"00","38"},
                   {"00","12"},
                   {"00","38"},
                   {"00","38"},
				   {"00","70"},
				   {"00","51"}};
    
    fd = fopen("busca.bin", "w+b");
    fwrite(vet_b, sizeof(vet_b), 1, fd);
    fclose(fd);    
    
    //////////////////////////////
    struct remove {
        char CodCli[3];
        char CodF[3];
    } vet_r[1] = { {"00","25"}};
       
    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}

