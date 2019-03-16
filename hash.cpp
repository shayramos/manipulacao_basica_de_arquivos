#include "estrutura.h"
#include "hash.h"

using namespace std;
  
Hash::Hash(int b) 
{ 
    this->tamanho = b; 
    lista = new list<RegisterData>[tamanho]; 
} 
  
void Hash::inserirRegistro(RegisterData registro) 
{ 
    int index = hashFunction(registro.chave); 
    lista[index].push_back(registro);
}

void Hash::deleteRegistro(RegisterData registro) 
{ 
  int index = hashFunction(registro.chave); 
  
  list <RegisterData> :: iterator i; 
  for (i = lista[index].begin(); i != lista[index].end(); i++) { 
    if (*i == (int)registro.chave)
        break;
  }
  
  if (i != lista[index].end()) 
    lista[index].erase(i); 
}

int main(){

}