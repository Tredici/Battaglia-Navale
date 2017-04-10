#ifndef GIOCO_H
#define GIOCO_H

#include "configurazione.h"

///extern enum class Esito : char;//Si possono usare gli operatori bit a bit per estrapolare informazioni multiple


class Gioco : public Configurazione
{
        Indice_cella cella_random(void);//Devo capire come usare un generatore di numeri casuali decente per ottenere le coordinate di una cella qualsiasi da bombardare
        bool gia_colpito(Indice_cella);//Funzione helper per quella di sopra che verifica se una determinata cella è già stata colpita, ovviamente non si puà colpire più volte la stesssa cella
        bool started {false};

        std::string mess_vittoria {"HAI VINTO"};
        std::string mess_sconfitta {"Hai perso, riprova!"};
    public:
        Gioco();
        void genera_flotte();
        void mossa_giocatore();
        void mossa_pc();
        //void inizia();//disegna lo schermo la tabella etc.
        void gioca();
        void messaggio_finale();

        //void disegna(void);//Disegna tutto com'è in quel momento
};

#endif // GIOCO_H
