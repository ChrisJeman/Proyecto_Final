// PeliculaBuilder.cpp
#include "PeliculaBuilder.h"

namespace streaming {

PeliculaBuilder& PeliculaBuilder::conId(IdPelicula id) {
    id_ = id;
    return *this;
}

PeliculaBuilder& PeliculaBuilder::conAnioTexto(const std::string& anioTexto) {
    try {
        anio_ = std::stoi(anioTexto);
    } catch (...) {
        anio_ = 0;
    }
    return *this;
}

PeliculaBuilder& PeliculaBuilder::conTitulo(std::string valor) {
    titulo_ = std::move(valor);
    return *this;
}

PeliculaBuilder& PeliculaBuilder::conOrigen(std::string valor) {
    origen_ = std::move(valor);
    return *this;
}

PeliculaBuilder& PeliculaBuilder::conDirector(std::string valor) {
    director_ = std::move(valor);
    return *this;
}

PeliculaBuilder& PeliculaBuilder::conCast(std::string valor) {
    cast_ = std::move(valor);
    return *this;
}

PeliculaBuilder& PeliculaBuilder::conGenero(std::string valor) {
    genero_ = std::move(valor);
    return *this;
}

PeliculaBuilder& PeliculaBuilder::conWikiPage(std::string valor) {
    wikiPage_ = std::move(valor);
    return *this;
}

PeliculaBuilder& PeliculaBuilder::conPlot(std::string valor) {
    plot_ = std::move(valor);
    return *this;
}

Pelicula PeliculaBuilder::construir() const {
    return Pelicula(id_, anio_, titulo_, origen_, director_, cast_, genero_, wikiPage_, plot_);
}

} // namespace streaming
