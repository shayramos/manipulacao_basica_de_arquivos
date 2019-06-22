#include "heap.hpp"
#include "registro.hpp"
#include "runs.hpp"
#include <fstream>
#include <iostream>


int main(int argc, char * argv[]) {
    std::unique_ptr<Heap> heap(new Heap());

    if (argc > 1) {
        std::fstream f(argv[1], std::fstream::in);
        while (!f.eof() && !heap->isFull()) {
            std::shared_ptr<Registro> reg(nullptr);
            f >> reg; //lendo registro do arquivo
            heap->heap_push(reg);
        }
        std::shared_ptr<Registro> antigaRaiz = heap->heap_pop();
        std::shared_ptr<Registro> novoElem(new Registro("zzz", "sdhgfg"));
        heap->heap_push(novoElem);
        std::cout << *antigaRaiz << std::endl;
        std::cout << heap->isFull() << std::endl;
    }

    return 0;
}