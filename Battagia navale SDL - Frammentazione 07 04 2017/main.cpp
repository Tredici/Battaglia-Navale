#include <iostream>
#include "include\gioco.h"

#include "include\manipolazione_testi.h"

using namespace std;

int main(int argv, char* argc[])
try
{
    /*
    std::string testo{ "The quick brown fox jumps over the lazy dog" };

    auto righe = suddividi_testo(testo, 8, 2);

    for(const auto& x : righe)
        std::cout << x << std::endl;
    */
    Gioco gioco;
    gioco.gioca();
    //gioco.DEBUG_messaggio();

    return 0;
}
catch( Configurazione::Quit_game_exception )
{
    std::cerr << "Addio mondo crudele!" << std::endl;
    return -1;
}
