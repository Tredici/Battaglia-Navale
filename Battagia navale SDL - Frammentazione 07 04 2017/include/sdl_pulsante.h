#ifndef SDL_PULSANTE_H
#define SDL_PULSANTE_H

#include "sdl_riquadro.h"


template <typename func>
class Pulsante : public Riquadro
{///Continua...
    func f;
    bool pulsante_on_off {false};
    bool cambia_colore {};
    SDL_Color colore_sfondo_1;
    SDL_Color colore_sfondo_2;
    bool colore_1 = true;//Quale colore dobbiamo usare?

    void imposta_colore(bool change)
    {
        if( pulsante_on_off && cambia_colore )
        {
            if( change )
                colore_1 = !colore_1;
            if( colore_1 )
                Riquadro::set_colore_sfondo( colore_sfondo_1 );
            else
                Riquadro::set_colore_sfondo( colore_sfondo_2 );
        }
    }
public:
    Pulsante(func F)
        : f{F}, colore_sfondo_1{ Riquadro::ottieni_colore_sfondo() }, colore_sfondo_2{ Riquadro::ottieni_colore_sfondo() }
        {}
    void operator()()
    {
        imposta_colore( true );
        f();
    }

    void if_opera(int x, int y)//Se v'è stato un click opera
    {
        if( clickato(x,y) )
            operator()();
    }

    bool clickato(int x, int y) noexcept//Il clich è avvenuto sul Pulsante
    {
        return interno(&rect, x, y);
    }

    bool on_off(bool opz) noexcept { return cambia_colore = pulsante_on_off = opz; }
    Pulsante& colori(SDL_Color col_1, SDL_Color col_2);

    virtual void set_colore_sfondo(SDL_Color c) noexcept override
    {
        Riquadro::set_colore_sfondo(c);//Innanzitutto il colore viene impostato per lo sfondo
        if(colore_1)//Poi viene anche impostato come colore_sfondo 1 o 2 a seconda di quale dovrebbe essere attivo attualmente
            set_colore_sfondo_1(&c);
        else
            set_colore_sfondo_2(&c);
        imposta_colore(false);
    }

    SDL_Color ottieni_colore_sfondo_1() const noexcept { return colore_sfondo_1; }
    SDL_Color ottieni_colore_sfondo_2() const noexcept { return colore_sfondo_2; }

    void set_colore_sfondo_1(const SDL_Color *colore_opz_1) noexcept
    {
        colore_sfondo_1= *colore_opz_1;
        imposta_colore(false);
    }
    void set_colore_sfondo_2(const SDL_Color *colore_opz_2) noexcept
    {
        colore_sfondo_2= *colore_opz_2;
        imposta_colore(false);
    }

    void set_colori_sfondo_1_2(const SDL_Color *colore_opz_1, const SDL_Color *colore_opz_2) noexcept
    {
        if( colore_opz_1 )
            set_colore_sfondo_1(colore_opz_1);
        if( colore_opz_2 )
            set_colore_sfondo_2(colore_opz_2);

    }
};


template <typename func>
Pulsante<func> crea_pulsante(func f)
{
    return Pulsante<func>( f );
}


#endif // SDL_PULSANTE_H
