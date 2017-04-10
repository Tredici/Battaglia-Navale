#ifndef SDL_TESTI_MULTILIEA_H_INCLUDED
#define SDL_TESTI_MULTILIEA_H_INCLUDED

#include <string>
#include <cstring>
#include <stdexcept>

#include <SDL.h>
#include <SDL_ttf.h>

#include "sdl_varie.h"
#include "manipolazione_testi.h"


SDL_Surface* crea_SDL_Surface_trasparente(int width,int height,int depth) throw(std::runtime_error);


SDL_Surface* get_multiline_text_surface(TTF_Font* fonts, const std::string& testo, SDL_Color colore_testo, unsigned int num_righe);





#endif // SDL_TESTI_MULTILIEA_H_INCLUDED
