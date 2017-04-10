#include "..\include\indice_cella.h"

bool Indice_cella::esterno()
{
    return riga==valore_esterno||colonna==valore_esterno;
}

bool operator==(Indice_cella a, Indice_cella b)
{
    return a.riga==b.riga && a.colonna==b.colonna;
}

bool operator!=(Indice_cella a, Indice_cella b)
{
    return !(a==b);
}

//Quando conoscerò per bene quali sono e come funzionano i vari generatori di numeri casuali della libreria standard  magari ricontrollerò la funzione
Indice_cella indice_cella_random(int max_riga, int max_colonna)
{
    static bool initialized {false};
    if( !initialized )//Non so quanto costosa sia l'inizializzazione del generatore di numeri casuali così per sicurezza lo inizializzo solo la prima volta
    {
        srand( time(nullptr) );//Inizializza il generatore di numeri casuali
        initialized = true;
    }
    return Indice_cella{ rand() % (max_riga+1), rand() % (max_colonna+1) };
}

Indice_cella ottieni_indice_cella_da_matrice(unsigned int num_elem, unsigned int num_colonne)
{
    return Indice_cella{ num_elem/num_colonne, num_elem%num_colonne };
}

