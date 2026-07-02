// Interfaz.h

#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Comun.h"
#include "GestorSesion.h"
#include "IndiceBusqueda.h"
#include "MotorRecomendaciones.h"
#include "Pelicula.h"

namespace streaming {

class Interfaz {
public:
    Interfaz(std::vector<Pelicula>& pelis, IndiceBusqueda& indice, GestorSesion& sesion,
              MotorRecomendaciones& recomendaciones);

    void iniciar();

private:
    void mostrarMenu() const;

    // Opciones del menu
    void opcionBuscarTexto();
    void opcionBuscarPorTag();
    void opcionVerMasTarde();
    void opcionRecomendaciones();

    void mostrarResultadosPaginados(const std::string& descripcionConsulta,
                                      const std::function<std::vector<std::pair<IdPelicula, Puntaje>>(int)>& obtenerPagina);

    void mostrarListaSimple(const std::vector<IdPelicula>& ids, const std::string& tituloSeccion);

    // Muestra la ficha de una pelicula seleccionada y ofrece Like/Ver mas tarde.
    void manejarSeleccion(IdPelicula id);

    std::vector<Pelicula>& pelis_;
    IndiceBusqueda& indice_;
    GestorSesion& sesion_;
    MotorRecomendaciones& recomendaciones_;
};

} 
