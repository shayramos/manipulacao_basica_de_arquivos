#define TAMANHO 11  //Tamanho da tabela de registros (DEVE SER UM NÚMERO PRIMO !)
#define HEADER_OFFSET  0 //Tamanho em bytes do header de arquivo (caso seja utilizado);

///////DEFINES para o status do registro;
#define VAZIO 0
#define OCUPADO 1
#define DELETADO 2


#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

//Utiliza-se Double Hashing neste exemplo

class Registro{
    public:
        
        // unsigned  int chave;
        char chave[20];
        char conteudo[50];
        // unsigned  int idade; 
        int status;  // 0 = livre, 1 = ocupado, 2 = deletado porém já utilizado anteriormente
        // char nome[20];
        
        //Construtor 
		Registro(const char* chave, const char* conteudo, int status);
         
};
