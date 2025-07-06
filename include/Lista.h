#ifndef LISTA_H
#define LISTA_H

template <typename T>
struct NoLista {
    T dado;
    NoLista* proximo;
    NoLista* anterior;

    NoLista(T d) : dado(d), proximo(nullptr), anterior(nullptr) {}
};

template <typename T>
class Lista {
private:
    NoLista<T>* cabeca;
    NoLista<T>* cauda;
    int tamanho;

public:
    Lista();
    ~Lista();

    void insere(T dado);
    NoLista<T>* getCabeca() const;
    NoLista<T>* getCauda() const;
    int getTamanho() const;
    bool vazia() const;
};

#include "../src/Lista.tpp"

#endif // LISTA_H