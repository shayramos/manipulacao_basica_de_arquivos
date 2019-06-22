#ifndef REGISTRO
#define REGISTRO

#include<cstring>
#include<ostream>
#include<memory>

class Registro
{
private:
    char chave[21], conteudo[51];

public:
    Registro(const char *,const char *);
    Registro(Registro *);
    ~Registro();

    //Definicao de operadores de comparação para simplificar codigo
    bool operator <(Registro & other) {
        return strcmp(this->chave, other.chave) < 0;
    }

    bool operator ==(Registro & other) {
        return strcmp(this->chave, other.chave) == 0;;
    }

    bool operator !=(Registro & other) {
        return strcmp(this->chave, other.chave) != 0;;
    }

    bool operator >(Registro & other) {
        return strcmp(this->chave, other.chave) > 0;;
    }

    bool operator <=(Registro & other) {
        return strcmp(this->chave, other.chave) <= 0;;
    }

    bool operator >=(Registro & other) {
        return strcmp(this->chave, other.chave) >= 0;;
    }

    friend std::ostream& operator <<(std::ostream & os, const Registro & reg) {
        os << reg.chave << " " << reg.conteudo;
    }


};

/* Overload do operador >> para podermos extrair registros de arquivos mais facilmente. */
std::istream & operator>> (std::istream & stream, std::shared_ptr<Registro> & registro);

#endif
