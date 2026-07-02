// Pelicula.h

#pragma once

#include <string>
#include <vector>

#include "Comun.h"

namespace streaming {

class Pelicula {
public:
    Pelicula() = default;

    Pelicula(IdPelicula id, int anio, std::string titulo, std::string origen,
              std::string director, std::string cast, std::string genero,
              std::string wikiPage, std::string plot);

    IdPelicula getId() const { return id_; }
    int getAnio() const { return anio_; }
    const std::string& getTitulo() const { return titulo_; }
    const std::string& getOrigen() const { return origen_; }
    const std::string& getDirector() const { return director_; }
    const std::string& getCast() const { return cast_; }
    const std::string& getGenero() const { return genero_; }
    const std::string& getWikiPage() const { return wikiPage_; }
    const std::string& getPlot() const { return plot_; }

    // Campos multivaluados ya separados 
    std::vector<std::string> generosComoConjunto() const;

    void mostrarResumen(int indice = -1) const;

    // Impresion completa: sinopsis + datos
    void mostrarDetalle() const;

private:
    IdPelicula id_ = -1;
    int anio_ = 0;
    std::string titulo_, origen_, director_, cast_, genero_, wikiPage_, plot_;
};

} 
