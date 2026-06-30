// EstrategiaRanking.cpp
#include "EstrategiaRanking.h"

#include <algorithm>

namespace streaming {

std::vector<std::pair<IdPelicula, Puntaje>> RankingPorPesoAcumulado::calcular(
    const MapaRanking& acumulado) const {

    std::vector<std::pair<IdPelicula, Puntaje>> lista(acumulado.begin(), acumulado.end());

    // partial_sort no aplica aqui porque necesitamos TODA la lista ordenada
    // (la paginacion puede pedir cualquier pagina, no solo el top-5). El
    // tamano de 'lista' es, como mucho, la cantidad de peliculas distintas
    // que matchearon la consulta -- normalmente muy por debajo del total
    // del catalogo, asi que el costo de sort es marginal.
    std::sort(lista.begin(), lista.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) {
            return a.second > b.second; // mayor puntaje primero
        }
        return a.first < b.first; // desempate estable
    });

    return lista;
}

} // namespace streaming
