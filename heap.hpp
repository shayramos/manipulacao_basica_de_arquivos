#ifndef HEAP
#define HEAP

#include "registro.hpp"
#include<vector>
#include<memory>
#include<iterator>
#include<iostream>



/* Classe wrapper para marcação dos registros no heap. */
class ElementoHeap {
    public:
        std::shared_ptr<Registro> reg;
        bool marcado;
        ElementoHeap(std::shared_ptr<Registro>, bool); 

        /* Operação de <=. Verdade se o elemento não está marcado, enquanto o outro está, ou se ambos têm a mesma marcação e o valor do registro é menor ou igual. */
        bool operator <= (ElementoHeap & other) {
            return (!this->marcado && other.marcado) ||  ((this->marcado == other.marcado) && (*(this->reg) <= *(other.reg)));
        }
        bool operator >= (ElementoHeap & other) {
            return (this->marcado && !other.marcado) ||  ((this->marcado == other.marcado) && (*(this->reg) >= *(other.reg)));
        }
        bool operator < (ElementoHeap & other) {
            return (!this->marcado && other.marcado) ||  ((this->marcado == other.marcado) && (*(this->reg) < *(other.reg)));
        }
        bool operator > (ElementoHeap & other) {
            return (this->marcado && !other.marcado) ||  ((this->marcado == other.marcado) && (*(this->reg) > *(other.reg)));
        }
        bool operator == (ElementoHeap & other) {
            return ((this->marcado == other.marcado) && (*(this->reg) == *(other.reg)));
        }
        bool operator != (ElementoHeap & other) {
            return ((this->marcado != other.marcado) || (*(this->reg) != *(other.reg)));
        }

};



class Heap
{
private:
    std::shared_ptr<std::vector<std::shared_ptr<ElementoHeap>>> heap;
    int elems;
    
public:
    /* Inicializa heap vazio. */
    Heap();
    /* Gera heap a partir do vector desorganizado */
    void make_heap();
    /* Insere elemento no heap. */
    bool heap_push(std::shared_ptr<ElementoHeap> r);
    bool heap_push(std::shared_ptr<Registro> r);
    std::shared_ptr<ElementoHeap> heap_peek_head();
    std::shared_ptr<Registro> heap_pop();
    bool isFull();
    const static int MAX_ELEMS = (536870912/sizeof(Registro)); // 512MiB dividido pelo tamanho do registro
    void unmark_elems();

    friend std::ostream& operator <<(std::ostream & os, const Heap & heap) { //overload do operador << para ostream, motivo de debug
        os << heap.elems << "\n";
        int a = 0;
        for (auto elem = heap.heap->cbegin() ; elem != heap.heap->cend(); ++elem) {
            os << a++ << "\t";
            os << *((*elem)->reg) << " " << (*elem)->marcado << std::endl;
        }
        os << "/-----/-----/" << "\n"; //se eu tirar essa linha dá segfault (WTF)
    }
};

#endif