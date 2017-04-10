#include "..\include\tabella_battaglia_navale.h"

const std::vector<size_t> Tabella_battaglia_navale::dimensioni_navi {5,4,3,2,1}; //5 navi di base (generati in quest'ordine per non avere troppi problemi con il generatore di numeri casuali
const unsigned int Tabella_battaglia_navale::dimensione_flotta = Tabella_battaglia_navale::dimensioni_navi.size();

bool random_bool(void)
{
    static bool initialized {false};
    if( !initialized )//Non so quanto costosa sia l'inizializzazione del generatore di numeri casuali così per sicurezza lo inizializzo solo la prima volta
    {
        srand( time(nullptr) );//Inizializza il generatore di numeri casuali
        initialized = true;
    }
    return ( rand()%2 );//Se il numero è pari ritorna false, se è dispari ritorna true
}

int Tabella_battaglia_navale::lato {50};

Tabella_battaglia_navale::Tabella_battaglia_navale()
: Tabella_battaglia_navale(8,8)
{}

Tabella_battaglia_navale::Tabella_battaglia_navale(int r, int c)
: navi_valide{dimensione_flotta}, righe{r}, colonne{c}, matrice{r,c}
{
    if(r<1||c<1)
        throw std::logic_error{"Dimessone tabella impossibile"};
}


bool Tabella_battaglia_navale::nave_inseribile(const Nave& n) const
{
    //Controlla se la nave per una qualche ragione esce fuori dalla griglia
    if( !cella_interna( n.cella_poppa() ) )
        return false;
    for(auto& x :  flotta)//Controlla se per caso la nave si interseca con altre navi già presenti
    {
        if( n.intercept( x ) )
        {
            return false;
        }
    }
    return true;
}


bool Tabella_battaglia_navale::flotta_distrutta() const
{
    return !navi_valide;
}

Nave Tabella_battaglia_navale::genera_nave_random(int dim) const
{
    Nave nave { dim, random_bool() };
    nave.set_prua( indice_cella_random( righe-1, colonne-1 ) );

    return nave;
}

void Tabella_battaglia_navale::genera_flotta_random()///*************************************************************
{
    for(auto x : dimensioni_navi)//Crea il giusto numero di navi nella flotta
    {
        Nave nave(1,true);//Simula un costruttore predefinito visto che Nave non ne ha uno per questioni di sicurezza
        do{//Si ripete finché non trova che la nave può essere inserita
            nave = genera_nave_random(x);
        }while( !nave_inseribile(nave) );
        flotta.push_back( nave );
    }
}

void Tabella_battaglia_navale::add_nave(Nave n)
{
    flotta.push_back(n);
}


void Tabella_battaglia_navale::set_x(int x)
{
    if(x<0)
        throw std::invalid_argument{ std::string{ "Tabella_battaglia_navale::set_x(...)->x<0" } };
    x_angolo_in_alto_a_sinistra = x;
}

void Tabella_battaglia_navale::set_y(int y)
{
    if(y<0)
        throw std::invalid_argument{ std::string{ "Tabella_battaglia_navale::set_y(...)->y<0" } };
    y_angolo_in_alto_a_sinistra = y;
}

void Tabella_battaglia_navale::set_xy(int x, int y)
{
    set_x(x); set_y(y);
}

bool Tabella_battaglia_navale::punto_interno(int x, int y) const noexcept
{
    return x_angolo_in_alto_a_sinistra <= x && x <= x_angolo_in_alto_a_sinistra + lato*colonne
        && y_angolo_in_alto_a_sinistra <= y && y <= y_angolo_in_alto_a_sinistra + lato*righe;
}

Indice_cella Tabella_battaglia_navale::cella_clickata(int x, int y) const noexcept
{
    return {riga(y), colonna(x)};
}

int Tabella_battaglia_navale::colonna(int x) const
{
    if(x < x_angolo_in_alto_a_sinistra)
        return Indice_cella::valore_esterno;
    x -= x_angolo_in_alto_a_sinistra;
    int i{};
    while(i<colonne)
    {
        if(x < (i+1)*lato)
            return i;
        ++i;
    }
    return Indice_cella::valore_esterno;
}

int Tabella_battaglia_navale::riga(int y) const
{
    if(y < y_angolo_in_alto_a_sinistra)
        return Indice_cella::valore_esterno;
    y -= y_angolo_in_alto_a_sinistra;
    int i{};
    while(i<righe)
    {
        if(y < (i+1)*lato)
            return i;
        ++i;
    }
    return Indice_cella::valore_esterno;
}

/*enum class Risultato_sparo : char
{
    vuoto,  //Su quella casella nulla è ancora stato tentato
    acqua,
    colpito
};*/


bool Tabella_battaglia_navale::colpisci(Indice_cella i) throw(std::invalid_argument,Casella_gia_colpita_exception)
{
    if( !cella_interna(i) )
        throw std::invalid_argument{ std::string{ "Tabella_battaglia_navale::colpisci(...)->Cella esterna" } };

    if( matrice[i] != Risultato_sparo::vuoto )//Se sì è già stata colpita?
        throw Casella_gia_colpita_exception{};//In tal caso lancia

    for(auto& x : flotta)//cerca nella flotta
    {
        if( x.cella_interna(i) )//La nave x passa per la cella in questione?
        {

            matrice[i] = Risultato_sparo::colpito;
            ult_colpo_a_segno = true;
            x.fuoco(i);
            if( x.affondato() )
            {
                ult_colpo_nave_affondata = true;
                --navi_valide;
            }
            return true;
        }
    }
    matrice[i] = Risultato_sparo::acqua;
    ult_colpo_a_segno = false;
    ult_colpo_nave_affondata = false;
    return false;
}

bool Tabella_battaglia_navale::cerca_nave(Indice_cella i) const
{
    for(auto& x : flotta)//Cerca in tutte le navi della flotta
        if( x.cella_interna(i) )//Una di queste "passa" per la cella in questione?
            return true;
    return false;
}

Nave* Tabella_battaglia_navale::nave_cella(Indice_cella i) noexcept
{
    for(auto& x : flotta)
        if( x.cella_interna(i) )
            return &x;
    return nullptr;
}


Tabella_battaglia_navale::Matrice::Matrice(int r, int c)
: r{r}, c{c}
{
    if(r < 1 || c < 1)
        throw std::invalid_argument{ std::string{ "Matrice(...)->dimensioni impossibili" } };
    tab = new Risultato_sparo*[r];
    for(int i=0; i<r; ++i)
    {
        tab[i] = new Risultato_sparo[c];
        for(int j=0; j<c; ++j)
            tab[i][j] = Risultato_sparo::vuoto;
    }
}

Tabella_battaglia_navale::Matrice::~Matrice()
{
    for(int i=0; i<righe(); ++i)
        delete[] tab[i];
    delete[] tab;
}

Risultato_sparo& Tabella_battaglia_navale::Matrice::operator[](Indice_cella i)
{
    if(i.riga >= righe() || i.colonna >= colonne())
        throw std::out_of_range{ std::string{"Matrice::operator[](...)->segmentation fault risk"} };
    return tab[i.riga][i.colonna];
}

Risultato_sparo Tabella_battaglia_navale::Matrice::operator[](Indice_cella i) const
{
    if(i.riga >= righe() || i.colonna >= colonne())
        throw std::out_of_range{ std::string{"Matrice::operator[](...)->segmentation fault risk"} };
    return tab[i.riga][i.colonna];
}

bool Tabella_battaglia_navale::Matrice::gia_mirato(Indice_cella i) const
{
    if( this->operator[](i) == Risultato_sparo::acqua )
        return false;
    return true;
}

bool Tabella_battaglia_navale::cella_interna(Indice_cella i) const
{
    if( i.riga < 0 || i.colonna < 0 )
        return false;
    if(i.riga > righe-1 || i.colonna > colonne-1)
        return false;
    return true;
}

Posizione Tabella_battaglia_navale::posizione_angolo_cella(Indice_cella i) const
{
    if( !cella_interna(i) )
    {
        throw std::invalid_argument{ std::string{ "Tabella_battaglia_navale::posizione_angolo_cella(Indice_cella)->cella esterna" } };
    }
    return Posizione {x_angolo_in_alto_a_sinistra+lato*i.colonna, y_angolo_in_alto_a_sinistra+lato*i.riga};
}

unsigned int Tabella_battaglia_navale::conta(Risultato_sparo cosa) const noexcept
{
    return matrice.conta( cosa );
}

unsigned int Tabella_battaglia_navale::Matrice::conta(Risultato_sparo cosa) const noexcept
{
    unsigned int contatore {};
    for(int i {}; i < r; ++i)
    {
        contatore += std::count( &tab[i][0], &tab[i][c], cosa );
    }
    return contatore;
}

std::vector<Indice_cella> Tabella_battaglia_navale::celle_contenenti(Risultato_sparo cosa) const
{
    return matrice.celle_contenenti( cosa );
}

std::vector<Indice_cella> Tabella_battaglia_navale::Matrice::celle_contenenti(Risultato_sparo cosa) const
{
    std::vector<Indice_cella> cose;
    for(int i {}; i < r; ++i)
    {
        for(int j {}; j < c; ++j)
        {
            if( tab[i][j] == cosa)
                cose.push_back( Indice_cella{i,j} );
        }
    }
    return cose;
}

void Tabella_battaglia_navale::aggiungi_nave(Nave n) throw( std::invalid_argument )
{
    if( !nave_inseribile(n) )
        throw std::invalid_argument{ std::string{ "Nave non inseribile" } };
    add_nave(n);
}

void Tabella_battaglia_navale::elimina_ultima_nave_aggiunta() noexcept
{
    if(!flotta.empty())
        flotta.pop_back();
}

