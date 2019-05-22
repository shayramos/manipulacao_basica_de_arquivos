#include "estrutura.h"

//Construtor de instâncias de Registro
  Registro::Registro( const char* chave, const char*  valor, long long anterior,long long proximo, int status){
	
	this->status = status;
	this->anterior = anterior;
	this->proximo = proximo;
	strncpy(this->chave, chave,20);
	strncpy(this->valor, valor,50);
 } 

