#ifndef SDL_FOUNDAMENTALS_H
#define SDL_FOUNDAMENTALS_H

#include <string>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <utility>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>//Utilizza anche i true type font

#include "sdl_colors.h"

/*
    E' destinata ad essere una base virtuale di gioco.
    Probabilmente finirà così:
    Gioco : public Configuratione
    Configuratione : public virtual SDL_Fondamentals
*/



class SDL_Fondamentals
{
        static constexpr int dimensione_minima {100}; //Niente finestre più piccole di 100*100
        int altezza_finestra {650};
        int larghezza_finestra {1250};
        SDL_RendererFlags rendererFlags = static_cast<SDL_RendererFlags>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        TTF_Font* fontSet = nullptr;
        std::string nome_finestra{"Applicazione."};

        SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND;//alpha blending
    public:
        static constexpr int std_fontSize = 72;//Più grande è meglio è
        static const std::string font_file;//Più avanti si potrebbe anche levare gli specificatori "static" e "const" per rendere possibile usare anche più file di font diversi a seconda del programma
        //si potrebbero inoltre aggiungere delle funzioni (meglio se nelle classi) derivare per scegliere in altri momenti file di font da caricare in momenti diversi
        SDL_Fondamentals();
        ~SDL_Fondamentals();
        void set_Window_h(int h);
        void set_Window_w(int w);
        void set_Window_wh(int w, int h);

        int get_Window_h(void) const;
        int get_Window_w(void) const;

        SDL_Window* get_Window();
        SDL_Renderer* get_Renderer();
        TTF_Font* get_FontSet();

        bool has_Window  () const noexcept { return window; }
        bool has_Renderer() const noexcept { return renderer; }
        bool has_FontSet () const noexcept { return fontSet; }

        void set_rendererFlags(SDL_RendererFlags f);
        void set_Window_name(const std::string& name);

        void destroy_renderer();
        void destroy_window();
        void delete_fonts();

        void destroy_window(SDL_Window*& w);
        void destroy_renderer(SDL_Renderer*& r);
        void delete_fonts(TTF_Font*& f);
};














struct Linea
{
    int x1{}, y1{};
    int x2{}, y2{};
    Linea() =default;
    Linea(int ,int ={},int ={},int ={});
    void varia_x(int d_x);
    void varia_y(int d_y);
    virtual double lunghezza() const;//Magari un giorno qualcuno vorrò la stessa funzione ma che restituisca float o long double
    Linea& operator=(const Linea&) =default;
};

void SDL_RenderDrawLine(SDL_Renderer*, Linea);

struct Linea_spessa : public Linea
{
    int s {1};//Spessore
    Linea_spessa() =default;
    Linea_spessa(const Linea_spessa&) =default;
    Linea_spessa(const Linea& l);

    Linea_spessa(int ,int ={},int ={},int ={}, int ={});
    Linea_spessa& imposta_spessore(int s);
    bool ortogonale() const;//La linea è vericale oppure orizzontale?
    bool verticale() const;
    bool orizzontale() const;
    Linea_spessa& operator=(const Linea& l);//Imposta lo spessore della linea a 1 come di default | lavora con lvalue
    Linea_spessa& operator=(Linea&& l);//Lavore con rvalue
    Linea_spessa& operator=(const Linea_spessa&) =default;
};

void SDL_RenderDrawLine(SDL_Renderer*, const Linea_spessa&);

SDL_Rect make_SDL_Rect_from_Linea_spessa(const Linea_spessa& ls);




#endif // SDL_FOUNDAMENTALS_H
