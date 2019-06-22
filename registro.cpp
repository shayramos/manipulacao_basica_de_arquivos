#include "registro.hpp"


Registro::Registro(const char * ch, const char * cont)
{
    strncpy(this->chave, ch, 21);
    strncpy(this->conteudo, cont, 51);
}

Registro::Registro(Registro * r) {
    strncpy(this->chave, r->chave, 21);
    strncpy(this->conteudo, r->conteudo, 51);
}


Registro::~Registro()
{
}


std::istream & operator>> (std::istream & stream, std::shared_ptr<Registro> & registro) {
    std::string chave, valor, linha;
    stream >> linha;
    chave = linha.substr(0, linha.find(','));
	valor = linha.substr(linha.find(',') + 1, std::string::npos);
	std::shared_ptr<Registro> reg(new Registro(chave.c_str(), valor.c_str()));
    registro = reg;
    return stream;
}
