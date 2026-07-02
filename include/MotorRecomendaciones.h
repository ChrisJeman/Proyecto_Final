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

} 
