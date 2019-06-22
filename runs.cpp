#include "runs.hpp"


/* A partir de um arquivo de entrada, gera uma run e escreve num arquivo destino. */
void escreveRunEmArquivo(std::unique_ptr<Heap> & heap, std::fstream & fonte, std::fstream & destino) {
    std::shared_ptr<Registro> atual = getNextElement(heap, fonte);
    while (atual != nullptr) { //se for nulo, acabou a run.
        destino << atual;
        atual = getNextElement(heap, fonte);
    }
}


/* Obtem o proximo elemento da run. Retorna nulo se a run acabou. */
std::shared_ptr<Registro> getNextElement(std::unique_ptr<Heap> & heap, std::fstream & fonte) { 
    std::shared_ptr<Registro> reg = nullptr, reg2 = nullptr;
    std::shared_ptr<ElementoHeap> elem = nullptr;
    elem = heap->heap_peek_head(); //checa o elemento da raiz
    if (elem->marcado) { //se a raiz está marcada, acabou a run.
        heap->unmark_elems(); 
        return nullptr; //sinal para acabar a run
    }
    fonte >> reg;
    reg2 = heap->heap_pop(); //retira a raiz do heap

    std::shared_ptr<ElementoHeap> elem2(new ElementoHeap(reg, *reg < *reg2)); //novo elemento, será marcado se o valor dele for menor que o anterior
    heap->heap_push(elem2); //insere o novo elemento

    return reg2;
}