#include "sdl_riquadro.h"


Riquadro::Riquadro(const std::string& tex)
: txt{tex}
{}

Riquadro::Riquadro(const std::string& tex, SDL_Rect rec)
: txt{tex}, rect{rec}
{}

Riquadro::Riquadro(std::string&& tex)
: txt{tex}
{}

Riquadro::Riquadro(std::string&& tex, SDL_Rect rec)
: txt{tex}, rect{rec}
{}


SDL_Surface* Riquadro::get_text_surface(TTF_Font* fonts)
{
    SDL_Surface* immagine = get_multiline_text_surface(fonts, txt, colore_testo, numero_linee_testo);
        ///<TTF_RenderText_Blended( fonts, txt.c_str(), colore_testo );
    if( !immagine )
        throw std::runtime_error{ std::string{ "Riquadro::get_text_surface(...)->fallita generazione SDL_Surface da parte di TTF_RenderText_Solid(...)\tcausa erorre " } + std::string{ TTF_GetError() } };
    rapporto_altezza_larghezza_immagine = static_cast<float>( immagine->h )/static_cast<float>( immagine->w );
    return immagine;
}

//Va riveduta in futuro, magari si potrà aggiungere una variabile chiamata "pixel_di_scostamento" nella classe da passare a reduce al posto di quel 5 che sicuramente creerà dei problemi più avanti
SDL_Rect Riquadro::rettangolo_testo(void) const noexcept
{
    std::pair<int,int> rect_hw = dimensioni_sfondo_hw();//Ottiene le dimensioni del rettangolo di sfondo
    //Costruisce il rettangolo destinato a contenere il testo sulla base di quello di sfondo
    float rapporto_hw = rapporto_h_w();//Così si risparmia il tempo di una chiamata a funzione eccessiva
    SDL_Rect rett_testo { 0, 0, static_cast<int>( frazione_testo * (rect_hw.second * rapporto_hw <= rect_hw.first ? rect_hw.second :  static_cast<int>( static_cast<float>( rect_hw.first ) / static_cast<float>( rapporto_hw ) ) ) ), static_cast<int>( frazione_testo * ( rect_hw.first / rapporto_hw <= rect_hw.second ? rect_hw.first : static_cast<int>( static_cast<float>( rect_hw.second ) * static_cast<float>( rapporto_hw ) ) ) ) };
    reduce( &rett_testo, 5);//Da almeno una parvenza di ordine
    centra_SDL_Rects( &rect, &rett_testo );//Fa si che i due rettangoli siano concentrici
    return rett_testo;//Restituisce il rettangolo richiesto al chiamante
}

SDL_Texture* Riquadro::get_Texture(TTF_Font* fonts, SDL_Renderer* r)
{
    if( changed )
    {
        destroy_Texture();
        changed = false;
    }
    if(!texture)//Se non possiede la texture
    {//La crea
        SDL_Surface* facesure = get_text_surface( fonts );
        SDL_Texture* turetext = SDL_CreateTextureFromSurface( r,  facesure );
        SDL_FreeSurface( facesure );
        if( !turetext )
            throw std::runtime_error{ std::string{ "Riquadro::get_Texture(...)->fallita creazione della texture da parte di SDL_CreateTextureFromSurface(...)" } + std::string{ SDL_GetError() } };
        texture = turetext;
    }
    return texture;//La fornisce
}

void Riquadro::destroy_Texture() noexcept
{
    if( !texture ) return;
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void Riquadro::set_colore_testo(SDL_Color c) noexcept
{
    colore_testo = c;
    changed = true;
}

void Riquadro::set_colore_sfondo(SDL_Color c) noexcept
{
    colore_sfondo = c;
}

void Riquadro::set_testo(const std::string& new_tex) noexcept
{
    txt = new_tex;
    changed = true;
}
void Riquadro::set_testo(std::string&& new_tex) noexcept
{
    txt = new_tex;
    changed = true;
}

const std::string& Riquadro::testo(void) const noexcept
{
    return txt;
}




void disegnaRiquadro(TTF_Font* fonts, SDL_Renderer* ren, Riquadro* riq)
/*  Il colore del renderer deve rimannere inalterato*/
{
    SDL_Color rendererColor;
    SDL_GetRenderDrawColor( ren, &rendererColor );//Ricorda lo stato iniziale
    //Disegna lo sfondo
    SDL_Color colore_sfondo = riq->ottieni_colore_sfondo();
    SDL_SetRenderDrawColor( ren, colore_sfondo );
    const SDL_Rect& rettangolo_Riquadro = riq->get_riq();//Ottiene un riferimento const al rettangolo contente il riquadtro
    SDL_RenderFillRect( ren, &rettangolo_Riquadro );//Essendo una funzione friend può accedere ai membri private e protected di un Riqquadro
    //Ci disegna sopra il testo
    SDL_Texture* texture_txt = riq->get_Texture( fonts, ren );//Ottiene la texture contenente il testo del riquadro

    const SDL_Rect& rettangolo_testo = riq->rettangolo_testo();
    SDL_RenderCopy( ren, texture_txt, nullptr, &rettangolo_testo );//Disegna la scritta al posto giusto

    SDL_SetRenderDrawColor(ren, rendererColor);//Reimposta lo stato delle cose
}

void impila(const Riquadro* a, Riquadro* b)
{
    const SDL_Rect r_1 = a->get_riq();
    SDL_Rect r_2 = b->get_riq();
    impila(&r_1, &r_2);
    b->set_pos(r_2.x, r_2.y);
}

void affianca(const Riquadro* a, Riquadro* b)
{
    const SDL_Rect r_1 = a->get_riq();
    SDL_Rect r_2 = b->get_riq();
    affianca(&r_1, &r_2);
    b->set_pos(r_2.x, r_2.y);
}

void allinea_orizzontalmente(const Riquadro* a, Riquadro* b)
{///<Continua...
    ;
}

void allinea_verticalmente(const Riquadro* a, Riquadro* b)
{///<Continua...
    ;
}

