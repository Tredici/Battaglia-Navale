#include "../include/sdl_varie.h"

void reduce(SDL_Rect* r) noexcept//Proviamo a vedere se specificarlo per una volta può essere utile
{
    if( (r->h)*(r->h) == 1 || r->h == 0) //L'altezza del rettangolo è 1 o -1 (o 0)?
        r->h = 0;
    else if( r->h > 1 ) //E' più alto di 1?
        r->h -= 2;
    else    //L'altezza è minore di -1?
        r->h += 2;

    if( (r->w)*(r->w) == 1 || r->w == 0)//Stessa cosa ma per quanto riguarda la larghezza
        r->w = 0;
    else if( r->w > 1 )
        r->w -= 2;
    else
        r->w += 2;

    //Se la dimensione associata è positiva allora incrementa la coordinata, se è negativa la riduca, se è nulla non compie variazioni
    r->x += (r->w > 0)? 1 : ( r->w != 0 ? -1 : 0 );
    r->y += (r->h > 0)? 1 : ( r->h != 0 ? -1 : 0 );
}

void reduce(SDL_Rect* rect, int times) noexcept
{
    for(int i {}; i<times; ++i)
        reduce( rect );
}


void SDL_SetRenderDrawColor(SDL_Renderer* r, SDL_Color c)
{
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
};

SDL_Color SDL_GetRenderDrawColor(SDL_Renderer* r, SDL_Color* c)
{
    SDL_Color tmp;
    SDL_GetRenderDrawColor(r, &tmp.r, &tmp.g, &tmp.b, &tmp.a);
    if( c )
        *c = tmp;
    return tmp;
};

bool interno(SDL_Rect* r, int x, int y)
{
    return r->x <= x && r->x + r->w >= x && //Il punto si trova tra i due lati del rettangolo
            r->y <= y && r->y + r->h >= y;  //      e si trova tra le due basi del rettangolo
}
bool interno(SDL_Rect* r, SDL_Point* p)
{
    return interno(r, p->x, p->y);
}

SDL_Point get_centro_SDL_Rect(const SDL_Rect* rect) noexcept
{
    return SDL_Point{ rect->x + rect->w/2, rect->y + rect->h/2 };
}
void set_centro_SDL_Rect(SDL_Rect* rect, SDL_Point p) noexcept
{
    rect->x = p.x - rect->w/2;
    rect->y = p.y - rect->h/2;
}
void centra_SDL_Rects(const SDL_Rect* a, SDL_Rect* b) noexcept
{
    set_centro_SDL_Rect( b, get_centro_SDL_Rect(a) );//Efficienza e semplicità
}

Uint8 get_trasparenza_colore_renderer(SDL_Renderer* r)
{
    SDL_Color c;
    SDL_GetRenderDrawColor(r, &c);
    return c.a;
}
void set_trasparenza_colore_renderer(SDL_Renderer* r, Uint8 trasparenza)
{
    SDL_Color col;
    SDL_GetRenderDrawColor(r, &col);
    col.a = trasparenza;
    SDL_SetRenderDrawColor(r, col);//Imposta il medesimo colore per il renderer solo cambiandone la trasparenza
}
void set_trasparenza_default_colore_renderer(SDL_Renderer* r)
{
    set_trasparenza_colore_renderer(r, 255);
}

void trasla_rettangolo(SDL_Rect* rect, int delta_x, int delta_y)
{
    rect->x += delta_x;
    rect->y += delta_y;
}

bool rettangolo_vuoto(const SDL_Rect* rect)
{
    return rect->w == 0 || rect->h == 0;
}

bool rettangolo_puntiforme(const SDL_Rect* rect)
{
    return rect->w == 0 && rect->h == 0;
}


Uint32 integer_from_SDL_Color(SDL_Color c)
{
    Uint32 i;
    memcpy(&i, &c, 4);
    return i;
}
SDL_Color SDL_Color_from_integer(Uint32 i)
{
    SDL_Color c;
    memcpy(&c, &i, 4);
    return c;
}


void allinea_orizzontalmente(const SDL_Rect* a, SDL_Rect* b)
{
    SDL_Point punto = get_centro_SDL_Rect(a);
    punto.x = get_centro_SDL_Rect(b).x;
    set_centro_SDL_Rect(b, punto);
}

void allinea_verticalmente(const SDL_Rect* a, SDL_Rect* b)
{
    SDL_Point punto = get_centro_SDL_Rect(a);
    punto.y = get_centro_SDL_Rect(b).y;
    set_centro_SDL_Rect(b, punto);
}

void impila(const SDL_Rect* a, SDL_Rect* b)
{
    SDL_Point punto = get_centro_SDL_Rect(a);
    punto.y += (a->h + b->h)/2 + (a->h + b->h)%2;//Se la somma è dispari evita la sovrapposizione di un pixel
    set_centro_SDL_Rect(b, punto);
}

void affianca(const SDL_Rect* a, SDL_Rect* b)
{
    SDL_Point punto = get_centro_SDL_Rect(a);
    punto.x += (a->w + b->w)/2 + (a->w + b->w)%2;//Se la somma è dispari evita la sovrapposizione di un pixel
    set_centro_SDL_Rect(b, punto);
}


