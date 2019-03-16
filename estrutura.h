#ifndef __estrutura__
#define __estrutura__ 1
#include <iostream>
#include <fstream>
#include <string>
#define TAMANHO 20

class RegisterData{
    private:
        unsigned int chave;
        std::string nome;
        unsigned int idade;
    public:
        RegisterData(unsigned int v1, std::string v2, unsigned int v3) {
            this->chave = v1;
            this->nome = v2;
            this->idade = v3;
        }

        RegisterData(std::ifstream &stream) {
            this->consultar(stream);
        }

        void inserir(std::ofstream &stream){
            stream << this->chave << std::endl;
            stream << this->nome << std::endl;
            stream << this->idade << std::endl;
        }

        void consultar(std::ifstream &stream){
            stream >> this->chave;
            stream >> this->nome;
            stream >> this->idade;
        }

        void remover(){

        }

        void imprimir(){
            std::cout << this->chave << std::endl;
            std::cout << this->nome << std::endl;
            std::cout << this->idade << std::endl;
        }
};

#endif  // __estrutura__
