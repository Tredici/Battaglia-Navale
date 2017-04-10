#ifndef SDL_VARIE_H_INCLUDED
#define SDL_VARIE_H_INCLUDED

#include <string.h>

#include <SDL.h>

void reduce(SDL_Rect*) noexcept;//Prende un SDL_Rect e fa ottenere il massimo SDL_Rect inscivibile in quello di partenza a partire da quello dato
void reduce(SDL_Rect*, int times) noexcept;


bool rettangolo_vuoto(const SDL_Rect* rect);//Controlla se il rettangolo ha area 0
bool rettangolo_puntiforme(const SDL_Rect* rect);//Controlla se invece è proprio ridotto a un punto
bool rettangolo_impossibile(const SDL_Rect* rect);

//Coppia di funzioni concepite per essere più comode di quelle di default di SDL
void SDL_SetRenderDrawColor(SDL_Renderer* r, SDL_Color c);
SDL_Color SDL_GetRenderDrawColor(SDL_Renderer* r, SDL_Color* c = {nullptr} );


bool interno(SDL_Rect*, int, int);
bool interno(SDL_Rect*, SDL_Point*);


SDL_Point get_centro_SDL_Rect(const SDL_Rect*) noexcept;//Ottiene il centro di un SDL_Rect
void set_centro_SDL_Rect(SDL_Rect*, SDL_Point) noexcept;//Fa si ,che il rettangolo abbia un determinato centro
void centra_SDL_Rects(const SDL_Rect*, SDL_Rect*) noexcept;//Il secondo SDL_Rect ootiene lo stesso centro del primo

//Permettono di lavorare esclusivamente sulla trasparenza del colore associato al renderer senza doversi preoccupare di altro
Uint8 get_trasparenza_colore_renderer(SDL_Renderer* r);
void set_trasparenza_colore_renderer(SDL_Renderer* r, Uint8 trasparenza);
void set_trasparenza_default_colore_renderer(SDL_Renderer* r);//Pone semplicemente la trasparenza del colore a 255

void trasla_rettangolo(SDL_Rect* rect, int delta_x, int delta_y);//Usa il potere del polimosrfismo per variare x e y di un SDL_Rect o di un qualcosa che ci somiglia

Uint32 integer_from_SDL_Color(SDL_Color);
SDL_Color SDL_Color_from_integer(Uint32);

void allinea_orizzontalmente(const SDL_Rect* a, SDL_Rect* b);
void allinea_verticalmente(const SDL_Rect* a, SDL_Rect* b);

void impila(const SDL_Rect* a, SDL_Rect* b);
void affianca(const SDL_Rect* a, SDL_Rect* b);

#endif // SDL_VARIE_H_INCLUDED
