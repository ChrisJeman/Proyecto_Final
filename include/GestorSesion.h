// GestorSesion.h
//
// GestorSesion es el unico punto de acceso al estado de interaccion del
// usuario (Likes, Ver-mas-tarde) durante toda la ejecucion -> PATRON
// SINGLETON (hereda de Singleton<GestorSesion>, ver Singleton.h). Tambien
// actua como "sujeto" del PATRON OBSERVER: cuando el usuario da Like,
// notifica a todos los observadores suscritos (ver Observador.h) sin
// conocer sus tipos concretos.
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

    // Persistencia simple en disco (data/likes.txt, data/ver_mas_tarde.txt)
    // usando el Repositorio<int> generico. Esto es lo que permite que, al
    // reiniciar el programa, la plataforma "recuerde" Ver-mas-tarde y los
    // Likes de la sesion anterior, tal como pide el enunciado.
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

} // namespace streaming
