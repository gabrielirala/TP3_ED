#include "Sistema.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// --- Funções Auxiliares ---
void insertionSort(Evento** arr, int n) {
    int i, j;
    Evento* chave;
    for (i = 1; i < n; i++) {
        chave = arr[i];
        j = i - 1;
        // CORREÇÃO: Usa o operador< sobrecarregado da classe Data.
        while (j >= 0 && chave->dataHora < arr[j]->dataHora) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = chave;
    }
}

bool pacoteNaLista(Lista<int>* lista, int idPacote) {
    NoLista<int>* atual = lista->getCabeca();
    while (atual != nullptr) {
        if (atual->dado == idPacote) return true;
        atual = atual->proximo;
    }
    return false;
}

// Funções "deleter" para limpeza de memória na árvore
void deleteClienteInfo(ClienteInfo* ci) {
    delete ci->pacotes;
    delete ci;
}

void deletePacoteInfo(PacoteInfo* pi) {
    delete pi->historico;
    delete pi;
}


// --- Métodos da Classe Sistema ---
Sistema::Sistema() {}

Sistema::~Sistema() {
    // Libera a memória dos valores armazenados nos índices
    indiceClientes.limparValores(deleteClienteInfo);
    indicePacotes.limparValores(deletePacoteInfo);

    // Libera a memória dos próprios eventos
    NoLista<Evento*>* noEvento = linhaDoTempo.getCabeca();
    while (noEvento != nullptr) {
        delete noEvento->dado;
        noEvento = noEvento->proximo;
    }
}

void Sistema::executar() {
    std::string linha;
    while (std::getline(std::cin, linha)) {
        if (linha.empty()) continue;
        processarLinha(linha);
    }
}

// --- Funções de Processamento e Parsing (Modularizadas) ---
void Sistema::processarLinha(const std::string& linha) {
    std::stringstream ss(linha);
    std::string dataStr, tipoCmd;
    if (!(ss >> dataStr >> tipoCmd)) return;

    if (tipoCmd == "EV") {
        Data data = Data::fromString(dataStr);
        int idPacote;
        std::string tipoEvento;
        if (!(ss >> idPacote >> tipoEvento)) return;
        processarEvento(ss, data, idPacote, tipoEvento);
    } else if (tipoCmd == "CL" || tipoCmd == "PC") {
        processarConsulta(linha, tipoCmd, ss);
    }
}

void Sistema::processarEvento(std::stringstream& ss, const Data& data, int idPacote, const std::string& tipoEvento) {
    if (tipoEvento == "RG") {
        processarEventoRG(ss, data, idPacote);
    } else if (tipoEvento == "TR") {
        processarEventoTR(ss, data, idPacote);
    } else if (tipoEvento == "AR" || tipoEvento == "RM" || tipoEvento == "UR" || tipoEvento == "EN") {
        processarEventoComLocal(ss, data, idPacote, tipoEvento);
    }
}

void Sistema::processarEventoRG(std::stringstream& ss, const Data& data, int idPacote) {
    std::string remetente, destinatario;
    if (!(ss >> remetente >> destinatario)) return;
    
    Evento* novoEvento = new EventoRG(data, idPacote, remetente, destinatario);
    linhaDoTempo.insere(novoEvento);

    // Atualiza índice de pacotes
    PacoteInfo* pInfo = indicePacotes.busca(idPacote);
    if (!pInfo) {
        pInfo = new PacoteInfo{new Lista<Evento*>()};
        indicePacotes.insere(idPacote, pInfo);
    }
    pInfo->historico->insere(novoEvento);

    // Atualiza índice de clientes
    ClienteInfo* cInfoRem = indiceClientes.busca(remetente);
    if (!cInfoRem) {
        cInfoRem = new ClienteInfo{new Lista<int>()};
        indiceClientes.insere(remetente, cInfoRem);
    }
    if (!pacoteNaLista(cInfoRem->pacotes, idPacote)) {
        cInfoRem->pacotes->insere(idPacote);
    }

    ClienteInfo* cInfoDest = indiceClientes.busca(destinatario);
    if (!cInfoDest) {
        cInfoDest = new ClienteInfo{new Lista<int>()};
        indiceClientes.insere(destinatario, cInfoDest);
    }
    if (!pacoteNaLista(cInfoDest->pacotes, idPacote)) {
        cInfoDest->pacotes->insere(idPacote);
    }
}

void Sistema::processarEventoTR(std::stringstream& ss, const Data& data, int idPacote) {
    std::string p1, p2;
    if (!(ss >> p1 >> p2)) return;

    Evento* novoEvento = new EventoTR(data, idPacote, p1, p2);
    linhaDoTempo.insere(novoEvento);

    // CORREÇÃO: Garante que o pacote seja criado no índice se for novo.
    PacoteInfo* pInfo = indicePacotes.busca(idPacote);
    if (!pInfo) {
        pInfo = new PacoteInfo{new Lista<Evento*>()};
        indicePacotes.insere(idPacote, pInfo);
    }
    pInfo->historico->insere(novoEvento);
}

void Sistema::processarEventoComLocal(std::stringstream& ss, const Data& data, int idPacote, const std::string& tipoEvento) {
    std::string local;
    ss >> std::ws; 
    std::getline(ss, local);
    if (local.empty()) return;

    Evento* novoEvento = nullptr;
    if (tipoEvento == "AR") novoEvento = new EventoAR(data, idPacote, local);
    else if (tipoEvento == "RM") novoEvento = new EventoRM(data, idPacote, local);
    else if (tipoEvento == "UR") novoEvento = new EventoUR(data, idPacote, local);
    else if (tipoEvento == "EN") novoEvento = new EventoEN(data, idPacote, local);

    if (novoEvento) {
        linhaDoTempo.insere(novoEvento);
        // CORREÇÃO: Garante que o pacote seja criado no índice se for novo.
        PacoteInfo* pInfo = indicePacotes.busca(idPacote);
        if (!pInfo) {
            pInfo = new PacoteInfo{new Lista<Evento*>()};
            indicePacotes.insere(idPacote, pInfo);
        }
        pInfo->historico->insere(novoEvento);
    }
}

void Sistema::processarConsulta(const std::string& linha, const std::string& tipoCmd, std::stringstream& ss) {
    if (tipoCmd == "CL") {
        std::string nomeCliente;
        if (!(ss >> nomeCliente)) return;
        processarConsultaCL(linha, nomeCliente);
    } else if (tipoCmd == "PC") {
        int idPacote;
        if (!(ss >> idPacote)) return;
        processarConsultaPC(linha, idPacote);
    }
}

void Sistema::processarConsultaCL(const std::string& linhaConsulta, const std::string& nomeCliente) {
    // CORREÇÃO: Formata a saída da linha de consulta para remover o zero inicial.
    std::stringstream ss(linhaConsulta);
    std::string dataStr, cmd, arg;
    ss >> dataStr >> cmd >> arg;
    std::cout << dataStr.substr(1) << " " << cmd << " " << arg << std::endl;

    ClienteInfo* cInfo = indiceClientes.busca(nomeCliente);
    
    if (!cInfo || cInfo->pacotes->vazia()) {
        std::cout << "0" << std::endl;
        return;
    }

    int count = 0;
    NoLista<int>* tempNo = cInfo->pacotes->getCabeca();
    while (tempNo != nullptr) {
        PacoteInfo* pInfo = indicePacotes.busca(tempNo->dado);
        if (pInfo && !pInfo->historico->vazia()) {
            count++;
            if (pInfo->historico->getTamanho() > 1) count++;
        }
        tempNo = tempNo->proximo;
    }

    if (count == 0) {
        std::cout << "0" << std::endl;
        return;
    }

    Evento** eventosResposta = new Evento*[count];
    int idx = 0;
    NoLista<int>* noPacoteId = cInfo->pacotes->getCabeca();
    while (noPacoteId != nullptr) {
        PacoteInfo* pInfo = indicePacotes.busca(noPacoteId->dado);
        if (pInfo && !pInfo->historico->vazia()) {
            eventosResposta[idx++] = pInfo->historico->getCabeca()->dado;
            if (pInfo->historico->getTamanho() > 1) {
                eventosResposta[idx++] = pInfo->historico->getCauda()->dado;
            }
        }
        noPacoteId = noPacoteId->proximo;
    }

    insertionSort(eventosResposta, count);

    std::cout << count << std::endl;
    for (int i = 0; i < count; ++i) {
        std::cout << eventosResposta[i]->toString() << std::endl;
    }

    delete[] eventosResposta;
}

void Sistema::processarConsultaPC(const std::string& linhaConsulta, int idPacote) {
    // CORREÇÃO: Formata a saída da linha de consulta para remover o zero inicial.
    std::stringstream ss(linhaConsulta);
    std::string dataStr, cmd, arg;
    ss >> dataStr >> cmd >> arg;
    std::cout << dataStr.substr(1) << " " << cmd << " " << arg << std::endl;
    
    PacoteInfo* pInfo = indicePacotes.busca(idPacote);

    if (!pInfo) {
        std::cout << "0" << std::endl;
        return;
    }
    
    std::cout << pInfo->historico->getTamanho() << std::endl;
    NoLista<Evento*>* noEvento = pInfo->historico->getCabeca();
    while (noEvento != nullptr) {
        std::cout << noEvento->dado->toString() << std::endl;
        noEvento = noEvento->proximo;
    }
}