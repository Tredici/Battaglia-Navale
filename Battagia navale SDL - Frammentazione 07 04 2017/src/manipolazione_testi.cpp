#include "..\include\manipolazione_testi.h"

//char * strncpy ( char * destination, const char * source, size_t num );
std::vector<std::string> suddividi_testo(const std::string& testo, unsigned int lunghezza_linea) noexcept
{///<Continua...
    const int tolleranza = lunghezza_linea/5 > 7 ? 7 : lunghezza_linea/5 ;
    suddividi_testo( testo, lunghezza_linea, tolleranza );

    return suddividi_testo(testo, lunghezza_linea, tolleranza);
}

std::vector<std::string> suddividi_testo(const std::string& testo, unsigned int lunghezza_linea, unsigned int tolleranza) noexcept
{
    if(lunghezza_linea == 0)
        lunghezza_linea = 80u;//Dimensione standard di una finestra su windows
    if( tolleranza > lunghezza_linea)
        tolleranza = lunghezza_linea;
    std::vector<std::string> linee;
    const size_t dimensione_stringa = testo.size();
    if( !dimensione_stringa )
        return std::vector<std::string>();//Restituisce vettore vuoto
    const char* str = testo.c_str();
    const char* const fine_str = str + dimensione_stringa;

    char txt[lunghezza_linea+2] {};

    auto punto_a_capo = [](const char* txt, int length)
                            {
                                const char * const inizio = txt;
                                const char * const fine = txt+length;
                                while(txt != fine)
                                {
                                    if(*txt == '\n')
                                        return int(txt - inizio);
                                    ++txt;
                                }
                                return int(txt - inizio);
                            };

    while(str < fine_str)
    {
        int posizione = lunghezza_linea;
        int a_capo {};
        if(str+lunghezza_linea > fine_str)
        {
            linee.push_back( std::string(str) );
        }
        else /**/if( (a_capo = punto_a_capo(str, posizione) ) !=  posizione )
        {
            strncpy(txt, str, a_capo);
            posizione = a_capo;
            txt[posizione] = '\0';
            linee.push_back( std::string{ txt } );
        }
        else/***/
        {
            strncpy(txt, str, lunghezza_linea);
            auto tmp_posizione = posizione;
            if( !(txt[tmp_posizione] == ' ' || txt[tmp_posizione] == '\n') )
                for(unsigned int i {}; i<tolleranza; ++i)
                {
                    --tmp_posizione;
                    if(txt[tmp_posizione] == ' ' )
                    {
                        posizione = tmp_posizione;
                        break;
                    }
                }
            txt[posizione] = '\0';
            linee.push_back( std::string{ txt } );
        }
        str += posizione;
        while(str[0] == ' ' || str[0] == '\n')
            ++str;
    }

    return linee;
}
