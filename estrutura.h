#define TAMANHO 11  //Tamanho da tabela de registros (DEVE SER UM NÚMERO PRIMO !)
#define HEADER_OFFSET  0 //Tamanho em bytes do header de arquivo (caso seja utilizado);
#define TAM_CHAVE 20
#define TAM_CONTEUDO 20

///////DEFINES para o status do registro;
#define VAZIO 0
#define OCUPADO 1
#define DELETADO 2

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//Utiliza-se Double Hashing neste exemplo

class Registro{
    public:

        // unsigned  int chave;
        char chave[TAM_CHAVE];
        char conteudo[TAM_CONTEUDO];
        // unsigned  int idade; 
        int status;  // 0 = livre, 1 = ocupado, 2 = deletado porém já utilizado anteriormente
        // char nome[20];

        //Construtor 
		Registro(const char* chave, const char* conteudo, int status);

};
