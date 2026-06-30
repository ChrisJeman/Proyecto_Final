// Comun.h
// Tipos, alias y constantes compartidas por todo el proyecto.
// Concentrar esto aca evita "numeros magicos" repartidos por el codigo
// y deja explicito el contrato entre modulos (rubrica: uso de typedefs/enums).
#pragma once

#include <unordered_map>

namespace streaming {

// --- Alias (typedef modernos) -------------------------------------------
using IdPelicula  = int;                                   // indice en el vector de peliculas
using Puntaje     = int;                                   // puntaje acumulado de relevancia
using MapaRanking = std::unordered_map<IdPelicula, Puntaje>; // id -> puntaje acumulado

// --- Categorias de Tag ----------------------------------------------------
// enum class evita conversiones implicitas accidentales a int.
enum class TipoTag {
    DIRECTOR,
    CAST,
    GENERO
};

// --- Pesos del algoritmo de importancia (ver docs/pseudocode_busqueda.txt) -
// A mayor peso, mayor relevancia de esa coincidencia en el ranking final.
inline constexpr Puntaje PESO_TITULO = 10; // coincidencia en el titulo
inline constexpr Puntaje PESO_TAG    = 7;  // coincidencia en director/cast/genero
inline constexpr Puntaje PESO_PLOT   = 3;  // coincidencia en la sinopsis

// Tamano de pagina para mostrar resultados de busqueda (5 en 5, segun enunciado)
inline constexpr int TAM_PAGINA = 5;

} // namespace streaming
