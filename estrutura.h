#define TAMANHO   7  //Tamanho da tabela de registros (DEVE SER UM NÚMERO PRIMO !)
                   //Valores recomendados: 2147483647, 27644437 (ver referências em https://en.wikipedia.org/wiki/List_of_prime_numbers)
#define HEADER_OFFSET  0 //Tamanho em bytes do header de arquivo (caso seja utilizado);


#define TAM_CHAVE  21
#define TAM_VALOR  51

#define VAZIO 0
#define OCUPADO 1


#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

class Registro{
    public:
        
        char chave[TAM_CHAVE]; //20 bytes de caracteres ASCII + 1 byte para '\0';
        char valor[TAM_VALOR];
        int status;
        long long int anterior,proximo; //Apontador para próximo registro que tenha mesmo hash (-1 indica ausência de valor seguinte);

        //Construtor 
	    Registro(const char* chave, const char* valor, long long int anterior, long long int proximo,  int status);
         
};
