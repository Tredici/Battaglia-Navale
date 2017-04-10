#ifndef NAVE_H
#define NAVE_H

#include <vector>
#include <stdexcept>

#include "indice_cella.h"

class Nave
{
        int length;//Deve essere almeno 1
        bool orizz; //La nave è in posizione orizzontale? | una volta che la posizione viene impostata al momento della creazione dovrebbe rimanere fissa (se il progetto non subisce modifiche)
        Indice_cella prua;//Posizione della parte più in alto a sinistra della nave
        std::vector<bool> parti;//La nave è formata da length quadrati e il vector salva se questi sono stati colpiti o meno
    public:
        Nave(int, bool);
        bool fuoco(Indice_cella);//"Fa fuoco" sulla cella in questione e dice se la nave è stata colpita?
        bool affondato() const;
        bool intercept(const Nave&) const;
        bool cella_interna(Indice_cella i) const;
        bool orizzontale() const { return orizz; }
        int lunghezza() const { return length; }
        Indice_cella cella_prua() const { return prua; }
        Indice_cella cella_poppa() const;

        void set_prua(Indice_cella i) { prua = i; }
        Nave(const Nave&) =default;
        Nave& operator=(const Nave&) =default;
};

#endif // NAVE_H
