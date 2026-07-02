// Texto.cpp
#include "Texto.h"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace streaming::texto {

std::string aMinusculas(std::string valor) {
    std::transform(valor.begin(), valor.end(), valor.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    return valor;
}

std::string normalizar(std::string valor) {
    valor = aMinusculas(std::move(valor));

    // Reemplazamos cualquier caracter no alfanumerico por un espacio, para que la tokenizacion sea mas simple.
    std::transform(valor.begin(), valor.end(), valor.begin(), [](unsigned char c) {
        return std::isalnum(c) ? static_cast<char>(c) : ' ';
    });
    return valor;
}

std::vector<std::string> tokenizar(const std::string& valor) {
    std::vector<std::string> palabras;
    std::istringstream flujo(valor);
    std::string palabra;
    while (flujo >> palabra) {
        palabras.push_back(palabra);
    }
    return palabras;
}

std::vector<std::string> tokenizarCampoMultivaluado(const std::string& valor) {
    return tokenizar(normalizar(valor));
}

}
