// EstrategiaSimilitud.h
//
// Segunda aplicacion del patron STRATEGY (ver EstrategiaRanking.h): el
// enunciado pide "implemente su propio algoritmo" para decidir que
// peliculas son similares a las que el usuario marco con Like. Aislamos
// ese algoritmo en su propia interfaz para que MotorRecomendaciones no
// dependa de una formula concreta.
#pragma once

#include "Pelicula.h"

namespace streaming {

class IEstrategiaSimilitud {
public:
    virtual ~IEstrategiaSimilitud() = default;

    // Devuelve un valor entre 0.0 (nada similar) y ~1.25 (genero identico +
    // mismo director) que indica cuanto se parece 'candidata' a 'base'.
    virtual double calcular(const Pelicula& base, const Pelicula& candidata) const = 0;
};

// Algoritmo propio: similitud de Jaccard entre los conjuntos de generos
// (interseccion / union de tokens de genero) mas un bono si comparten
// director. Es deliberadamente simple y barato de calcular (O(g log g),
// g = cantidad de generos) porque se evalua contra TODO el catalogo cada
// vez que el usuario da un nuevo Like.
class SimilitudPorGeneroYDirector : public IEstrategiaSimilitud {
public:
    double calcular(const Pelicula& base, const Pelicula& candidata) const override;
};

} // namespace streaming
