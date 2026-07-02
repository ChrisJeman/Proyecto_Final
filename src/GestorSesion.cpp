// GestorSesion.cpp
#include "GestorSesion.h"

#include <algorithm>

namespace streaming {
// GestorSesion es el unico punto de acceso al estado de interaccion del usuario
GestorSesion::GestorSesion() = default;

void GestorSesion::darLike(IdPelicula id) {
    if (!tieneLike(id)) {
        likes_.push_back(id);
    }
    for (const auto& observador : observadores_) {
        observador->alRecibirLike(id);
    }
}

void GestorSesion::agregarVerMasTarde(IdPelicula id) {
    if (!estaEnVerMasTarde(id)) {
        verMasTarde_.push_back(id);
    }
}

void GestorSesion::quitarVerMasTarde(IdPelicula id) {
    verMasTarde_.erase(std::remove(verMasTarde_.begin(), verMasTarde_.end(), id), verMasTarde_.end());
}

bool GestorSesion::tieneLike(IdPelicula id) const {
    return std::find(likes_.begin(), likes_.end(), id) != likes_.end();
}

bool GestorSesion::estaEnVerMasTarde(IdPelicula id) const {
    return std::find(verMasTarde_.begin(), verMasTarde_.end(), id) != verMasTarde_.end();
}

void GestorSesion::suscribir(std::shared_ptr<IObservadorLikes> observador) {
    observadores_.push_back(std::move(observador));
}

void GestorSesion::cargarDesdeDisco() {
    likes_ = repoLikes_.cargar();
    verMasTarde_ = repoVerMasTarde_.cargar();
}

void GestorSesion::guardarEnDisco() const {
    repoLikes_.guardar(likes_);
    repoVerMasTarde_.guardar(verMasTarde_);
}

} 
