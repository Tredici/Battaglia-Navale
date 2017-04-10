#include "../include/sdl_testi_multiliea.h"


SDL_Surface* crea_SDL_Surface_trasparente(int width,int height) throw(std::runtime_error)
{
    SDL_Surface *surface = nullptr;

    Uint32 rmask {}, gmask {}, bmask {}, amask {};

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Color resulting_color {};
    Uint32 origine = rmask | gmask | bmask | amask ;
    memcpy(&resulting_color, &origine, sizeof(Uint32) );

    surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);//Crea la superficie

    if (surface == nullptr)
        throw std::runtime_error{ std::string{ "crea_SDL_Surface_trasparente(...)->SDL_CreateRGBSurface() failed: " } + std::string{ SDL_GetError() } };

    if ( SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, resulting_color.r, resulting_color.g, resulting_color.b, resulting_color.a) ) < 0 )//La rende trasparente
    {
        SDL_FreeSurface( surface );
        throw std::runtime_error{ std::string{ "crea_SDL_Surface_trasparente(...)->SDL_SetColorKey() failed: " } + std::string{ SDL_GetError() } };
    }
    return surface;//La resituisce
}



SDL_Surface* get_multiline_text_surface(TTF_Font* fonts, const std::string& testo, SDL_Color colore_testo, unsigned int num_righe)
{
    //La suddivisione delle linee si basa sulla loro lunghezza non sul loro numero
    int lunghezza_linea = 1 + testo.size()/(num_righe);///<(num_righe==1?0:-1));//Quei "+1" sono lì per approssimazioni messe a sentimento
        //Suddivide il testo nel 'corretto numero di linee'
    auto linee = suddividi_testo(testo, lunghezza_linea, lunghezza_linea * 0.3);//Anche quel 0.3 è lì per ragioni sentimentali
    //L'altezza è sempre quella...
    const int altezza_carattere { TTF_FontHeight( fonts ) +2 };//Quel due è per il pixel da mettere sopra e sotto ofni riga
    const int altezza_immagine = altezza_carattere * linee.size();//Giustamente
    int larghezza_immagine {};
    for(const auto& x : linee)
    {
        int tmp {};
        TTF_SizeText(fonts, x.c_str(), &tmp, nullptr);
        if( tmp > larghezza_immagine )
            larghezza_immagine = tmp;
    }
    //Adesso abbiamo tutte le informazioni che servono per realizzare l'immagine

    //Immagine di sfondo
    SDL_Surface* immagine { crea_SDL_Surface_trasparente(larghezza_immagine, altezza_immagine) };
    if( !immagine )
        throw std::runtime_error{ std::string{ "get_multiline_text_surface(...)->fallita crea_SDL_Surface_trasparente(...)\t" } + std::string{ SDL_GetError() } };
    size_t i {};
    SDL_Rect destinazione {};
    destinazione.h = altezza_carattere;
    for(const auto& x : linee)
    {
        destinazione.y = i * altezza_carattere;
        SDL_Surface* linea = TTF_RenderText_Blended(fonts, x.c_str(), colore_testo);//Crea l'immagine della prima linea di testo

        if( !linea )
            throw std::runtime_error{ std::string{ "get_multiline_text_surface(...)->fallita TTF_RenderText_Blended(...)\t" } + std::string{ TTF_GetError() } };
        destinazione.w = linea->w;
        //Incolla la linea di testo creato sull'immagine di sfondo
        SDL_BlitSurface(linea, nullptr, immagine, &destinazione);
        SDL_FreeSurface(linea);//Distrugge la linea
        ++i;
    }

    return immagine;
}
