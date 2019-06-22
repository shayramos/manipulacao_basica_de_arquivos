#ifndef RUNS
#define RUNS

#include "heap.hpp"
#include <fstream>

void escreveRunEmArquivo(std::unique_ptr<Heap> &, std::fstream &, std::fstream &);

std::shared_ptr<Registro> getNextElement(std::unique_ptr<Heap> &, std::fstream &);


#endif