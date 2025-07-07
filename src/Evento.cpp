#include "Evento.h"
#include <sstream>
#include <iomanip>

Evento::Evento(Data dh, int id, std::string t) : dataHora(dh), idPacote(id), tipo(t) {}
Evento::~Evento() {}

// --- CONSTRUTORES ---

// Construtor de EventoRG corrigido que você já tinha
EventoRG::EventoRG(Data dh, int id, std::string rem, std::string dest, std::string origem, std::string destino)
    : Evento(dh, id, "RG"), remetente(rem), destinatario(dest), armazemOrigem(origem), armazemDestino(destino) {}

// CONSTRUTORES QUE ESTAVAM FALTANDO
EventoAR::EventoAR(Data dh, int id, std::string l)
    : Evento(dh, id, "AR"), local(l) {}

EventoRM::EventoRM(Data dh, int id, std::string l)
    : Evento(dh, id, "RM"), local(l) {}

EventoUR::EventoUR(Data dh, int id, std::string l)
    : Evento(dh, id, "UR"), local(l) {}

EventoTR::EventoTR(Data dh, int id, std::string orig, std::string dest)
    : Evento(dh, id, "TR"), localOrigem(orig), localDestino(dest) {}

EventoEN::EventoEN(Data dh, int id, std::string l)
    : Evento(dh, id, "EN"), local(l) {}


// --- MÉTODOS toString() CORRIGIDOS ---

std::string EventoRG::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " EV " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << remetente << " " << destinatario << " " << armazemOrigem << " " << armazemDestino;
    return ss.str();
}

std::string EventoAR::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " EV " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << local;
    return ss.str();
}

std::string EventoRM::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " EV " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << local;
    return ss.str();
}

std::string EventoUR::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " EV " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << local;
    return ss.str();
}

std::string EventoTR::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " EV " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << localOrigem << " " << localDestino;
    return ss.str();
}

std::string EventoEN::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " EV " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << local;
    return ss.str();
}