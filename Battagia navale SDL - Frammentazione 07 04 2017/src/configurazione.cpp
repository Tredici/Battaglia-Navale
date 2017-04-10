#include "..\include\configurazione.h"

int Configurazione::spessore_linee_griglie {4};

Configurazione::Configurazione()//Si appella alle funzioni della classe base per caricare i puntatori.
:   finestra { get_Window() },  //Probabilmente non servirà a nulla
    renderer { get_Renderer() },
    fonts { get_FontSet() }
{
    titolo.set_dim( get_Window_w()/2, static_cast<int>( altezza_striscia_titolo()*4.0/5.0 ) );
    titolo.set_centro( get_Window_w()/2, altezza_striscia_titolo()/2 );
    titolo.set_colore_sfondo(My_SDL_colors::blue);
    titolo.set_colore_testo(My_SDL_colors::green);
}

Configurazione::~Configurazione()//Non è necessario distruggere esplicitamente i puntatori definiti prima perché questo compito viene eseguito dalla superclasse che ci fornisce finestra, renderer e altro
{}


Esito Configurazione::stato()
{
    if( tabG2.flotta_distrutta() )
        return Esito::vittoria_giocatore;
    else if( tabG1.flotta_distrutta() )
        return Esito::vittoria_pc;
    else
        return Esito::in_corso;
}

void Configurazione::disegnaTitolo(void) noexcept
{
    disegnaRiquadro( fonts, renderer, &titolo );
}

void Configurazione::presenta()
{

    centra_griglie();

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);//Pulisce tutto con il bianco
    SDL_RenderClear(renderer);
    SDL_Rect rett {0,0, get_Window_w()/2, get_Window_h() };

    SDL_SetRenderDrawColor(renderer, coloreG1);
    SDL_RenderFillRect(renderer, &rett);

    rett.x += get_Window_w()/2;
    rett.w += 1;//Per evitare che se la finestra a larghezza dispari resti una colonnina di pixel bianchi
    SDL_SetRenderDrawColor(renderer, coloreG2);
    SDL_RenderFillRect(renderer, &rett);

    SDL_Rect banda_del_titolo { 0, 0, get_Window_w(), h_titolo };
    SDL_SetRenderDrawColor(renderer, coloreTitolo);
    SDL_RenderFillRect(renderer, &banda_del_titolo);

    disegnaTitolo();//Disegna il titolo del gioco

    //Dopo aver praparato tutto lo sfondo etc.
    //Disegna le due tabelle con tutte le navette etc
    tabG1.disegna(renderer, spessore_linee_griglie);
    tabG2.disegna(renderer, spessore_linee_griglie);

    tabG1.disegna_navi( renderer, spessore_linee_griglie/2 );
///************************** RIMUOVERE NELLA VERSIONE DEFINITIVA *****************
    tabG2.disegna_navi( renderer, spessore_linee_griglie/2 );
///************************** ************************** **************************

    //Disegna le celle colpite dai bombardamenti etc...
    tabG1.disegna_celle_colpite( renderer, spessore_linee_griglie/2 );
    tabG2.disegna_celle_colpite( renderer, spessore_linee_griglie/2 );

    if( has_to_present )
        SDL_RenderPresent(renderer);
}

void Configurazione::presenta_solo_sfondo()//Mostra solo il backgroung
{
    centra_griglie();

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);//Pulisce tutto con il bianco
    SDL_RenderClear(renderer);
    SDL_Rect rett {0,0, get_Window_w()/2, get_Window_h() };

    SDL_SetRenderDrawColor(renderer, coloreG1);
    SDL_RenderFillRect(renderer, &rett);

    rett.x += get_Window_w()/2;
    rett.w += 1;//Per evitare che se la finestra a larghezza dispari resti una colonnina di pixel bianchi
    SDL_SetRenderDrawColor(renderer, coloreG2);
    SDL_RenderFillRect(renderer, &rett);

    SDL_Rect banda_del_titolo { 0, 0, get_Window_w(), h_titolo };
    SDL_SetRenderDrawColor(renderer, coloreTitolo);
    SDL_RenderFillRect(renderer, &banda_del_titolo);

    //Dopo aver praparato tutto lo sfondo etc.
    //Disegna le due tabelle con tutte le navette etc
    tabG1.disegna(renderer, spessore_linee_griglie);
    tabG2.disegna(renderer, spessore_linee_griglie);

    if( has_to_present )
        SDL_RenderPresent( renderer );
}

void Configurazione::Griglia_battaglia_navale::disegna(SDL_Renderer* r, int spessore) const
/*
    Disegna la tabella in questione mediante l'utilizzo di linee sottili un (1) pixel.
    Magari in futuro saprò realizzare anche dei rettangoli con un bordo leggermente più spesso
*/
{
    SDL_Color old{};
    SDL_GetRenderDrawColor(r, &old);//Conserva il vecchio colore del renderer
    SDL_Color black {0, 0, 0, 255}; //Le tabelle vengono disegnate in nero
    SDL_SetRenderDrawColor(r, black);
    Linea_spessa tratto = Linea{x_angolo_in_alto_a_sinistra, y_angolo_in_alto_a_sinistra, x_angolo_in_alto_a_sinistra+lato*colonne, y_angolo_in_alto_a_sinistra};
    tratto.imposta_spessore( spessore );//Se lo spessore è minore di 1 lancia
    for(int i {}; i <= righe ; ++i)
    {
        SDL_RenderDrawLine(r, tratto);
        tratto.varia_y(lato);
    }
    tratto = Linea{x_angolo_in_alto_a_sinistra, y_angolo_in_alto_a_sinistra, x_angolo_in_alto_a_sinistra, y_angolo_in_alto_a_sinistra+lato*righe};
    tratto.imposta_spessore( spessore );
    for(int i {}; i <= colonne ; ++i)
    {
        SDL_RenderDrawLine(r, tratto);
        tratto.varia_x(lato);
    }
    SDL_SetRenderDrawColor(r, old);//Reimposta il vecchio colore come colore del renderer
}

void Configurazione::centra_griglie()
{
    int centro_x = get_Window_w()/4; //Si ricordi che le parti in cui è divisa la finestra sono due
    int centro_y = ( get_Window_h()-h_titolo )/2 + h_titolo;
    tabG1.set_y( centro_y - tabG1.get_righe() * Griglia_battaglia_navale::get_lato()/2.0 );
    tabG2.set_y( centro_y - tabG2.get_righe() * Griglia_battaglia_navale::get_lato()/2.0 );
    tabG1.set_x( centro_x - tabG1.get_colonne() * Griglia_battaglia_navale::get_lato()/2.0 );
    tabG2.set_x( centro_x - tabG1.get_colonne() * Griglia_battaglia_navale::get_lato()/2.0 + get_Window_w()/2 );
}

int Configurazione::altezza_striscia_titolo() const
{
    return h_titolo;
}

enum class Configurazione::Click : char
{
    fuori,
    tab1, tab2,
    altro //Riservato per impieghi futuri
};

/*std::pair<Configurazione::Click, Posizione> Configurazione::intercetta_click(SDL_Event& e)
{///Continua...
    int x, y;
    switch( e.type )
    {
    case SDL_QUIT:
        throw Quit_game_exception{"SDL_QUIT"};
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);
    }
}*/


Configurazione::Quit_game_exception::Quit_game_exception()
: s{"Quit_game_exception() thrown"}
{}

Configurazione::Quit_game_exception::Quit_game_exception(const std::string& s)
: s{s}
{}

const char* Configurazione::Quit_game_exception::what(void) const noexcept
{
    return s.c_str();
}

void Configurazione::set_spessore_linee(int s)
{
    if(s < 1)
        throw std::invalid_argument{ std::string{ "Configurazione::set_spessore_linee(...)->spessore minore di 1" } };
    spessore_linee_griglie = s;
}


Rettangolo_bordato ottieni_Rettangolo_bordato_da_Nave(const Nave& n, int lato_cella)
{///< Continua...
    Rettangolo_bordato rb;
    rb.w = lato_cella;//All'inizio viene imposatata la dimensione di un quadratino della cella
    rb.h = lato_cella;//Sia per quanto riguarda la larghezza sia per l'altezza
    if( n.orizzontale() )//La nave è orizzontale?
        rb.w *= n.lunghezza();//Se sì è tot volte più larga
    else
        rb.h *= n.lunghezza();//Altrimenti è tot volte più alta
    return rb;
}

//Si avvale della funzione helper di sopra per generare i vari rettangoli da disegnare
std::vector<Rettangolo_bordato> Configurazione::Griglia_battaglia_navale::navi_da_disegnare(void) const
{
    std::vector<Rettangolo_bordato> rettangoli;
    const std::vector<Nave>& navi = dati_flotta();//Ottiene l'elenco di tutte le navi da disegnare
    for(const auto& x : navi)
    {
        Rettangolo_bordato rb = ottieni_Rettangolo_bordato_da_Nave( x, Tabella_battaglia_navale::get_lato() );
        Posizione delta = posizione_angolo_cella( x.cella_prua() );
        trasla_rettangolo(&rb, delta.x, delta.y);
        rettangoli.push_back( rb );
    }
    return rettangoli;
}

void Configurazione::Griglia_battaglia_navale::disegna_navi(SDL_Renderer* r, int spessore_linee) const
{
    SDL_Color old;
    SDL_GetRenderDrawColor(r, &old);//Memorizza il colore iniziale del renderer
    SDL_SetRenderDrawColor(r, colore_navi);
    std::vector<Rettangolo_bordato> elenco_navi = navi_da_disegnare();
    for(auto& x : elenco_navi)
    {
        reduce(&x);//Per far si che quantomeno si intraveda la linea nera della griglia di separazione tra le navi
        x.set_bordo( spessore_linee*3 );//In modo che il bordo non appaia troppo grosso
        SDL_RenderFillRettangoloBordato(r, x, 150);
    }

    SDL_SetRenderDrawColor(r, old);//Reimposta lo status quo
}

void Configurazione::genera_flotta_pc()
{
    tabG2.genera_flotta_random();//La flotta del PC è generata casualmente come è giusto che sia
}


void Configurazione::genera_flotta_utente()
{
    ///In fase di sviluppo... ....................................................................................................
    bool remember { has_to_present };
    if( remember )
        has_to_present = false;

    const std::vector<size_t> navi_da_generare = Tabella_battaglia_navale::navi_di_una_flotta();
    size_t navi_inserite {};
    const size_t navi_da_inserire = navi_da_generare.size();

    bool metti_orizzontale = true;

    std::function< void(void) > cambia_nome = nullptr;

    std::function< void(void) > funzione_bottone = [&metti_orizzontale, &cambia_nome]() mutable { metti_orizzontale = !metti_orizzontale;
                                                                                    cambia_nome();
                                                                                    };
    //L'idea è quella di creare un pulsante con sopra una grande 'i' sul quale si possa clickare per ottenere delle informazioni su cosa fare
    std::function< void(void) > funzione_info = [this] //La funzione disegna un riquadro bianco con le spiegazioni e basta
                                                ()mutable
                                                {
                                                    auto spiegazione = crea_pulsante([]{});//Non deve fare assolutamente niente
                                                    spiegazione.set_dim( get_Window_w()/2, get_Window_h()/2 );
                                                    spiegazione.set_testo("Cliccare la cella dove si vuole inserire la prua della nave.\n"
                                                                          "Cliccare sul pulsante \"Verticale\" per cambiare il verso con il quale sara' inserita la nave.\n"
                                                                          "Cliccare su \"Undo\" per eliminare l'ultima nave inserita.\n"
                                                                          "\n"
                                                                          "Clickare sulla finestra per chiudere l'info."
                                                                          );
                                                    spiegazione.set_centro(get_Window_w()/2, get_Window_h()/2);
                                                    spiegazione.set_numero_linee_testo(7);//Vediamo se sette linee sono abbastanza
                                                    SDL_Event e;
                                                    int x{-1000}, y{-1000};

                                                    disegnaRiquadro(fonts, renderer, &spiegazione);//Alla fine è solo questo che bisogna disegnare

                                                    SDL_RenderPresent( renderer );
                                                    while(!spiegazione.clickato(x,y))
                                                    {
                                                        SDL_Delay(1);//Faccio attendere un millisecondo perchè non sono sicuro che far correre troppo un ciclo sia "bene"
                                                        if(SDL_PollEvent(&e)!=0)
                                                        {
                                                            if(e.type == SDL_QUIT)//Uno deve essere sempre libero di poter terminare un programma in qualsiasi momento
                                                                throw Quit_game_exception();
                                                            if(e.type == SDL_MOUSEBUTTONDOWN)
                                                                SDL_GetMouseState(&x,&y);
                                                        }
                                                    }
                                                };

    auto info = crea_pulsante(funzione_info);
    info.set_pos(10, altezza_striscia_titolo() + 200);
    info.set_dim(60, 50);
    info.set_testo( std::string{"info"} );//Una semplice i
    info.set_colore_testo(My_SDL_colors::blue);
    info.set_colore_sfondo(My_SDL_colors::cyan);

    auto bottone = crea_pulsante(funzione_bottone);//Il bottone serve per mettere o meno le navi orizzontali
    bottone.set_pos( 10 , altezza_striscia_titolo() + 10 );
    bottone.set_testo( std::string( "Orizzontale?" ) );

    cambia_nome = [&bottone, &metti_orizzontale]() mutable {
                                            if( metti_orizzontale )
                                                bottone.set_testo( std::string( "Orizzontale?" ) );
                                            else
                                                bottone.set_testo( std::string( "Verticale?" ) );
                                            };

    bottone.set_dim(150, 40);
    bottone.on_off( true );

    bottone.set_colori_sfondo_1_2( &My_SDL_colors::green, &My_SDL_colors::cyan );

    auto nave_attuale = navi_da_generare[ navi_inserite ];

    const std::string inizio_messaggio( "Clicca sulle caselle della tabella dove vuoi inserire la prua della nave " );
    auto num_to_string = [](int i){ char risp[17] {};
                                    sprintf(risp, "%d", i);
                                    return std::string( risp ); };

    Riquadro messaggio( inizio_messaggio + num_to_string( nave_attuale ) );
    messaggio.set_dim(get_Window_w()/2, 30);
    messaggio.set_centro( get_Window_w()/2, altezza_striscia_titolo()+5 );

    std::function<void(void)> funzione_undo = [&, this]()mutable   {   elimina_ultima_nave_inserita_G1();
                                                                    if(navi_inserite)
                                                                    {
                                                                        --navi_inserite;
                                                                        nave_attuale = navi_da_generare[ navi_inserite ];
                                                                        messaggio.set_testo( inizio_messaggio + num_to_string( nave_attuale ) );
                                                                    }
                                                                };//Semplice semplice

    auto undo = crea_pulsante(funzione_undo);
    undo.set_testo( std::string{"Undo"} );
    undo.set_colore_testo(My_SDL_colors::yellow);
    undo.set_colore_sfondo(My_SDL_colors::purple);
    undo.set_pos(10, altezza_striscia_titolo() + 100);
    undo.set_dim(90, 45);

    SDL_Event e;
    do{
        while( SDL_PollEvent( &e ) != 0 )
        {
            switch( e.type )
            {
                case SDL_QUIT:
                    throw Quit_game_exception{"E' finita :(."};
                    break;//Istruzione cbe non potrà mai essere eseguita però almeno evidenza la separazione che sussiste tra i vari case
                case SDL_MOUSEBUTTONDOWN:///< Completare alla svelta
                    {
                        int x,y;
                        SDL_GetMouseState( &x, &y );
                        if( tabG1.punto_interno(x,y) )
                        {
                            Indice_cella cella_prova = tabG1.cella_clickata(x,y);// La cella in cui si prova a inserire una nave
                            Nave nuova_nave( nave_attuale, metti_orizzontale );
                            nuova_nave.set_prua( cella_prova );
                            try
                            {
                                tabG1.aggiungi_nave( nuova_nave );//Prova ad aggiungere la nave alla tabella
                                navi_inserite++;
                                nave_attuale = navi_da_generare[ navi_inserite ];
                                messaggio.set_testo( inizio_messaggio + num_to_string( nave_attuale ) );//Nuova nave nuovo testo
                            }
                            catch( std::invalid_argument& i_a )//Se non ci riesce viene lanciata un'eccezione
                            {
                                ;
                            }

                        }
                        else if( bottone.clickato(x,y) )//Pulsante cliccato
                        {
                            bottone();//Esegue il pulssante
                        }
                        else if( info.clickato(x,y) )//Si richiedono spiegazioni?
                        {
                            info();//Mostra l'info
                        }
                        else if( undo.clickato(x,y) )
                        {
                            undo();
                        }

                    }
                    break;
            }

            presenta();//Presenta il background
            disegnaRiquadro( fonts, renderer, &bottone ); //Dopodiché aggiunge le immagini e i bottoni
            if(navi_inserite < navi_da_inserire)//Alla fine non è necessario mostrarlo
                disegnaRiquadro( fonts, renderer, &messaggio );
            disegnaRiquadro( fonts, renderer, &undo );
            disegnaRiquadro( fonts, renderer, &info );
            SDL_RenderPresent( renderer );

            ///< Aggiungere poi un'ultimo controllo per quando vengono inserite tutte le navi per dare la possibilità di effettuare un'ultima modifica alla disposizione della flotta
            if(navi_inserite == navi_da_inserire)
            {
                const int larghezza = 1.5*get_Window_w()/4;
                bool esci {};

                auto continua = crea_pulsante([&esci]()mutable
                                                   { esci = true; }
                                                   );
                continua.set_testo("Continua");
                continua.set_dim(larghezza, 70);

                Riquadro ultimatum{ "Sei sicuro di voler usare questa disposizione delle navi?\n"
                                    "Se si' clicka continua, altrimenti clicka \"Undo\""};//Prima o poi deciderò il colore giuto per questo
                ultimatum.set_numero_linee_testo(3);
                SDL_Rect rect_ultimatum{0, 0, larghezza, static_cast<int>(1.5*(get_Window_h()-altezza_striscia_titolo())/4) };
                set_centro_SDL_Rect(&rect_ultimatum, { 3*get_Window_w()/4, (altezza_striscia_titolo()+(get_Window_h()-altezza_striscia_titolo())/2 - continua.get_riq().h/2) });
                ultimatum.set_riq( rect_ultimatum );
                impila(&ultimatum, &continua);
                ultimatum.set_colore_sfondo(My_SDL_colors::yellow);
                continua.set_colore_sfondo(My_SDL_colors::green);

                SDL_Event e;//Binding dei nomi
                while(!esci)
                {
                    presenta();
                    disegnaRiquadro( fonts, renderer, &bottone );
                    disegnaRiquadro( fonts, renderer, &undo );
                    disegnaRiquadro( fonts, renderer, &ultimatum );
                    disegnaRiquadro( fonts, renderer, &continua );

                    SDL_RenderPresent(renderer);
                    while( SDL_PollEvent(&e) != 0 )
                    {
                        switch(e.type)
                        {
                        case SDL_QUIT:
                            throw Quit_game_exception();
                        case SDL_MOUSEBUTTONDOWN:
                            {
                                int x,y;
                                SDL_GetMouseState(&x,&y);
                                if(continua.clickato(x,y))
                                {
                                    continua();
                                }
                                else if(undo.clickato(x,y))
                                {
                                    undo();
                                    esci = true;
                                }
                            }

                            break;
                        }
                    }

                }
            }
        }
    }while( navi_inserite < navi_da_inserire);


    if( remember )
        has_to_present = true;
}

Rettangolo_bordato Configurazione::Griglia_battaglia_navale::casella_bordata(Indice_cella i) const
{///Continua...
    int x = x_angolo_in_alto_a_sinistra;
    int y = y_angolo_in_alto_a_sinistra;
    x += i.colonna * lato;
    y += i.riga * lato;
    return Rettangolo_bordato{ x, y, lato, lato };
}
std::vector<Rettangolo_bordato> Configurazione::Griglia_battaglia_navale::celle_colpo_a_segno(void) const
{///Continua...
    std::vector<Indice_cella> caselle = celle_contenenti( Risultato_sparo::colpito );
    std::vector<Rettangolo_bordato> rettangoli;
    rettangoli.reserve( caselle.size() );
    for(auto& x : caselle)
    {
        rettangoli.push_back( casella_bordata( x ) );
    }
    return rettangoli;
}

std::vector<Rettangolo_bordato> Configurazione::Griglia_battaglia_navale::celle_colpo_in_acqua(void) const
{
    std::vector<Indice_cella> caselle = celle_contenenti( Risultato_sparo::acqua );
    std::vector<Rettangolo_bordato> rettangoli;
    rettangoli.reserve( caselle.size() );
    for(auto& x : caselle)
    {
        rettangoli.push_back( casella_bordata( x ) );
    }
    for_each( rettangoli.begin(), rettangoli.end(), [](Rettangolo_bordato& rb) mutable { rb.b = spessore_linee_griglie; } );
    return rettangoli;
}


bool Configurazione::colpisci_cella_tab1(Indice_cella cas) throw(Casella_gia_colpita_exception,std::invalid_argument)
{
    return tabG1.colpisci( cas );//Se è colpibile colpisce, altrimenti lancia
}

bool Configurazione::colpisci_cella_tab2(Indice_cella cas) throw(Casella_gia_colpita_exception,std::invalid_argument)
{
    return tabG2.colpisci( cas );
}

void Configurazione::Griglia_battaglia_navale::disegna_celle_colpite(SDL_Renderer* r, int spessore_linee) const
{
    SDL_Color old = SDL_GetRenderDrawColor( r );//Trova il colore iniziale del renderer
    disegna_celle_colpo_a_segno( r, spessore_linee );
    disegna_celle_colpo_in_acqua( r,spessore_linee );
    SDL_SetRenderDrawColor( r, old );//Reimposta il colore iniziale
}

void Configurazione::Griglia_battaglia_navale::disegna_celle_colpo_a_segno(SDL_Renderer* r, int spessore_linee) const
{///< Continua...
    std::vector<Rettangolo_bordato> celle_boom = celle_colpo_a_segno();
    SDL_SetRenderDrawColor( r, colore_celle_boom );
    for( auto& x : celle_boom )
    {
        x.set_bordo( spessore_linee );
        SDL_RenderFillRettangoloBordato( r, x, 150 );
    }
}

void Configurazione::Griglia_battaglia_navale::disegna_celle_colpo_in_acqua(SDL_Renderer* r, int spessore_linee) const
{///< Continua...
    std::vector<Rettangolo_bordato> celle_splah = celle_colpo_in_acqua();
    SDL_SetRenderDrawColor( r, colore_celle_acqua );
    for( auto& x : celle_splah )
    {
        x.set_bordo( spessore_linee );
        SDL_RenderFillRettangoloBordato( r, x, 150 );
    }
}

void Configurazione::intecetta_click_G1_e_colpisci() throw(Casella_gia_colpita_exception,std::invalid_argument)
//Una "std::invalid_argument" dovrebbe mai essere lanciata perché il controllo per la cella interna viene fatto prima di 'sparare'
{//Parentesi utilizzate per contenere l'ambito di visibilità delle due variabili "x" e "y"
    Indice_cella cella  {};//La cella sulla quale si esegue il click
    int x, y;
    SDL_GetMouseState(&x,&y);//Dove è avvenuto il click?
    if( tabG2.punto_interno( x, y) )//All'interno della tabella?
    {
        cella = tabG2.cella_clickata(x,y);//Che cella è stata clickata?
        tabG2.colpisci( cella );//Prova a colpire la cella
    }
    else throw std::invalid_argument{ std::string{"Non è stata clickata alcuna cella"} };
}

void Configurazione::elimina_ultima_nave_inserita_G1() noexcept
{
    tabG1.elimina_ultima_nave_aggiunta();
}

void Configurazione::elimina_ultima_nave_inserita_G2() noexcept
{
    tabG2.elimina_ultima_nave_aggiunta();
}

