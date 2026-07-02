// PeliculaBuilder.h
//
// PATRON: BUILDER

#pragma once

#include <string>

#include "Pelicula.h"

namespace streaming {

class PeliculaBuilder {
public:
    PeliculaBuilder& conId(IdPelicula id);
    PeliculaBuilder& conAnioTexto(const std::string& anioTexto); 
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
