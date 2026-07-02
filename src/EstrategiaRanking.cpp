// EstrategiaRanking.cpp
#include "EstrategiaRanking.h"

#include <algorithm>

namespace streaming {

std::vector<std::pair<IdPelicula, Puntaje>> RankingPorPesoAcumulado::calcular(
    const MapaRanking& acumulado) const {

    std::vector<std::pair<IdPelicula, Puntaje>> lista(acumulado.begin(), acumulado.end());

    std::sort(lista.begin(), lista.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) {
            return a.second > b.second; // mayor puntaje primero
        }
        return a.first < b.first; // desempate estable
    });

    return lista;
}

}
