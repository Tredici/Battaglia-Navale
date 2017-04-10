#ifndef MANIPOLAZIONE_TESTI_H_INCLUDED
#define MANIPOLAZIONE_TESTI_H_INCLUDED

#include <cstring>
#include <string>
#include <vector>

std::vector<std::string> suddividi_testo(const std::string& testo, unsigned int lunghezza_linea) noexcept;
std::vector<std::string> suddividi_testo(const std::string& testo, unsigned int lunghezza_linea, unsigned int tolleranza) noexcept;


#endif // MANIPOLAZIONE_TESTI_H_INCLUDED
