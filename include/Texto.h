// Texto.h
// Funciones de utilidad para procesamiento de texto, agrupadas en un
// namespace (en vez de metodos sueltos repetidos en cada clase, como
// ocurria con "convertirMinusculas" duplicado en main.cpp e Interfaz.cpp).
#pragma once

#include <string>
#include <vector>

namespace streaming::texto {

// Convierte a minusculas (locale-independiente, solo ASCII para simplicidad).
std::string aMinusculas(std::string valor);

// Elimina caracteres que no sean letras/numeros, dejando espacios como
// separadores (referencias [1], puntuacion, etc. se descartan).
std::string normalizar(std::string valor);

// Separa una cadena ya normalizada en palabras (separador: espacio).
std::vector<std::string> tokenizar(const std::string& valor);

// Separa un campo tipo "Actor Uno, Actor Dos" en tokens normalizados,
// tratando la coma como separador adicional. Util para cast/genero/director.
std::vector<std::string> tokenizarCampoMultivaluado(const std::string& valor);

} // namespace streaming::texto
