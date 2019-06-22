#include "heap.hpp"
#include<utility>
#include <iostream>
#include <algorithm>



bool compare(std::shared_ptr<ElementoHeap> & a, std::shared_ptr<ElementoHeap> & b) {
    return *a > *b;
}

ElementoHeap::ElementoHeap(std::shared_ptr<Registro> r, bool m) { 
        this->reg.swap(r);
        this->marcado = m;
}


Heap::Heap()
{   
    std::shared_ptr<std::vector<std::shared_ptr<ElementoHeap>>> ptr(new std::vector<std::shared_ptr<ElementoHeap>>());
    this->heap = ptr;
    this->elems = 0; 
}


void Heap::make_heap() {
    std::make_heap(this->heap->begin(), this->heap->end(), compare); //larguei de mão e usei stl
}

bool Heap::heap_push(std::shared_ptr<ElementoHeap> elemento) {
    if (this->isFull()) {
        return false;
    }
    else {
        this->heap->push_back(elemento);
        std::push_heap(this->heap->begin(), this->heap->end(), compare); //stl
        this->elems++;
        return true;
    }
}

bool Heap::heap_push(std::shared_ptr<Registro> registro) {
    std::shared_ptr<ElementoHeap> elemento(new ElementoHeap(registro, false));
    return this->heap_push(elemento);
}

bool Heap::isFull() {
    return this->elems == Heap::MAX_ELEMS;
}

/* Retorna ponteiro para raiz do heap. */
std::shared_ptr<ElementoHeap> Heap::heap_peek_head() {
    return this->heap->at(0);
}


/* Remove a raiz do heap e retorna o registro correspondente. */
std::shared_ptr<Registro> Heap::heap_pop() {
    std::pop_heap(this->heap->begin(), this->heap->end(), compare);
    std::shared_ptr<Registro> reg = this->heap->at(this->heap->size() - 1)->reg;
    this->heap->pop_back();
    this->elems--;
    return reg;
}

void Heap::unmark_elems() {
    if (this->heap->at(0)->marcado) {
        for (auto elem : *(this->heap)) {
            elem->marcado = false;
        }
    }
}