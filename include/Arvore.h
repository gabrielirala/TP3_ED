#ifndef ARVORE_H
#define ARVORE_H

#include <string>

template <typename Chave, typename Valor>
struct NoArvore {
    Chave chave;
    Valor valor;
    NoArvore *esquerda;
    NoArvore *direita;

    NoArvore(Chave c, Valor v) : chave(c), valor(v), esquerda(nullptr), direita(nullptr) {}
};

template <typename Chave, typename Valor>
class Arvore {
private:
    NoArvore<Chave, Valor>* raiz;

    NoArvore<Chave, Valor>* insereRecursivo(NoArvore<Chave, Valor>* no, Chave chave, Valor valor);
    Valor buscaRecursiva(NoArvore<Chave, Valor>* no, Chave chave);
    void destruirRecursivo(NoArvore<Chave, Valor>* no);
    void limparValoresRecursivo(NoArvore<Chave, Valor>* no, void (*deleter)(Valor));

public:
    Arvore();
    ~Arvore();
    void insere(Chave chave, Valor valor);
    Valor busca(Chave chave);
    void limparValores(void (*deleter)(Valor));
};

// --- Implementação do Template ---

template <typename Chave, typename Valor>
Arvore<Chave, Valor>::Arvore() : raiz(nullptr) {}

template <typename Chave, typename Valor>
Arvore<Chave, Valor>::~Arvore() {
    destruirRecursivo(raiz);
}

template <typename Chave, typename Valor>
void Arvore<Chave, Valor>::destruirRecursivo(NoArvore<Chave, Valor>* no) {
    if (no != nullptr) {
        destruirRecursivo(no->esquerda);
        destruirRecursivo(no->direita);
        delete no;
    }
}

template <typename Chave, typename Valor>
void Arvore<Chave, Valor>::limparValores(void (*deleter)(Valor)) {
    limparValoresRecursivo(raiz, deleter);
}

template <typename Chave, typename Valor>
void Arvore<Chave, Valor>::limparValoresRecursivo(NoArvore<Chave, Valor>* no, void (*deleter)(Valor)) {
    if (no != nullptr) {
        limparValoresRecursivo(no->esquerda, deleter);
        limparValoresRecursivo(no->direita, deleter);
        deleter(no->valor);
    }
}

template <typename Chave, typename Valor>
void Arvore<Chave, Valor>::insere(Chave chave, Valor valor) {
    raiz = insereRecursivo(raiz, chave, valor);
}

template <typename Chave, typename Valor>
NoArvore<Chave, Valor>* Arvore<Chave, Valor>::insereRecursivo(NoArvore<Chave, Valor>* no, Chave chave, Valor valor) {
    if (no == nullptr) {
        return new NoArvore<Chave, Valor>(chave, valor);
    }
    if (chave < no->chave) {
        no->esquerda = insereRecursivo(no->esquerda, chave, valor);
    } else if (chave > no->chave) {
        no->direita = insereRecursivo(no->direita, chave, valor);
    }
    return no;
}

template <typename Chave, typename Valor>
Valor Arvore<Chave, Valor>::busca(Chave chave) {
    return buscaRecursiva(raiz, chave);
}

template <typename Chave, typename Valor>
Valor Arvore<Chave, Valor>::buscaRecursiva(NoArvore<Chave, Valor>* no, Chave chave) {
    if (no == nullptr) return nullptr;
    if (chave < no->chave) return buscaRecursiva(no->esquerda, chave);
    else if (chave > no->chave) return buscaRecursiva(no->direita, chave);
    else return no->valor;
}

#endif // ARVORE_H