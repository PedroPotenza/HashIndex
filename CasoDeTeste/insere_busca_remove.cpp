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
    } vet[5] = { {"01", "12", "Joao", "Filme-12", "Gen-12"},
                 {"01", "25", "Joao", "Filme-25", "Gen-25"},
                 {"01", "38", "Joao", "Filme-38", "Gen-38"},
                 {"01", "38", "Joao", "Filme-38", "Gen-38"},
                 {"01", "51", "Joao", "Filme-51", "Gen-51"},
			   };
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct busca {
        char CodCli[3];
        char CodF[3];
    } vet_b[6] = { {"01","25"},
                   {"01","38"},
                   {"01","12"},
                   {"01","38"},
				   {"01","70"},
				   {"01","51"}};
    
    fd = fopen("busca.bin", "w+b");
    fwrite(vet_b, sizeof(vet_b), 1, fd);
    fclose(fd);    
    
    //////////////////////////////
    struct remove {
        char CodCli[3];
        char CodF[3];
    } vet_r[1] = { {"01","25"}};
       
    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}

