#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H

#include <algorithm>

template<typename Chave, typename Valor>
struct NoAVL {
    Chave chave;
    Valor valor;
    NoAVL* esquerda;
    NoAVL* direita;
    int altura;
    
    NoAVL(Chave c, Valor v) : chave(c), valor(v), esquerda(nullptr), direita(nullptr), altura(1) {}
};

template<typename Chave, typename Valor>
class ArvoreAVL {
private:
    NoAVL<Chave, Valor>* raiz;
    
    int getAltura(NoAVL<Chave, Valor>* no);
    int getFatorBalanceamento(NoAVL<Chave, Valor>* no);
    NoAVL<Chave, Valor>* rotacaoEsquerda(NoAVL<Chave, Valor>* y);
    NoAVL<Chave, Valor>* rotacaoDireita(NoAVL<Chave, Valor>* x);
    NoAVL<Chave, Valor>* insereRecursivo(NoAVL<Chave, Valor>* no, Chave chave, Valor valor);
    Valor buscaRecursiva(NoAVL<Chave, Valor>* no, Chave chave);
    void destruirRecursivo(NoAVL<Chave, Valor>* no);
    void limparValoresRecursivo(NoAVL<Chave, Valor>* no, void (*deleter)(Valor));

public:
    ArvoreAVL();
    ~ArvoreAVL();
    void insere(Chave chave, Valor valor);
    Valor busca(Chave chave);
    void limparValores(void (*deleter)(Valor));
};

// Implementação
template<typename Chave, typename Valor>
ArvoreAVL<Chave, Valor>::ArvoreAVL() : raiz(nullptr) {}

template<typename Chave, typename Valor>
ArvoreAVL<Chave, Valor>::~ArvoreAVL() {
    destruirRecursivo(raiz);
}

template<typename Chave, typename Valor>
int ArvoreAVL<Chave, Valor>::getAltura(NoAVL<Chave, Valor>* no) {
    return no ? no->altura : 0;
}

template<typename Chave, typename Valor>
int ArvoreAVL<Chave, Valor>::getFatorBalanceamento(NoAVL<Chave, Valor>* no) {
    return no ? getAltura(no->esquerda) - getAltura(no->direita) : 0;
}

template<typename Chave, typename Valor>
NoAVL<Chave, Valor>* ArvoreAVL<Chave, Valor>::rotacaoEsquerda(NoAVL<Chave, Valor>* y) {
    NoAVL<Chave, Valor>* x = y->direita;
    NoAVL<Chave, Valor>* T2 = x->esquerda;
    
    x->esquerda = y;
    y->direita = T2;
    
    y->altura = std::max(getAltura(y->esquerda), getAltura(y->direita)) + 1;
    x->altura = std::max(getAltura(x->esquerda), getAltura(x->direita)) + 1;
    
    return x;
}

template<typename Chave, typename Valor>
NoAVL<Chave, Valor>* ArvoreAVL<Chave, Valor>::rotacaoDireita(NoAVL<Chave, Valor>* x) {
    NoAVL<Chave, Valor>* y = x->esquerda;
    NoAVL<Chave, Valor>* T2 = y->direita;
    
    y->direita = x;
    x->esquerda = T2;
    
    x->altura = std::max(getAltura(x->esquerda), getAltura(x->direita)) + 1;
    y->altura = std::max(getAltura(y->esquerda), getAltura(y->direita)) + 1;
    
    return y;
}

template<typename Chave, typename Valor>
NoAVL<Chave, Valor>* ArvoreAVL<Chave, Valor>::insereRecursivo(NoAVL<Chave, Valor>* no, Chave chave, Valor valor) {
    if (no == nullptr) {
        return new NoAVL<Chave, Valor>(chave, valor);
    }
    
    if (chave < no->chave) {
        no->esquerda = insereRecursivo(no->esquerda, chave, valor);
    } else if (chave > no->chave) {
        no->direita = insereRecursivo(no->direita, chave, valor);
    } else {
        return no;
    }
    
    no->altura = 1 + std::max(getAltura(no->esquerda), getAltura(no->direita));
    
    int balanceamento = getFatorBalanceamento(no);
    
    if (balanceamento > 1 && chave < no->esquerda->chave) {
        return rotacaoDireita(no);
    }
    
    if (balanceamento < -1 && chave > no->direita->chave) {
        return rotacaoEsquerda(no);
    }
    
    if (balanceamento > 1 && chave > no->esquerda->chave) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }
    
    if (balanceamento < -1 && chave < no->direita->chave) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }
    
    return no;
}

template<typename Chave, typename Valor>
void ArvoreAVL<Chave, Valor>::insere(Chave chave, Valor valor) {
    raiz = insereRecursivo(raiz, chave, valor);
}

template<typename Chave, typename Valor>
Valor ArvoreAVL<Chave, Valor>::busca(Chave chave) {
    return buscaRecursiva(raiz, chave);
}

template<typename Chave, typename Valor>
Valor ArvoreAVL<Chave, Valor>::buscaRecursiva(NoAVL<Chave, Valor>* no, Chave chave) {
    if (no == nullptr) return nullptr;
    if (chave < no->chave) return buscaRecursiva(no->esquerda, chave);
    else if (chave > no->chave) return buscaRecursiva(no->direita, chave);
    else return no->valor;
}

template<typename Chave, typename Valor>
void ArvoreAVL<Chave, Valor>::destruirRecursivo(NoAVL<Chave, Valor>* no) {
    if (no != nullptr) {
        destruirRecursivo(no->esquerda);
        destruirRecursivo(no->direita);
        delete no;
    }
}

template<typename Chave, typename Valor>
void ArvoreAVL<Chave, Valor>::limparValores(void (*deleter)(Valor)) {
    limparValoresRecursivo(raiz, deleter);
}

template<typename Chave, typename Valor>
void ArvoreAVL<Chave, Valor>::limparValoresRecursivo(NoAVL<Chave, Valor>* no, void (*deleter)(Valor)) {
    if (no != nullptr) {
        limparValoresRecursivo(no->esquerda, deleter);
        limparValoresRecursivo(no->direita, deleter);
        deleter(no->valor);
    }
}

#endif // ARVORE_AVL_H
