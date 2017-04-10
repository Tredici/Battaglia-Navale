#include "sdl_rettangolo_bordato.h"

Rettangolo_bordato::Rettangolo_bordato() noexcept
: b{1}
{
    SDL_Rect::x = {};
    SDL_Rect::y = {};
    SDL_Rect::w = {};
    SDL_Rect::h = {};
}

Rettangolo_bordato::Rettangolo_bordato(int x,int y,int w,int h, int b) noexcept
: b{b}
{
    if(b<1)
        throw std::invalid_argument{ std::string{ "Rettangolo_bordato(...)->bordo rettangolo min. di 1" } };
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

void Rettangolo_bordato::set_bordo(int b)
{
    if(b<1)
        throw std::invalid_argument{ std::string{ "Rettangolo_bordato(...)->bordo rettangolo min. di 1" } };
    if( b*2 > std::min(w,h) )
        throw std::invalid_argument{ std::string{ "Rettangolo_bordato(...)->bordo maggiore della larghezza del rettangolo" } };
    this->b = b;
}

void SDL_RenderDrawRettangoloBordato(SDL_Renderer* r, const Rettangolo_bordato* b_rect)
{
    if( b_rect->b < 1 )
        throw std::logic_error{ std::string{ "SDL_RenderDrawRettangoloBordato(...)->bordo rettangolo min. di 1" } };
    SDL_Rect rect;
    rect = *b_rect;//Ottiene una copia della classe base
    for(int i {1}; i<b_rect->b; ++i)
    {
        SDL_RenderDrawRect(r, &rect);//Si avvale del potere del polimorfismo
        reduce(&rect);
    }
    SDL_RenderDrawRect(r, &rect);
}

Rettangolo_bordato::operator SDL_Rect()
{
    return SDL_Rect{x, y, w, h};
}

SDL_Rect ottieni_SDL_Rect_interno_da_Rettangolo_bordato(const Rettangolo_bordato& rb) noexcept
{
    return SDL_Rect {rb.x+rb.b, rb.y+rb.b, rb.w-2*rb.b, rb.h-2*rb.b};
}

//Funziona così: disegna il bordo, dopodiché disegna un SDL_Rect pieno al suo interno usando il medesimo colore colore solo cambiando la trasparenza
void SDL_RenderFillRettangoloBordato(SDL_Renderer* r, const Rettangolo_bordato& rb, Uint8 trasparenza)
{
    SDL_Color old;
    SDL_GetRenderDrawColor(r, &old);//Salva il colore del renderer iniziale
    set_trasparenza_default_colore_renderer(r);//Trasparenza a 255
    SDL_RenderDrawRettangoloBordato(r, &rb);//Disegna il bordo opaco
    set_trasparenza_colore_renderer(r, trasparenza);//Imposta la trasparenza dell'interno a quanto si desidera
    SDL_Rect interno = ottieni_SDL_Rect_interno_da_Rettangolo_bordato(rb);//Ottiene il rettangolo interno a quello originario
    SDL_RenderFillRect(r, &interno);//Disegna l'interno
    SDL_SetRenderDrawColor(r, old);//Reimposta lo status quo
}


void SDL_RenderDrawVerticiRettangoloBordato(SDL_Renderer* r, const Rettangolo_bordato* rb) noexcept
{
    SDL_Rect quadrato = *rb;
    quadrato.w = quadrato.h = rb->b;
    SDL_RenderFillRect(r, &quadrato);//Vertice in alto a sinistra
    quadrato.y += rb->h - rb->b;
    SDL_RenderFillRect(r, &quadrato);//Vertice in basso a sinistra
    quadrato.x += rb->w - rb->b;
    SDL_RenderFillRect(r, &quadrato);//Vertice in basso a destra
    quadrato.y -= rb->h - rb->b;
    SDL_RenderFillRect(r, &quadrato);//Vertice in alto a destra
}





