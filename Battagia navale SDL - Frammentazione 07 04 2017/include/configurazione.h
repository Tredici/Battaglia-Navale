#ifndef CONFIGURAZIONE_H
#define CONFIGURAZIONE_H

#include <string>
#include <functional>
#include <utility>
#include <cstdio>

#include "sdl_strumenti.h"
#include "tabella_battaglia_navale.h"
#include <SDL.h>

//Ottiene il rettangolo bordato per disegnare la nave in seguito | Nota: (x;y) valgono (0;0) nel risultato
//Pertanto, per "correggere" la posizione del rettangolo, bisogna effettuare qualche clacolo di correzione.
//La figura fornita ha bordo 1 inizialmente
Rettangolo_bordato ottieni_Rettangolo_bordato_da_Nave(const Nave& n, int lato_cella);


enum class Esito : char//Si possono usare gli operatori bit a bit per estrapolare informazioni multiple
{
    in_corso=0, teminato=1, vittoria_giocatore=3, vittoria_pc=5
};

//Configurazione estende la classe per gestire le opzioni base di SDL
class Configurazione : public virtual SDL_Fondamentals
{
        SDL_Window* finestra {nullptr};
        SDL_Renderer* renderer {nullptr};
        TTF_Font* fonts {nullptr};

        bool has_to_present {true};

        Riquadro titolo{ std::string( "Delfini nani!" ) };

        void disegnaTitolo(void) noexcept;

    class Griglia_battaglia_navale : public Tabella_battaglia_navale//Estende la classe tabella per aggiungerci una funzione disegna e basta
    {
        SDL_Color colore_navi {143, 0, 255, 255};//Il viola è impostato particolarmente a caso
        SDL_Color colore_celle_acqua = My_SDL_colors::blue;// { My_SDL_colors::blue };
        SDL_Color colore_celle_boom  = My_SDL_colors::orange;// { My_SDL_colors::orange };

        Rettangolo_bordato casella_bordata(Indice_cella i) const;//Restituisce un rettangolo bordato relativo a una determinata cella
        void disegna_celle_colpo_a_segno(SDL_Renderer* r, int spessore_linee) const;
        void disegna_celle_colpo_in_acqua(SDL_Renderer* r, int spessore_linee) const;

        std::vector<Rettangolo_bordato> celle_colpo_a_segno(void) const;//Restituisce dei quadratini bordati localizzati sulle celle in cui i colpi hanno centrato delle navi
        std::vector<Rettangolo_bordato> celle_colpo_in_acqua(void) const;//Lo stesso ma i quadrati
    public:
        using Tabella_battaglia_navale::Tabella_battaglia_navale;//Importa i costruttori
        void disegna(SDL_Renderer*, int) const;//L'intero indica lo spessore delle linee

        std::vector<Rettangolo_bordato> navi_da_disegnare(void) const;//Fornisce tutti i rettangoli da disegnare
        void disegna_navi(SDL_Renderer* r, int spessore_linee) const;//Dovrà lavorare per entrambe le griglie, quella del giocatore e quella del PC
        void disegna_celle_colpite(SDL_Renderer* r, int spessore_linee) const;//Sarà fatta funzionare solo nel caso della griglia del giocatore (altrimenti significherebbe barare)
    };

        Griglia_battaglia_navale tabG1;//8x8
        Griglia_battaglia_navale tabG2;//8x8
        const SDL_Color coloreG1{ 255, 0, 0, 255 };
        const SDL_Color coloreG2{ 0, 255, 255, 255 };
        const SDL_Color coloreTitolo{ 255, 255, 0, 255};
        int h_titolo {120};

        static int spessore_linee_griglie;

        void centra_griglie(); //Posiziona le griglie una al centro della zona blu e l'altra al centro della zona rossa

        void elimina_ultima_nave_inserita_G1() noexcept;
        void elimina_ultima_nave_inserita_G2() noexcept;

    protected:
    public://Nella vesrione definitiva del programma questo specificatore dovrà essere rimosso e le due funzioni successive dovranno essere mantenute protected
        void genera_flotta_pc();
        void genera_flotta_utente();

    public:
        class Quit_game_exception : std::exception
        {
            const std::string s;
        public:
            Quit_game_exception();
            Quit_game_exception(const std::string& s);
            virtual const char* what(void) const noexcept;
        };


        bool colpisci_cella_tab1(Indice_cella) throw(Casella_gia_colpita_exception,std::invalid_argument);//Restituisce true se va a segno
        bool colpisci_cella_tab2(Indice_cella) throw(Casella_gia_colpita_exception,std::invalid_argument);//...

        Configurazione();
        ~Configurazione();

        void presenta_solo_sfondo();
        void presenta();
        int altezza_striscia_titolo() const;

        Esito stato();//Come siamo messi?

        enum class Click : char; //Dove è avvenuto il click?

        int spessore_linee(void) const { return spessore_linee_griglie; }
        void set_spessore_linee(int i);

    ///<<<TEST<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        void DEBUG_messaggio(){
            //SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
            //SDL_RenderClear( renderer );
            Riquadro a_caso("Il film parte dal momento in cui Selene è sotto attacco sia del brutale clan dei licantropi che del gruppo di vampiri che l'hanno tradita in passato. Ad aiutarla c'è solo David e il padre Thomas. Insieme a loro dovrà porre fine all'eterno conflitto tra i due gruppi.");
            a_caso.set_numero_linee_testo(5);
            a_caso.set_dim(2*200, 2*120);
            disegnaRiquadro( fonts, renderer, &a_caso );
            SDL_RenderPresent( renderer );
            SDL_Delay(5000);
        }
    ///<<<TEST<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        int righe_tabelle() const noexcept { return tabG1.get_righe(); }
        int colonne_tabelle() const noexcept { return tabG1.get_colonne(); }

        void intecetta_click_G1_e_colpisci() throw(Casella_gia_colpita_exception,std::invalid_argument);

    protected:
        //std::pair<Click, Posizione> intercetta_click(SDL_Event& e);

};

#endif // CONFIGURAZIONE_H
