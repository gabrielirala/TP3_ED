// Lista.tpp
template<typename T>
Lista<T>::Lista() : cabeca(nullptr), cauda(nullptr), tamanho(0) {}

template<typename T>
Lista<T>::~Lista() {
    NoLista<T>* atual = cabeca;
    while (atual != nullptr) {
        NoLista<T>* proximo = atual->proximo;
        delete atual;
        atual = proximo;
    }
}

template<typename T>
void Lista<T>::insere(T dado) {
    NoLista<T>* novo = new NoLista<T>(dado);
    
    if (cabeca == nullptr) {
        cabeca = cauda = novo;
    } else {
        cauda->proximo = novo;
        novo->anterior = cauda;
        cauda = novo;
    }
    
    tamanho++;
}

template<typename T>
NoLista<T>* Lista<T>::getCabeca() const {
    return cabeca;
}

template<typename T>
NoLista<T>* Lista<T>::getCauda() const {
    return cauda;
}

template<typename T>
int Lista<T>::getTamanho() const {
    return tamanho;
}

template<typename T>
bool Lista<T>::vazia() const {
    return tamanho == 0;
}
