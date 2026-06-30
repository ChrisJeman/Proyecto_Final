// Observador.h
//
// PATRON DE DISENO: OBSERVER (interfaz del lado "observador")
// GestorSesion (el "sujeto") no deberia saber nada de "recomendaciones":
// su responsabilidad es solo guardar Likes/Ver-mas-tarde. Quien SI le
// interesa reaccionar a un nuevo Like es MotorRecomendaciones, que se
// suscribe implementando esta interfaz. Asi, agregar en el futuro otro
// modulo que reaccione a los Likes (por ejemplo, estadisticas de genero
// favorito) no requeriria modificar GestorSesion en absoluto.
#pragma once

#include "Comun.h"

namespace streaming {

class IObservadorLikes {
public:
    virtual ~IObservadorLikes() = default;
    virtual void alRecibirLike(IdPelicula id) = 0;
};

} // namespace streaming
