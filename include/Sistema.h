#ifndef SISTEMA_H
#define SISTEMA_H

#include "Lista.h"
#include "ArvoreAVL.h"
#include "Evento.h"
#include <string>

struct PacoteInfo {
    Lista<Evento*>* historico;
};

struct ClienteInfo {
    Lista<int>* pacotes;
};

struct EstadoPacote {
    std::string papel;
    int idPacote;
    std::string estadoAtual;
    Data ultimaAtualizacao;
    
    EstadoPacote(std::string p, int id, std::string estado, Data data) 
        : papel(p), idPacote(id), estadoAtual(estado), ultimaAtualizacao(data) {}
};

class Sistema {
private:
    Lista<Evento*> linhaDoTempo;
    ArvoreAVL<int, PacoteInfo*> indicePacotes;
    ArvoreAVL<std::string, ClienteInfo*> indiceClientes;

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
    
    // Novos métodos para consulta CL corrigida
    std::string determinarPapelCliente(const std::string& nomeCliente, int idPacote);
    std::string calcularEstadoAtual(int idPacote);
    Data obterUltimaAtualizacao(int idPacote);
    void ordenarEstadosPorData(Lista<EstadoPacote>& estados);

public:
    Sistema();
    ~Sistema();
    void executar(std::istream& entrada);
};

#endif // SISTEMA_H

