#ifndef EVENTO_H
#define EVENTO_H

#include <string>
#include "Data.h"

class Evento {
public:
    Data dataHora;
    int idPacote;
    std::string tipo;

    Evento(Data dh, int id, std::string t);
    virtual ~Evento();
    virtual std::string toString() const = 0;
};

class EventoRG : public Evento {
public:
    std::string remetente;
    std::string destinatario;
    // ADICIONE ESTES CAMPOS
    std::string armazemOrigem;
    std::string armazemDestino;

    // ATUALIZE O CONSTRUTOR
    EventoRG(Data dh, int id, std::string rem, std::string dest, std::string origem, std::string destino);
    std::string toString() const override;
};

class EventoAR : public Evento {
public:
    std::string local;
    EventoAR(Data dh, int id, std::string l);
    std::string toString() const override;
};

class EventoRM : public Evento {
public:
    std::string local;
    EventoRM(Data dh, int id, std::string l);
    std::string toString() const override;
};

class EventoUR : public Evento {
public:
    std::string local;
    EventoUR(Data dh, int id, std::string l);
    std::string toString() const override;
};

class EventoTR : public Evento {
public:
    std::string localOrigem;
    std::string localDestino;
    EventoTR(Data dh, int id, std::string orig, std::string dest);
    std::string toString() const override;
};

class EventoEN : public Evento {
public:
    std::string local;
    EventoEN(Data dh, int id, std::string l);
    std::string toString() const override;
};

#endif // EVENTO_H

