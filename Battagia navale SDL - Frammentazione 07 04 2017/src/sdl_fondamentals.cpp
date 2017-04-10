#include "..\include\sdl_fondamentals.h"



const std::string SDL_Fondamentals::font_file { "Fonts.ttf" };

SDL_Fondamentals::SDL_Fondamentals()
{//
    if( SDL_Init( SDL_INIT_VIDEO ) <0 )
        throw std::runtime_error{ std::string{ "runtime err.: SDL_Foundamentals()->Fallita inizializzazione di SDL. " } + std::string{ SDL_GetError() } };
    if( !( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG) )
    {
        SDL_Quit();
        throw std::runtime_error{ std::string{ "runtime err.: SDL_Foundamentals()->Fallita inizializzazione di IMG. " } + std::string{ IMG_GetError() } };
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    if( TTF_Init() < 0 )
    {
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error{ std::string{ "runtime err.: SDL_Foundamentals()->Fallita inizializzazione di TTF. " } + std::string{ TTF_GetError() } };
    }
}

SDL_Fondamentals::~SDL_Fondamentals()
{//Termina l'esecuzione dei subsystems di SDL
    destroy_renderer();
    destroy_window();
    delete_fonts();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SDL_Fondamentals::set_rendererFlags(SDL_RendererFlags f)
{
    this->rendererFlags = f;
}

void SDL_Fondamentals::set_Window_h(int h)
{
    if(h<dimensione_minima)
        throw std::invalid_argument( std::string{"set_Window_h(...)->valore troppo piccolo per la finestra"} );
    altezza_finestra = h;
}

void SDL_Fondamentals::set_Window_w(int w)
{
    if(w<dimensione_minima)
        throw std::invalid_argument( std::string{"set_Window_w(...)->valore troppo piccolo per la finestra"} );
    larghezza_finestra = w;
}

void SDL_Fondamentals::set_Window_wh(int w, int h)
{
    set_Window_w(w);
    set_Window_h(h);
}

void SDL_Fondamentals::set_Window_name(const std::string& name)
{
    this->nome_finestra = name;
}


SDL_Window* SDL_Fondamentals::get_Window()
{
    if(!window)//Se la finestra già esiste è in'utile crearne una nuova
        window = SDL_CreateWindow(nome_finestra.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, larghezza_finestra, altezza_finestra, SDL_WINDOW_SHOWN);
    if(!window)
        throw std::runtime_error{ std::string{"get_Window()->Fallita creazione della finestra"} + std::string{SDL_GetError()} };
    return window;
}

SDL_Renderer* SDL_Fondamentals::get_Renderer()
{
    if(!window)
        throw std::logic_error{ std::string{ "get_Renderer()->Tentativo di creare un renderer senza possedere una finestra." } };
    if(!renderer)
        renderer = SDL_CreateRenderer(window, -1, rendererFlags);
    if(!renderer)
        throw std::runtime_error{ std::string{"get_Renderer()->Fallita creazione del renderer"} + std::string{SDL_GetError()} };
    SDL_SetRenderDrawBlendMode(renderer, blend_mode);//Imposta la blend mode
    return renderer;
}

void SDL_Fondamentals::destroy_renderer()
{
    if(renderer)
        SDL_DestroyRenderer(renderer);
    renderer = nullptr;
}

void SDL_Fondamentals::destroy_window()
{
    if(window)
        SDL_DestroyWindow(window);
    window = nullptr;
}

void SDL_Fondamentals::delete_fonts()
{
    if(fontSet)
        TTF_CloseFont(fontSet);
    fontSet = nullptr;
}

//Versioni speciali che distruggono un particolare set passato come argomento e lo mettono a nullptr e, se questo risulta uguale a quello posseduto da
//Le tre funzioni chiedono un riferimento a un puntatore in modo da poter impostare la variabile a nullptr se necessario
void SDL_Fondamentals::destroy_renderer(SDL_Renderer*& r)
{
    if(r == renderer)
    {
        destroy_renderer();
    }
    else
    {
        SDL_DestroyRenderer(r);
    }
    r = nullptr;
}
void SDL_Fondamentals::destroy_window(SDL_Window*& w)
{
    if(w == window)
    {
        destroy_window();
    }
    else
    {
        SDL_DestroyWindow(w);
    }
    w = nullptr;
}
void SDL_Fondamentals::delete_fonts(TTF_Font*& f)
{
    if(f == fontSet)
    {
        delete_fonts();
    }
    else
    {
        TTF_CloseFont(f);
    }
    f = nullptr;
}


int SDL_Fondamentals::get_Window_h(void) const
{
    return altezza_finestra;
}

int SDL_Fondamentals::get_Window_w(void) const
{
    return larghezza_finestra;
}

TTF_Font* SDL_Fondamentals::get_FontSet()
{
    TTF_Font* fonts = TTF_OpenFont( font_file.c_str(), std_fontSize );//Usa un temporaneo per questioni di comodità
    if(!fonts)
        throw std::runtime_error{ std::string{ "SDL_Fondamentals::get_FontSet()->Failed loading file " } + font_file + std::string{"\t"} + std::string{ TTF_GetError() } };
    return fontSet = fonts;
}



















Linea::Linea(int x1,int y1,int x2,int y2)
: x1{x1}, y1{y1}, x2{x2}, y2{y2}
{}

void SDL_RenderDrawLine(SDL_Renderer* r, Linea l)
{
    SDL_RenderDrawLine(r, l.x1, l.y1, l.x2, l.y2);
}

void Linea::varia_x(int d_x)
{
    x1 += d_x;
    x2 += d_x;
}
void Linea::varia_y(int d_y)
{
    y1 += d_y;
    y2 += d_y;
}

Linea_spessa::Linea_spessa(int x1,int y1,int x2,int y2, int b)
: Linea{x1,y1,x2,y2}, s{b}
{
    if(s<1)
        throw std::invalid_argument{ std::string{ "Linea_spessa(...)->spessore linea min. di 1" } };
}


void SDL_RenderDrawLine(SDL_Renderer* r, const Linea_spessa& ls)
{
    if( !ls.ortogonale() )
        throw std::domain_error{ std::string{ "SDL_RenderDrawLine(...)->Funzione non ancora implememnatata per la gestionee di linee oblique" } };
    SDL_Rect rect = make_SDL_Rect_from_Linea_spessa( ls );
    SDL_RenderFillRect(r, &rect);
}

SDL_Rect make_SDL_Rect_from_Linea_spessa(const Linea_spessa& ls)
{
    if( !ls.ortogonale() )
        throw std::invalid_argument{ std::string{"make_SDL_Rect_from_Linea_spessa(...)->tentativo di creare un SDL_Rect da una Linea_spessa non -|"} };
    SDL_Rect rect {}; //Mettiamo tutto a 0 perché è bello fare cose a caso
    if( ls.orizzontale() )//ls.y1 == ls.y2
    {
        rect.x = std::min(ls.x1, ls.x2);
        rect.w = abs( ls.x2 - ls.x1 );
        rect.y = ls.y1 - ls.s/2;
        rect.h = ls.s;
    }
    else//ls.x1 == ls.x2
    {
        rect.y = std::min(ls.y1, ls.y2);
        rect.h = abs( ls.y2 - ls.y1 );
        rect.x = ls.x1 - ls.s/2;
        rect.w = ls.s;
    }
    return rect;
}

bool Linea_spessa::ortogonale() const
{
    return x1 == x2 || y1 == y2;
}

bool Linea_spessa::verticale() const
{
    return x1 == x2;
}

bool Linea_spessa::orizzontale() const
{
    return y1 == y2;
}

double Linea::lunghezza() const
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}


Linea_spessa& Linea_spessa::operator=(const Linea& l)
{
    this->x1 = l.x1;
    this->x2 = l.x2;
    this->y1 = l.y1;
    this->y2 = l.y2;
    this->s = 1;
    return *this;
}

Linea_spessa& Linea_spessa::operator=(Linea&& l)
{
    this->x1 = l.x1;
    this->x2 = l.x2;
    this->y1 = l.y1;
    this->y2 = l.y2;
    this->s = 1;
    return *this;
}


Linea_spessa& Linea_spessa::imposta_spessore(int s)
{
    if(s < 1)
        throw std::invalid_argument{ std::string{ "Linea_spessa::imposta_spessore(...)->Spessore minore di 1" } };
    this->s = s;
    return *this;
}

Linea_spessa::Linea_spessa(const Linea& l)
: s{1}
{
    this->x1 = l.x1;
    this->x2 = l.x2;
    this->y1 = l.y1;
    this->y2 = l.y2;
}












