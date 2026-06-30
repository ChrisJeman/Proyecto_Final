// MotorRecomendaciones.h
//
// Implementacion concreta de IObservadorLikes (PATRON OBSERVER). Cada vez
// que el usuario da un nuevo Like, GestorSesion la notifica y este motor
// recalcula las peliculas mas similares al CONJUNTO de peliculas con Like,
// usando la estrategia de similitud inyectada (PATRON STRATEGY, ver
// EstrategiaSimilitud.h).
//
// PROGRAMACION PARALELA (tercer uso en el proyecto): recalcular implica
// comparar cada pelicula con Like contra TODO el catalogo (decenas de
// miles de peliculas). Se reparte el catalogo en bloques y cada hilo
// calcula, de forma independiente, el top-K local de su bloque (usando
// std::partial_sort); al final solo se combinan esos top-K parciales
// (una lista pequena) y se vuelve a ordenar -- un esquema tipo
// "map-reduce" sencillo que evita comparar listas grandes con locks.
#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "Comun.h"
#include "EstrategiaSimilitud.h"
#include "Observador.h"
#include "Pelicula.h"

namespace streaming {

class MotorRecomendaciones : public IObservadorLikes {
public:
    MotorRecomendaciones(const std::vector<Pelicula>& catalogo,
                          std::shared_ptr<IEstrategiaSimilitud> estrategia, unsigned numHilos = 0);

    void alRecibirLike(IdPelicula id) override;

    // Recalcula desde cero a partir de una lista de likes ya existente
    // (usado al iniciar el programa, para mostrar similares de la sesion
    // anterior sin esperar un nuevo Like).
    void recalcularDesde(const std::vector<IdPelicula>& likesActuales);

    std::vector<IdPelicula> obtenerRecomendaciones() const;

private:
    void recalcular();

    const std::vector<Pelicula>& catalogo_;
    std::shared_ptr<IEstrategiaSimilitud> estrategia_;
    unsigned numHilos_;

    std::vector<IdPelicula> likes_;
    std::vector<IdPelicula> recomendacionesCache_;
    mutable std::mutex mutex_;

    static constexpr std::size_t MAX_RECOMENDACIONES = 5;
};

} // namespace streaming
