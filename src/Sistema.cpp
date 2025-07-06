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
    indiceClientes.limparValores(deleteClienteInfo);
    indicePacotes.limparValores(deletePacoteInfo);

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

// --- Funções de Processamento e Parsing (Modularizadas e Corrigidas) ---
void Sistema::processarLinha(const std::string& linha) {
    std::stringstream ss(linha);
    std::string dataStr, tipoCmd;
    if (!(ss >> dataStr >> tipoCmd)) return;

    if (tipoCmd == "EV") {
        Data data = Data::fromString(dataStr);
        int idPacote;
        std::string tipoEvento;
        // Ordem de parsing correta: <id_pacote> <tipo_evento>
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
    
    // Ignora campos adicionais do input
    std::string campo1, campo2;
    ss >> campo1 >> campo2;

    Evento* novoEvento = new EventoRG(data, idPacote, remetente, destinatario);
    linhaDoTempo.insere(novoEvento);

    PacoteInfo* pInfo = indicePacotes.busca(idPacote);
    if (!pInfo) {
        pInfo = new PacoteInfo{new Lista<Evento*>()};
        indicePacotes.insere(idPacote, pInfo);
    }
    pInfo->historico->insere(novoEvento);

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
    std::string localOrigem, localDestino;
    if (!(ss >> localOrigem >> localDestino)) return;

    Evento* novoEvento = new EventoTR(data, idPacote, localOrigem, localDestino);
    linhaDoTempo.insere(novoEvento);
    
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

// CORREÇÃO: Consulta CL retorna todos os eventos de todos os pacotes do cliente, em ordem.
void Sistema::processarConsultaCL(const std::string& linhaConsulta, const std::string& nomeCliente) {
    std::string dataFormatada = linhaConsulta;
    // O formato de saída para a linha de consulta remove um zero inicial do timestamp
    if (dataFormatada.length() > 1 && dataFormatada[0] == '0') {
        dataFormatada = dataFormatada.substr(1);
    }
    std::cout << dataFormatada << std::endl;

    ClienteInfo* cInfo = indiceClientes.busca(nomeCliente);
    if (!cInfo || cInfo->pacotes->vazia()) {
        std::cout << "0" << std::endl;
        return;
    }

    // Passagem 1: Contar o número total de eventos para alocar o array
    int totalEventos = 0;
    NoLista<int>* noPacoteContagem = cInfo->pacotes->getCabeca();
    while (noPacoteContagem != nullptr) {
        PacoteInfo* pInfo = indicePacotes.busca(noPacoteContagem->dado);
        if (pInfo) {
            totalEventos += pInfo->historico->getTamanho();
        }
        noPacoteContagem = noPacoteContagem->proximo;
    }
    
    if (totalEventos == 0) {
        std::cout << "0" << std::endl;
        return;
    }
    
    // Alocar e preencher o array
    Evento** eventosCliente = new Evento*[totalEventos];
    int idx = 0;
    NoLista<int>* noPacote = cInfo->pacotes->getCabeca();
    while (noPacote != nullptr) {
        PacoteInfo* pInfo = indicePacotes.busca(noPacote->dado);
        if (pInfo && !pInfo->historico->vazia()) {
            NoLista<Evento*>* noEvento = pInfo->historico->getCabeca();
            while (noEvento != nullptr) {
                eventosCliente[idx++] = noEvento->dado;
                noEvento = noEvento->proximo;
            }
        }
        noPacote = noPacote->proximo;
    }
    
    // Ordenar a lista combinada e imprimir
    insertionSort(eventosCliente, totalEventos);
    
    std::cout << totalEventos << std::endl;
    for (int i = 0; i < totalEventos; ++i) {
        std::cout << eventosCliente[i]->toString() << std::endl;
    }
    
    delete[] eventosCliente;
}

void Sistema::processarConsultaPC(const std::string& linhaConsulta, int idPacote) {
    std::string dataFormatada = linhaConsulta;
    if (dataFormatada.length() > 1 && dataFormatada[0] == '0') {
        dataFormatada = dataFormatada.substr(1);
    }
    std::cout << dataFormatada << std::endl;

    PacoteInfo* pInfo = indicePacotes.busca(idPacote);
    if (!pInfo || pInfo->historico->vazia()) {
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