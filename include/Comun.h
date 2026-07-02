// Comun.h
// Tipos, alias y constantes compartidas por el project.
#pragma once

#include <unordered_map>

namespace streaming {

using IdPelicula  = int;                                   // indice en el vector de peliculas
using Puntaje     = int;                                   // puntaje acumulado de relevancia
using MapaRanking = std::unordered_map<IdPelicula, Puntaje>; // id -> puntaje acumulado

// Categorias de Tag 
enum class TipoTag {
    DIRECTOR,
    CAST,
    GENERO
};

// Pesos del algoritmo de importancia 
// A mayor peso, mayor relevancia de esa coincidencia en el ranking final
inline constexpr Puntaje PESO_TITULO = 10; // coincidencia en el titulo
inline constexpr Puntaje PESO_TAG    = 7;  
inline constexpr Puntaje PESO_PLOT   = 3;  

//Tamano de pagina para mostrar resultados de busqueda (5 en 5)
inline constexpr int TAM_PAGINA = 5;

} 
