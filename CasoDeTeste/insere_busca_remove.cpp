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
    } vet[13] = { {"01", "12", "Joao", "Filme-12", "Mod=8"},
                 {"01", "25", "Joao", "Filme-25", "Mod=8"},
                 {"01", "38", "Joao", "Filme-38", "Mod=8"},
                 {"01", "38", "Joao", "Filme-38", "Mod=8"}, //Duplicado
                 {"01", "51", "Joao", "Filme-51", "Mod=8"},
                 {"02", "52", "Atenor", "Cuphead", "Mod=5"},
                 {"02", "62", "Atenor", " Filme Tutorial do YouTube 100 por cento", "Mod=2"},
                 {"02", "62", "Atenor", "Filme Tutorial do YouTube 100 por cento", "Mod=2"}, //Duplicado
                 {"03", "08", "James", "008 - Sem tempo para copyright", "Mod=9"},
                 {"52", "00", "Mario", "A historia da pipa", "Mod=0"},
                 {"52", "01", "Mario", "O ceu estrelado", "Mod=1"},
                 {"63", "01", "Luigi", "O ceu estrelado", "Mod=9"},
                 {"63", "00", "Luigi", "A historia da pipa", "Mod=8"}
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
    } vet_r[6] = { {"01","25"},
                   {"01", "51"},
                   {"01", "38"},
                   {"01","25"},
                   {"52", "01"},
                   {"02", "52"}};
       
    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}