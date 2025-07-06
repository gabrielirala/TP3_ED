#include "Evento.h"
#include <sstream>
#include <iomanip>

Evento::Evento(Data dh, int id, std::string t) : dataHora(dh), idPacote(id), tipo(t) {}
Evento::~Evento() {}

EventoRG::EventoRG(Data dh, int id, std::string rem, std::string dest)
    : Evento(dh, id, "RG"), remetente(rem), destinatario(dest) {}
std::string EventoRG::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << remetente << " " << destinatario;
    return ss.str();
}

EventoAR::EventoAR(Data dh, int id, std::string l)
    : Evento(dh, id, "AR"), local(l) {}
std::string EventoAR::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << local;
    return ss.str();
}

EventoRM::EventoRM(Data dh, int id, std::string l)
    : Evento(dh, id, "RM"), local(l) {}
std::string EventoRM::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << local;
    return ss.str();
}

EventoUR::EventoUR(Data dh, int id, std::string l)
    : Evento(dh, id, "UR"), local(l) {}
std::string EventoUR::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << local;
    return ss.str();
}

EventoTR::EventoTR(Data dh, int id, std::string orig, std::string dest)
    : Evento(dh, id, "TR"), localOrigem(orig), localDestino(dest) {}
std::string EventoTR::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << localOrigem << " " << localDestino;
    return ss.str();
}

EventoEN::EventoEN(Data dh, int id, std::string l)
    : Evento(dh, id, "EN"), local(l) {}
std::string EventoEN::toString() const {
    std::stringstream ss;
    ss << dataHora.toString() << " " << tipo << " " << std::setfill('0') << std::setw(3) << idPacote << " " << local;
    return ss.str();
}