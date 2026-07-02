// Texto.h

#pragma once

#include <string>
#include <vector>

namespace streaming::texto {

// Convierte a minusculas 
std::string aMinusculas(std::string valor);

// Elimina caracteres que no sean letras/numeros
std::string normalizar(std::string valor);

// Separa una cadena ya normalizada en palabras 
std::vector<std::string> tokenizar(const std::string& valor);

// Separa un campo tipo "Actor Uno, Actor Dos" en tokens 
std::vector<std::string> tokenizarCampoMultivaluado(const std::string& valor);

} 
