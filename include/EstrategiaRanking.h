// EstrategiaRanking.h
//
// PATRON DE DISENO: STRATEGY
// Strategy nos permite encapsular detras de una interfaz, de forma que IndiceBusqueda no sabe
// como se calcula el orden. Asi se puede cambiar de algoritmo de ranking sin tocar el motor de busqueda
#pragma once

#include <utility>
#include <vector>

#include "Comun.h"

namespace streaming {

class IEstrategiaRanking {
public:
    virtual ~IEstrategiaRanking() = default;

    // Recibe <idPelicula, puntajeAcumulado> para todas las peliculas que tuvieron al menos una coincidenci
    virtual std::vector<std::pair<IdPelicula, Puntaje>> calcular(const MapaRanking& acumulado) const = 0;
};

// Estrategia: orden descendente por puntaje acumulado (suma de pesos de titulo/tag/plot, ver Comun.h). En caso de empate de puntaje, se desempata por id ascendente 
class RankingPorPesoAcumulado : public IEstrategiaRanking {
public:
    std::vector<std::pair<IdPelicula, Puntaje>> calcular(const MapaRanking& acumulado) const override;
};

}
