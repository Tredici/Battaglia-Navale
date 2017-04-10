#ifndef INDICE_CELLA_H
#define INDICE_CELLA_H

#include <ctime>
#include <cstdlib>

struct Indice_cella
{
    Indice_cella() : riga{}, colonna{} {}
    Indice_cella(int r, int c) : riga{r}, colonna{c} {}
    bool esterno();
    int riga;
    int colonna;
    static const int valore_esterno = -1;
};
bool operator==(Indice_cella, Indice_cella);
bool operator!=(Indice_cella a, Indice_cella b);
Indice_cella indice_cella_random(int max_riga, int max_colonna);//Genera un Indice_cella a caso con i membri con valore contenuto tra 0 e max_riga/colonna
Indice_cella ottieni_indice_cella_da_matrice(unsigned int num_elem, unsigned int num_colonne);

#endif // INDICE_CELLA_H
