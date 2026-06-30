// EstrategiaSimilitud.cpp
#include "EstrategiaSimilitud.h"
#include "Texto.h"

#include <algorithm>
#include <set>

namespace streaming {

double SimilitudPorGeneroYDirector::calcular(const Pelicula& base, const Pelicula& candidata) const {
    if (base.getId() == candidata.getId()) {
        return 0.0; // una pelicula no se recomienda a si misma
    }

    auto generosBase = base.generosComoConjunto();
    auto generosCand = candidata.generosComoConjunto();

    std::set<std::string> conjuntoBase(generosBase.begin(), generosBase.end());
    std::set<std::string> conjuntoCand(generosCand.begin(), generosCand.end());

    std::vector<std::string> interseccion;
    std::vector<std::string> unionTotal;

    std::set_intersection(conjuntoBase.begin(), conjuntoBase.end(),
                           conjuntoCand.begin(), conjuntoCand.end(),
                           std::back_inserter(interseccion));
    std::set_union(conjuntoBase.begin(), conjuntoBase.end(),
                   conjuntoCand.begin(), conjuntoCand.end(),
                   std::back_inserter(unionTotal));

    double jaccard = unionTotal.empty()
                          ? 0.0
                          : static_cast<double>(interseccion.size()) / static_cast<double>(unionTotal.size());

    bool mismoDirector = !base.getDirector().empty() &&
                          texto::aMinusculas(base.getDirector()) == texto::aMinusculas(candidata.getDirector());

    return jaccard + (mismoDirector ? 0.25 : 0.0);
}

} // namespace streaming
