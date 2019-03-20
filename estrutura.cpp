#include "estrutura.h"
using namespace std;

  Registro::Registro( unsigned  int chave, unsigned  int idade,  int status, const char*  nome){
	 this->chave = chave;
	 this->idade = idade;
	 this->status = status;
	 strcpy(this->nome, nome);
	 
 }

