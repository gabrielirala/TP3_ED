#ifndef LISTA_TPP
#define LISTA_TPP

#include "../include/Lista.h"

template <typename T>
Lista<T>::Lista() : cabeca(nullptr), cauda(nullptr), tamanho(0) {}

template <typename T>
Lista<T>::~Lista() {
    NoLista<T>* atual = cabeca;
    while (atual != nullptr) {
        NoLista<T>* proximo = atual->proximo;
        delete atual;
        atual = proximo;
    }
}

template <typename T>
void Lista<T>::insere(T dado) {
    NoLista<T>* novoNo = new NoLista<T>(dado);
    if (vazia()) {
        cabeca = novoNo;
        cauda = novoNo;
    } else {
        cauda->proximo = novoNo;
        novoNo->anterior = cauda;
        cauda = novoNo;
    }
    tamanho++;
}

template <typename T>
NoLista<T>* Lista<T>::getCabeca() const {
    return cabeca;
}

template <typename T>
NoLista<T>* Lista<T>::getCauda() const {
    return cauda;
}

template <typename T>
int Lista<T>::getTamanho() const {
    return tamanho;
}

template <typename T>
bool Lista<T>::vazia() const {
    return tamanho == 0;
}

#endif // LISTA_TPP