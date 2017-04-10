#ifndef TABELLA_BATTAGLIA_NAVALE_H
#define TABELLA_BATTAGLIA_NAVALE_H

#include <vector>
#include <algorithm>

#include "indice_cella.h"
#include "nave.h"

struct Posizione//Indica la posizione di un punto
{
    Posizione() =default;
    constexpr Posizione(int x, int y) : x{x}, y{y} {}
    constexpr Posizione(const Posizione&) =default;
    constexpr Posizione(Posizione&&) =default;
    int x{}, y{};
};

enum class Risultato_sparo : char
{
    vuoto,  //Su quella casella nulla è ancora stato tentato
    acqua,
    colpito
};

bool random_bool(void);

class Casella_gia_colpita_exception {};

class Tabella_battaglia_navale
{
    std::vector<Nave> flotta;//Il numero delle navi possedute dalla flotta
    static const std::vector<size_t> dimensioni_navi;
    static const unsigned int dimensione_flotta;
    size_t navi_valide;//Il numero di navi che compongono la flotta, all'inizio vale "flotta.size()"

    Nave genera_nave_random(int dim) const;//usa il generatore di n. casuali per realizzare e posizionare una nave di dim "dim"
    void add_nave(Nave n);//Aggiunge una nave alla flotta se e solo se siamo sicuri si possa farlo
    int colonna(int x) const;
    int riga(int y) const;

    bool ult_colpo_a_segno {};
    bool ult_colpo_nave_affondata {};

protected:
    int righe;
    int colonne;

    class Matrice
    {
        Risultato_sparo** tab{};
        int r{}, c{};
    public:
        int righe() const { return r; }
        int colonne() const { return c; }
        Matrice(int r, int c);
        ~Matrice();
        Risultato_sparo& operator[](Indice_cella i);
        Risultato_sparo operator[](Indice_cella i) const;
        Matrice(const Matrice&) =delete;
        Matrice& operator=(const Matrice&) =delete;
        bool gia_mirato(Indice_cella i) const;//Dice se la cella in questione è già stata bersaglio di un colpo
        unsigned int conta(Risultato_sparo cosa) const noexcept;
        std::vector<Indice_cella> celle_contenenti(Risultato_sparo cosa) const;
    };
    Matrice matrice;

    static int lato;//Per semplicità facciamo le celle tutte quadrate | IN SEGUITO VA TRASFORMATO IN UN MEMBRO PRIVATE CON IL QUALE SI POTRA' PERO' INTERAGIRE MEDIANTE FUNZIONI SICURE (cosa che permetterà di cambiare la dimensione della grigli a runtime)
    //il limite è 0,0
    int x_angolo_in_alto_a_sinistra {};
    int y_angolo_in_alto_a_sinistra {};

    const std::vector<Nave>& dati_flotta(void) const { return flotta; }

    public:
        /*class Iterator{
            Risultato_sparo* elem;
        public:
            operator*() { return elem; }
            Risultato_sparo* operator++() {  }
        };*/

        static int get_lato(void){ return lato; }
        int get_x(void) const { return x_angolo_in_alto_a_sinistra; }
        int get_y(void) const { return y_angolo_in_alto_a_sinistra; }
        void set_x(int x);
        void set_y(int y);
        void set_xy(int x, int y);
        int get_righe(void) const { return righe; }
        int get_colonne(void) const { return colonne; }

        Tabella_battaglia_navale();
        Tabella_battaglia_navale(int r, int c);
        ~Tabella_battaglia_navale() =default;//Per ora il distruttore appare come un distruttore perfettamente normale e pertanto quello di default andrà benissimo

        bool nave_inseribile(const Nave&) const;
        void aggiungi_nave(Nave) throw( std::invalid_argument );

        void genera_flotta_random();   //Si avvale di genera navi random per generare
        bool flotta_distrutta() const; //Ci sono ancora navi non affondate

        bool punto_interno(int x, int y) const noexcept;
        Indice_cella cella_clickata(int x, int y) const noexcept;

        bool colpisci(Indice_cella i) throw(std::invalid_argument,Casella_gia_colpita_exception);//Spara un colpo in quel punto e vede che succede (centra una nave o no?)
        bool cerca_nave(Indice_cella i) const;//In quella cella c'è una nave?
        Nave* nave_cella(Indice_cella i) noexcept;

        int num_navi_flotta(void) const { return dimensione_flotta; }  //Restituisce il numero di navi che compongono la flotta

        bool cella_interna(Indice_cella i) const;
        Posizione posizione_angolo_cella(Indice_cella i) const;

        unsigned int conta(Risultato_sparo cosa) const noexcept;

        std::vector<Indice_cella> celle_contenenti(Risultato_sparo cosa) const;

        static std::vector<size_t> navi_di_una_flotta(void) noexcept { return dimensioni_navi; }

        bool ultimo_colpo_a_segno() const noexcept { return ult_colpo_a_segno; }
        bool ultimo_colpo_nave_affondata() const noexcept { return ult_colpo_nave_affondata; }

        void elimina_ultima_nave_aggiunta() noexcept;
};

#endif // TABELLA_BATTAGLIA_NAVALE_H
