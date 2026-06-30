// LectorCSV.h
//
// PROGRAMACION PARALELA (parte 1 de 2, ver tambien IndiceBusqueda.h):
// Leer el archivo en si es una operacion de E/S secuencial por naturaleza
// (un solo disco, un solo flujo de bytes), pero una vez que tenemos todos
// los registros en memoria como texto, CONVERTIR cada registro en un
// objeto Pelicula (separar columnas, parsear el anio, etc.) es trabajo de
// CPU puro e independiente registro-a-registro. Esa fase si se reparte
// entre varios hilos con std::async, sin necesidad de locks porque cada
// hilo escribe solo en su propio bloque del vector de salida.
#pragma once

#include <string>
#include <vector>

#include "Pelicula.h"

namespace streaming {

class LectorCSV {
public:
    // Carga el archivo CSV completo. Si paralelo=true, reparte el parseo de
    // registros entre numHilos hilos (0 = usar std::thread::hardware_concurrency()).
    static std::vector<Pelicula> cargar(const std::string& ruta, bool paralelo = true,
                                          unsigned numHilos = 0);

private:
    // Lee el archivo completo y devuelve un registro CSV por elemento (ya
    // habiendo unido las lineas que estaban partidas por saltos de linea
    // dentro de campos entre comillas).
    static std::vector<std::string> leerRegistros(const std::string& ruta);

    // Divide un registro CSV respetando comillas (campos con comas internas).
    static std::vector<std::string> dividirRegistro(const std::string& registro);

    // Convierte un rango [inicio, fin) de registros en objetos Pelicula.
    // El id asignado a cada pelicula es 'inicio + posicion_relativa', por lo
    // que el orden final coincide exactamente con el orden del CSV.
    static std::vector<Pelicula> parsearRango(const std::vector<std::string>& registros,
                                                std::size_t inicio, std::size_t fin);
};

} // namespace streaming
