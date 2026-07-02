// EstrategiaSimilitud.h
//
// Patron STRATEGY (ver EstrategiaRanking.h): Aislamos
// el algoritmo en su propia interfaz para que MotorRecomendaciones no
// dependa de una formula concreta.
#pragma once

#include "Pelicula.h"

namespace streaming {

class IEstrategiaSimilitud {
public:
    virtual ~IEstrategiaSimilitud() = default;

    // Devuelve un valor entre 0.0 y 1.25 (genero identico + mismo director) que indica cuanto se parece 'candidata' a 'base' (menor a mayor similitud)
    virtual double calcular(const Pelicula& base, const Pelicula& candidata) const = 0;
};

// Algoritmo propio: similitud de Jaccard entre los conjuntos de generos (interseccion / union de tokens de genero) mas un bono si comparten director. (O(g log g),
// g = cantidad de generos) 
class SimilitudPorGeneroYDirector : public IEstrategiaSimilitud {
public:
    double calcular(const Pelicula& base, const Pelicula& candidata) const override;
};

} 
