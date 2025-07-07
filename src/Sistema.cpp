#include "Sistema.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// ===================================================================
// FUNÇÕES AUXILIARES
// ===================================================================

// Ordena um array de ponteiros de Evento com base na data/hora
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

// Verifica se um pacote já existe na lista de um cliente
bool pacoteNaLista(Lista<int>* lista, int idPacote) {
    NoLista<int>* atual = lista->getCabeca();
    while (atual != nullptr) {
        if (atual->dado == idPacote) return true;
        atual = atual->proximo;
    }
    return false;
}

// Funções "deleter" para a limpeza de memória das árvores
void deleteClienteInfo(ClienteInfo* ci) {
    delete ci->pacotes;
    delete ci;
}

void deletePacoteInfo(PacoteInfo* pi) {
    // A memória dos Eventos é gerida pela linhaDoTempo.
    // Aqui, apenas apagamos a lista de ponteiros do pacote.
    delete pi->historico;
    delete pi;
}

// ===================================================================
// MÉTODOS DA CLASSE SISTEMA
// ===================================================================

Sistema::Sistema() {}

Sistema::~Sistema() {
    indiceClientes.limparValores(deleteClienteInfo);
    indicePacotes.limparValores(deletePacoteInfo);

    // Apaga centralmente todos os objetos Evento criados
    NoLista<Evento*>* noEvento = linhaDoTempo.getCabeca();
    while (noEvento != nullptr) {
        delete noEvento->dado;
        noEvento = noEvento->proximo;
    }
}

void Sistema::executar(std::istream& entrada) {
    std::string linha;
    while (std::getline(entrada, linha)) {
        if (linha.empty()) continue;
        processarLinha(linha);
    }
}

void Sistema::processarLinha(const std::string& linha) {
    std::stringstream ss(linha);
    std::string dataStr, tipoCmd;
    if (!(ss >> dataStr >> tipoCmd)) return;

    if (tipoCmd == "EV") {
        Data data = Data::fromString(dataStr);
        int idPacote;
        std::string tipoEvento;
        if (!(ss >> tipoEvento >> idPacote)) return;
        
        if (tipoEvento == "RG") processarEventoRG(ss, data, idPacote);
        else if (tipoEvento == "TR") processarEventoTR(ss, data, idPacote);
        else if (tipoEvento == "AR" || tipoEvento == "RM" || tipoEvento == "UR" || tipoEvento == "EN")
            processarEventoComLocal(ss, data, idPacote, tipoEvento);
    } else if (tipoCmd == "CL" || tipoCmd == "PC") {
        processarConsulta(linha, tipoCmd, ss);
    }
}

void Sistema::processarEventoRG(std::stringstream& ss, const Data& data, int idPacote) {
    std::string remetente, destinatario, armazemOrigem, armazemDestino;
    if (!(ss >> remetente >> destinatario >> armazemOrigem >> armazemDestino)) return;
    
    Evento* novoEvento = new EventoRG(data, idPacote, remetente, destinatario, armazemOrigem, armazemDestino);
    linhaDoTempo.insere(novoEvento); // Garante que a memória será libertada

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
    if (!pacoteNaLista(cInfoRem->pacotes, idPacote)) cInfoRem->pacotes->insere(idPacote);

    ClienteInfo* cInfoDest = indiceClientes.busca(destinatario);
    if (!cInfoDest) {
        cInfoDest = new ClienteInfo{new Lista<int>()};
        indiceClientes.insere(destinatario, cInfoDest);
    }
    if (!pacoteNaLista(cInfoDest->pacotes, idPacote)) cInfoDest->pacotes->insere(idPacote);
}

void Sistema::processarEventoTR(std::stringstream& ss, const Data& data, int idPacote) {
    std::string localOrigem, localDestino;
    if (!(ss >> localOrigem >> localDestino)) return;

    Evento* novoEvento = new EventoTR(data, idPacote, localOrigem, localDestino);
    linhaDoTempo.insere(novoEvento); // Garante que a memória será libertada
    
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
        linhaDoTempo.insere(novoEvento); // Garante que a memória será libertada
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

// Esta é a função que implementa a lógica do exemplo (histórico completo)
void Sistema::processarConsultaCL(const std::string& linhaConsulta, const std::string& nomeCliente) {
    // Formata a linha da consulta com 6 dígitos, como estava correto
    std::string dataStr = linhaConsulta.substr(0, 7);
    std::string restoLinha = linhaConsulta.substr(7);
    int timestamp = std::stoi(dataStr);
    std::cout << std::setfill('0') << std::setw(6) << timestamp << restoLinha << std::endl;

    ClienteInfo* cInfo = indiceClientes.busca(nomeCliente);
    if (!cInfo || cInfo->pacotes->vazia()) {
        std::cout << "0" << std::endl;
        return;
    }

    Data dataConsulta = Data::fromString(dataStr);
    Lista<Evento*> eventosFinais;

    // Itera sobre cada pacote associado ao cliente
    NoLista<int>* noPacote = cInfo->pacotes->getCabeca();
    while (noPacote != nullptr) {
        int idPacote = noPacote->dado;
        PacoteInfo* pInfo = indicePacotes.busca(idPacote);
        
        if (pInfo && !pInfo->historico->vazia()) {
            Evento* eventoRegistro = nullptr;
            Evento* ultimoEvento = nullptr;

            // Encontra o evento RG e o último evento válido
            NoLista<Evento*>* noEvento = pInfo->historico->getCabeca();
            while (noEvento != nullptr) {
                // Considera apenas eventos até o momento da consulta
                if (!(dataConsulta < noEvento->dado->dataHora)) {
                    if (noEvento->dado->tipo == "RG") {
                        eventoRegistro = noEvento->dado;
                    }
                    ultimoEvento = noEvento->dado; // O último evento válido será o último encontrado
                }
                noEvento = noEvento->proximo;
            }

            // Adiciona o evento de registro se ele existir
            if (eventoRegistro) {
                eventosFinais.insere(eventoRegistro);
            }
            // Adiciona o último evento se ele for diferente do de registro
            if (ultimoEvento && ultimoEvento != eventoRegistro) {
                eventosFinais.insere(ultimoEvento);
            }
        }
        noPacote = noPacote->proximo;
    }
    
    // Converte a lista para um array para poder ordenar
    int totalEventos = eventosFinais.getTamanho();
    std::cout << totalEventos << std::endl;

    if (totalEventos > 0) {
        Evento** eventosArray = new Evento*[totalEventos];
        NoLista<Evento*>* noAtual = eventosFinais.getCabeca();
        int i = 0;
        while(noAtual != nullptr) {
            eventosArray[i++] = noAtual->dado;
            noAtual = noAtual->proximo;
        }

        // Ordena o resultado final e imprime
        insertionSort(eventosArray, totalEventos);
        for (int j = 0; j < totalEventos; ++j) {
            std::cout << eventosArray[j]->toString() << std::endl;
        }
        
        delete[] eventosArray;
    }
}

void Sistema::processarConsultaPC(const std::string& linhaConsulta, int idPacote) {
    // Extrai o timestamp e o resto da linha
    std::string dataStr = linhaConsulta.substr(0, 7);
    std::string restoLinha = linhaConsulta.substr(7);

    // Converte e re-formata o timestamp para 6 dígitos
    int timestamp = std::stoi(dataStr);
    std::cout << std::setfill('0') << std::setw(6) << timestamp << restoLinha << std::endl;
    
    PacoteInfo* pInfo = indicePacotes.busca(idPacote);
    Data dataConsulta = Data::fromString(dataStr);
    
    if (!pInfo || pInfo->historico->vazia()) {
        std::cout << "0" << std::endl;
        return;
    }

    Lista<Evento*> eventosDoPacote;
    NoLista<Evento*>* noEvento = pInfo->historico->getCabeca();
    while (noEvento != nullptr) {
        // Filtra eventos que ocorreram após a consulta
        if (!(dataConsulta < noEvento->dado->dataHora)) {
            eventosDoPacote.insere(noEvento->dado);
        }
        noEvento = noEvento->proximo;
    }

    int totalEventos = eventosDoPacote.getTamanho();
    std::cout << totalEventos << std::endl;

    if (totalEventos > 0) {
        // Converte para array para ordenar
        Evento** eventosArray = new Evento*[totalEventos];
        NoLista<Evento*>* noAtual = eventosDoPacote.getCabeca();
        int i = 0;
        while(noAtual != nullptr) {
            eventosArray[i++] = noAtual->dado;
            noAtual = noAtual->proximo;
        }

        insertionSort(eventosArray, totalEventos);

        for (int j = 0; j < totalEventos; ++j) {
            std::cout << eventosArray[j]->toString() << std::endl;
        }

        delete[] eventosArray;
    }
}