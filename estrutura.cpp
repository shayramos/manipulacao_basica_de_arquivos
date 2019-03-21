#include "estrutura.h"


  Registro::Registro( unsigned  int chave, unsigned  int idade,  int status, const char*  nome){
	 this->chave = chave;
	 this->idade = idade;
	 this->status = status;
	 strncpy(this->nome, nome,20);	 
 }

