#define TAMANHO 3  //Tamanho da tabela de registros
#define HEADER_OFFSET  0 //Tamanho em bytes do header de arquivo (caso seja utilizado);


///////DEFINES para o status do registro;
#define VAZIO 0
#define OCUPADO 1
#define DELETADO 2


#include <iostream>
#include <fstream>

#include <string>
#include <cstring>
using namespace std;

//Utiliza-se Linear Probing (Sondagem Linear) nesse exemplo

class Registro{
    public:
        
        unsigned  int chave; 
        unsigned  int idade; 
        int status;  // 0 = livre, 1 = ocupado, 2 = deletado porém já utilizado anteriormente
        char nome[20];
        

		Registro(unsigned  int chave, unsigned  int idade,  int status, const char* nome);
        
        
};
