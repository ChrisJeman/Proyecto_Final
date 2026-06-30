// Pelicula.cpp
#include "Pelicula.h"
#include "Texto.h"

#include <iostream>

namespace streaming {

Pelicula::Pelicula(IdPelicula id, int anio, std::string titulo, std::string origen,
                     std::string director, std::string cast, std::string genero,
                     std::string wikiPage, std::string plot)
    : id_(id),
      anio_(anio),
      titulo_(std::move(titulo)),
      origen_(std::move(origen)),
      director_(std::move(director)),
      cast_(std::move(cast)),
      genero_(std::move(genero)),
      wikiPage_(std::move(wikiPage)),
      plot_(std::move(plot)) {}

std::vector<std::string> Pelicula::generosComoConjunto() const {
    return texto::tokenizarCampoMultivaluado(genero_);
}

void Pelicula::mostrarResumen(int indice) const {
    if (indice >= 0) {
        std::cout << "  " << indice << ". ";
    }
    std::cout << titulo_ << " (" << anio_ << ")";
    if (!genero_.empty()) {
        std::cout << " - " << genero_;
    }
    std::cout << '\n';
}

void Pelicula::mostrarDetalle() const {
    std::cout << "\n=========================\n";
    std::cout << "Titulo: " << titulo_ << '\n';
    std::cout << "Anio: " << anio_ << '\n';
    std::cout << "Origen: " << origen_ << '\n';
    std::cout << "Director: " << director_ << '\n';
    std::cout << "Cast: " << cast_ << '\n';
    std::cout << "Genero: " << genero_ << '\n';
    std::cout << "Sinopsis: " << plot_ << '\n';
    std::cout << "Wiki: " << wikiPage_ << '\n';
    std::cout << "=========================\n";
}

} // namespace streaming
