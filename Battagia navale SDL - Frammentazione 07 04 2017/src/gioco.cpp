#include "..\include\gioco.h"

Gioco::Gioco()
{
    //ctor
}



void Gioco::genera_flotte()
{
    genera_flotta_pc();
    genera_flotta_utente();
}

void Gioco::gioca()
{
    genera_flotte();

    int turno_g1 { true };
    while( stato() == Esito::in_corso )//Mentre il gioco prosegue
    {
        if( turno_g1 )
            mossa_giocatore();
        else
            mossa_pc();

        turno_g1 = !turno_g1;
        presenta();
        SDL_Delay(100);//Diamo il tempo alla gente di metabolizzare cosa è accaduto
    }
    messaggio_finale();
}


void Gioco::mossa_giocatore()
{
    //percepisce il clic dell'utente
    //se è in un punto valido opera sulla cella cliccata
    //altrimenti aspetta il click successivo
    bool ha_agito {};
    SDL_Event e;

    while( !ha_agito )//Ciclo principale del gioco
    {
        while( SDL_PollEvent(&e) != 0 )
        {
            switch( e.type )
            {
                case SDL_QUIT:
                    throw Quit_game_exception {};
                    break;
                case SDL_MOUSEBUTTONDOWN://V'è stato un click
                    ///<Continua...
                    try//Prova
                    {
                        intecetta_click_G1_e_colpisci();//Prova a colpire la cella
                        ha_agito = true;
                    }
                    catch( Casella_gia_colpita_exception )//E' stata clickata una cella già colpita
                    {}
                    catch( std::invalid_argument )
                    {}
                    break;
            }
        }
        presenta();
    }
}

void Gioco::mossa_pc()
{
    Indice_cella cella  {};
    //Non v'è ragione per cui questi due valori debbano cambiare a run time, pertanto si può risparmiare sul numero di chiamate a funzione
    static int righe { righe_tabelle() };
    static int colonne { colonne_tabelle() };

    bool centrato {};//false
    do{//Ripete fino a che non fa centro
        cella = indice_cella_random( righe-1, colonne-1 );
        try
        {
            colpisci_cella_tab1( cella );
            centrato = true;
        }
        catch( std::invalid_argument i_a )
        {}
        catch( Casella_gia_colpita_exception cgc )
        {}

    }while( !centrato );
}

void Gioco::messaggio_finale()
{
    SDL_Renderer* rend = get_Renderer();
    TTF_Font* fonts = get_FontSet();
    Riquadro mess_finale { ( stato() == Esito::vittoria_giocatore ? mess_vittoria : mess_sconfitta ) };
    mess_finale.set_frazione_testo(0.75f);
    int w = get_Window_w();
    int h = get_Window_h();
    int w_mess {w};
    int h_mess {h};
    float moltiplicatore { 0.25f };
    h_mess *= moltiplicatore; w_mess *= moltiplicatore;
    mess_finale.set_dim( w_mess, h_mess );
    mess_finale.set_centro( w/2, h/2 );
    mess_finale.set_colore_testo( My_SDL_colors::blue );
    mess_finale.set_colore_sfondo( My_SDL_colors::orange );
    presenta();
    disegnaRiquadro( fonts, rend, &mess_finale );
    SDL_RenderPresent( rend );
    SDL_Delay(2500);
    presenta();
    SDL_Delay(250);
}





