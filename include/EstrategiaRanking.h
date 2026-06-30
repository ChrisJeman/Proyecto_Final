// EstrategiaRanking.h
//
// PATRON DE DISENO: STRATEGY
// El enunciado pide "implementar un algoritmo para determinar que pelicula
// tiene mas importancia en una busqueda". Strategy nos permite encapsular
// ESE algoritmo detras de una interfaz, de forma que IndiceBusqueda no sabe
// (ni le importa) como se calcula el orden: solo pide "calcular(mapa)". Asi
// se puede cambiar de algoritmo de ranking sin tocar el motor de busqueda
// (por ejemplo, en el futuro podria agregarse una estrategia que tambien
// pondere por anio de estreno o popularidad, sin romper nada existente).
#pragma once

#include <utility>
#include <vector>

#include "Comun.h"

namespace streaming {

class IEstrategiaRanking {
public:
    virtual ~IEstrategiaRanking() = default;

    // Recibe <idPelicula, puntajeAcumulado> para TODAS las peliculas que
    // tuvieron al menos una coincidencia, y devuelve la lista ordenada de
    // mayor a menor importancia.
    virtual std::vector<std::pair<IdPelicula, Puntaje>> calcular(const MapaRanking& acumulado) const = 0;
};

// Estrategia concreta: orden descendente por puntaje acumulado (suma de
// pesos de titulo/tag/plot, ver Comun.h). En caso de empate de puntaje,
// se desempata por id ascendente para que el resultado sea determinista
// (mismo input -> mismo orden siempre, util para pruebas).
class RankingPorPesoAcumulado : public IEstrategiaRanking {
public:
    std::vector<std::pair<IdPelicula, Puntaje>> calcular(const MapaRanking& acumulado) const override;
};

} // namespace streaming
