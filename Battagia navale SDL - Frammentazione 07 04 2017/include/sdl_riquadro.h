#ifndef SDL_RIQUADRO_H
#define SDL_RIQUADRO_H

#include <string>
#include <stdexcept>
#include <cmath>

#include <SDL.h>
#include <SDL_ttf.h>

#include "sdl_varie.h"
#include "sdl_testi_multiliea.h"


class Riquadro
{///Continua...
private:
    std::string txt{"Testo di esempio"};
    SDL_Texture* texture {nullptr};//Qui si salverà usando i TTF la texture contenente il testo
    SDL_Color colore_testo {0, 0, 0, 255};//Colore del testo: default nero
    bool changed {};
    float rapporto_altezza_larghezza_immagine {};
    SDL_Surface* get_text_surface(TTF_Font* fonts);
    SDL_Color colore_sfondo {255, 255, 255, 255};//Colore di sfondo: default bianco
    float frazione_testo {1};//Intervallo valido [1;0)

    unsigned short int numero_linee_testo {1};//Valore di default
protected:
    SDL_Rect rect {0, 0, 350, 50};
public:
    Riquadro() =default;
    Riquadro(const std::string&);
    Riquadro(const std::string&, SDL_Rect);
    Riquadro(std::string&&);
    Riquadro(std::string&&, SDL_Rect);

    const std::string& testo(void) const noexcept;
    const SDL_Rect& rettangolo_sfondo(void) const noexcept { return rect; }
    SDL_Rect get_riq() const noexcept { return rect; } //Permette di accedere al rettangolo senza però poterlo modificare

    SDL_Rect rettangolo_testo(void) const noexcept;//E' il rettangolo dove viene incollata la texture del testo
    SDL_Texture* get_Texture(TTF_Font* fonts, SDL_Renderer* r);

    float rapporto_h_w() const noexcept { return rapporto_altezza_larghezza_immagine; }
    std::pair<int,int> dimensioni_sfondo_hw() const noexcept { return { rect.h, rect.w }; }
    SDL_Color ottieni_colore_sfondo() const noexcept { return colore_sfondo; }  //Colore di sfondo: default bianco
    SDL_Color ottieni_colore_testo() const noexcept { return colore_testo; }

    void set_pos(int x, int y) noexcept { rect.x = x; rect.y = y; }
    void set_dim(int w, int h) noexcept { rect.w = w; rect.h = h; changed = true; }
    void set_riq(const SDL_Rect& r ) noexcept { rect = r; changed = true; }

    void set_centro(SDL_Point p) noexcept { set_centro_SDL_Rect( &rect, p ); }
    void set_centro(int x, int y) noexcept { set_centro( {x, y} ); }

    void set_colore_testo(SDL_Color) noexcept;
    virtual void set_colore_sfondo(SDL_Color) noexcept;

    void set_testo(const std::string& new_tex) noexcept;
    void set_testo(std::string&& new_tex) noexcept;//Usa il potere dello spostamento

    void set_frazione_testo(float f) noexcept { frazione_testo = (f<=1 && f>0)? sqrt(f):1; }
    float get_frazione_testo() const noexcept { return frazione_testo*frazione_testo; }

    void set_numero_linee_testo(unsigned short int n) noexcept { numero_linee_testo = n; changed = true; }
    unsigned short int get_numero_linee_testo() const noexcept { return numero_linee_testo; }

    void destroy_Texture() noexcept;
};

void disegnaRiquadro(TTF_Font* fonts, SDL_Renderer* ren, Riquadro* riq);

void allinea_orizzontalmente(const Riquadro* a, Riquadro* b);
void allinea_verticalmente(const Riquadro* a, Riquadro* b);

void impila(const Riquadro* a, Riquadro* b);//Uno sotto l'altro con allineamento verticale
void affianca(const Riquadro* a, Riquadro* b);

#endif // SDL_RIQUADRO_H
