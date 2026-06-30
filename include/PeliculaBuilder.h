// PeliculaBuilder.h
//
// PATRON DE DISENO: BUILDER
// Pelicula tiene 8 campos; construirla con un constructor de 8 parametros
// posicionales es fragil (facil invertir dos strings por error) y dificil
// de leer en el sitio de la llamada. El Builder arma el objeto paso a paso
// con metodos nombrados y encadenables (fluent interface), y centraliza la
// conversion/validacion de datos (ej. anio invalido -> 0).
#pragma once

#include <string>

#include "Pelicula.h"

namespace streaming {

class PeliculaBuilder {
public:
    PeliculaBuilder& conId(IdPelicula id);
    PeliculaBuilder& conAnioTexto(const std::string& anioTexto); // parsea con stoi, 0 si falla
    PeliculaBuilder& conTitulo(std::string valor);
    PeliculaBuilder& conOrigen(std::string valor);
    PeliculaBuilder& conDirector(std::string valor);
    PeliculaBuilder& conCast(std::string valor);
    PeliculaBuilder& conGenero(std::string valor);
    PeliculaBuilder& conWikiPage(std::string valor);
    PeliculaBuilder& conPlot(std::string valor);

    Pelicula construir() const;

private:
    IdPelicula id_ = -1;
    int anio_ = 0;
    std::string titulo_, origen_, director_, cast_, genero_, wikiPage_, plot_;
};

} // namespace streaming
