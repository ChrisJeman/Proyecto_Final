// LectorCSV.h

#pragma once

#include <string>
#include <vector>

#include "Pelicula.h"

namespace streaming {

class LectorCSV {
public:

    static std::vector<Pelicula> cargar(const std::string& ruta, bool paralelo = true,
                                          unsigned numHilos = 0);

private:
    // Lee el archivo completo y devuelve un registro CSV por elemento
    static std::vector<std::string> leerRegistros(const std::string& ruta);

    static std::vector<std::string> dividirRegistro(const std::string& registro);

    // Convierte un rango de registros en objetos Pelicula.
    static std::vector<Pelicula> parsearRango(const std::vector<std::string>& registros,
                                                std::size_t inicio, std::size_t fin);
};

}