// Observador.h
//
// PATRON: OBSERVER 
#pragma once

#include "Comun.h"

namespace streaming {

class IObservadorLikes {
public:
    virtual ~IObservadorLikes() = default;
    virtual void alRecibirLike(IdPelicula id) = 0;
};

} 