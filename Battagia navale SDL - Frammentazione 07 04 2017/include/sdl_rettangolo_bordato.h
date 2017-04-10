#ifndef SDL_RETTANGOLO_BORDATO_H
#define SDL_RETTANGOLO_BORDATO_H

#include <string>
#include <stdexcept>

#include <SDL.h>

#include "sdl_varie.h"

//Grazie al polimorfismo un puntatore a un rettangolo bordato può essere usato allo stesso modo di un SDL_Rect*
struct Rettangolo_bordato : public SDL_Rect
{
    int b;//Bordo
    Rettangolo_bordato() noexcept;
    Rettangolo_bordato(int x,int y ={},int w ={},int h ={}, int b ={1}) noexcept;
    void set_bordo(int b); //Permette di impostare controllandolo il valore del bordo
    explicit operator SDL_Rect();//Sono ammesse solo conversioni esplicite
};

void SDL_RenderDrawRettangoloBordato(SDL_Renderer*, const Rettangolo_bordato*);//Disegna il rettangolo bordato disegnandone solamente il bordo

//Disegna i vertici di un rettangolo bordato rappresentandoli come dei quadratini di lato pari allo spessore del rettangolo
void SDL_RenderDrawVerticiRettangoloBordato(SDL_Renderer*, const Rettangolo_bordato*) noexcept;

SDL_Rect ottieni_SDL_Rect_interno_da_Rettangolo_bordato(const Rettangolo_bordato& rb) noexcept;//Fa ottenere un SDL_Rect perfettamente inseribile dentro il Rettangolo_bordato di partenza

//Ricorda che la trasparenza a 0 indica che l'oggetto è invisibile, a 255 invece significa perfettamente opaco
void SDL_RenderFillRettangoloBordato(SDL_Renderer* r, const Rettangolo_bordato& rb, Uint8 trasparenza);



#endif // SDL_RETTANGOLO_BORDATO_H
