#ifndef SISTEMA_H
#define SISTEMA_H

#include "Lista.h"
#include "Arvore.h"
#include "Evento.h"
#include <iostream>

struct PacoteInfo {
    Lista<Evento*>* historico;
};

struct ClienteInfo {
    Lista<int>* pacotes;
};

class Sistema {
private:
    Lista<Evento*> linhaDoTempo;
    Arvore<int, PacoteInfo*> indicePacotes;
    Arvore<std::string, ClienteInfo*> indiceClientes;

    // Funções de processamento modularizadas
    void processarLinha(const std::string& linha);
    void processarEvento(std::stringstream& ss, const Data& data, int idPacote, const std::string& tipoEvento);
    void processarConsulta(const std::string& linha, const std::string& tipoCmd, std::stringstream& ss);

    // Helpers de parsing de eventos
    void processarEventoRG(std::stringstream& ss, const Data& data, int idPacote);
    void processarEventoComLocal(std::stringstream& ss, const Data& data, int idPacote, const std::string& tipoEvento);
    void processarEventoTR(std::stringstream& ss, const Data& data, int idPacote);
    
    // Helpers de consulta
    void processarConsultaCL(const std::string& linhaConsulta, const std::string& nomeCliente);
    void processarConsultaPC(const std::string& linhaConsulta, int idPacote);

public:
    Sistema();
    ~Sistema();
    void executar();
};

#endif // SISTEMA_H