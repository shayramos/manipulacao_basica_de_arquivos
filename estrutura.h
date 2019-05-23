#define TAMANHO 7  //Tamanho da tabela de registros (DEVE SER UM NÚMERO PRIMO !)
                   //Valor recomendado: 2147483647 (pode ser 67.280.421.310.721 -valor máximo também)
#define HEADER_OFFSET  0 //Tamanho em bytes do header de arquivo (caso seja utilizado);

#define VAZIO 0
#define OCUPADO 1
//#define DELETADO 2

#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class Registro{
    public:
        
        char chave[20];
        char valor[50];
        int status;
        long long int anterior,proximo; //Apontador para próximo registro que tenha mesmo hash (-1 indica ausência de valor seguinte);

        //Construtor 
	    Registro(const char* chave, const char* valor, long long int anterior, long long int proximo,  int status);
         
};
