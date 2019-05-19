#include "estrutura.h"

//Construtor de instâncias de Registro
  Registro::Registro(const char* chave, const char* conteudo, int status){
	//  this->chave = chave;
	 strncpy(this->chave, chave,20);
	//  this->idade = idade;
	 strncpy(this->conteudo, conteudo,50);	 
	 this->status = status;
 }

