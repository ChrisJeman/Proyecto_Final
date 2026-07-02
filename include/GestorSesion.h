// GestorSesion.h
// PATRON SINGLETON
// GestorSesion es el unico punto de acceso al estado de interaccion del
// usuario (Likes, Ver-mas-tarde) durante toda la ejecucion .Hereda de Singleton<GestorSesion>, ver Singleton.h)
#pragma once

#include <memory>
#include <vector>

#include "Comun.h"
#include "Observador.h"
#include "Repositorio.h"
#include "Singleton.h"

namespace streaming {

class GestorSesion : public Singleton<GestorSesion> {
    friend class Singleton<GestorSesion>;

public:
    void darLike(IdPelicula id);
    void agregarVerMasTarde(IdPelicula id);
    void quitarVerMasTarde(IdPelicula id);

    bool tieneLike(IdPelicula id) const;
    bool estaEnVerMasTarde(IdPelicula id) const;

    const std::vector<IdPelicula>& obtenerLikes() const { return likes_; }
    const std::vector<IdPelicula>& obtenerVerMasTarde() const { return verMasTarde_; }

    void suscribir(std::shared_ptr<IObservadorLikes> observador);

    void cargarDesdeDisco();
    void guardarEnDisco() const;

private:
    GestorSesion();

    std::vector<IdPelicula> likes_;
    std::vector<IdPelicula> verMasTarde_;
    std::vector<std::shared_ptr<IObservadorLikes>> observadores_;

    Repositorio<IdPelicula> repoLikes_{"data/likes.txt"};
    Repositorio<IdPelicula> repoVerMasTarde_{"data/ver_mas_tarde.txt"};
};

}
