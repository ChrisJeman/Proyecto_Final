// LectorCSV.cpp
#include "LectorCSV.h"
#include "PeliculaBuilder.h"

#include <algorithm>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <thread>

namespace streaming {

std::vector<std::string> LectorCSV::leerRegistros(const std::string& ruta) {
    std::vector<std::string> registros;
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cout << "Error abriendo el archivo: " << ruta << '\n';
        return registros;
    }

    std::string linea;
    if (!std::getline(archivo, linea)) { // descartar cabecera
        return registros;
    }

    while (std::getline(archivo, linea)) {
        std::string registro = linea;
        int comillas = static_cast<int>(std::count(registro.begin(), registro.end(), '"'));

        // Un campo entre comillas puede contener saltos de linea (sinopsis
        // multi-parrafo); mientras el numero de comillas sea impar seguimos
        // uniendo lineas porque el campo todavia no se cerro.
        while ((comillas % 2) != 0 && std::getline(archivo, linea)) {
            registro += '\n';
            registro += linea;
            comillas += static_cast<int>(std::count(linea.begin(), linea.end(), '"'));
        }

        registros.push_back(std::move(registro));
    }

    return registros;
}

std::vector<std::string> LectorCSV::dividirRegistro(const std::string& registro) {
    std::vector<std::string> campos;
    std::string campo;
    bool dentroComillas = false;

    for (std::size_t i = 0; i < registro.size(); ++i) {
        char c = registro[i];

        if (c == '"') {
            if (dentroComillas && i + 1 < registro.size() && registro[i + 1] == '"') {
                campo.push_back('"');
                ++i; // comilla escapada ("")
            } else {
                dentroComillas = !dentroComillas;
            }
        } else if (c == ',' && !dentroComillas) {
            campos.push_back(campo);
            campo.clear();
        } else {
            campo.push_back(c);
        }
    }
    campos.push_back(campo);
    return campos;
}

std::vector<Pelicula> LectorCSV::parsearRango(const std::vector<std::string>& registros,
                                                 std::size_t inicio, std::size_t fin) {
    std::vector<Pelicula> resultado;
    resultado.reserve(fin - inicio);

    for (std::size_t i = inicio; i < fin; ++i) {
        std::vector<std::string> campos = dividirRegistro(registros[i]);
        if (campos.size() < 8) {
            continue;
        }

        Pelicula peli = PeliculaBuilder()
                            .conId(static_cast<IdPelicula>(i))
                            .conAnioTexto(campos[0])
                            .conTitulo(campos[1])
                            .conOrigen(campos[2])
                            .conDirector(campos[3])
                            .conCast(campos[4])
                            .conGenero(campos[5])
                            .conWikiPage(campos[6])
                            .conPlot(campos[7])
                            .construir();

        resultado.push_back(std::move(peli));
    }

    return resultado;
}

std::vector<Pelicula> LectorCSV::cargar(const std::string& ruta, bool paralelo, unsigned numHilos) {
    std::vector<std::string> registros = leerRegistros(ruta);
    if (registros.empty()) {
        return {};
    }

    if (numHilos == 0) {
        numHilos = std::max(1u, std::thread::hardware_concurrency());
    }

    if (!paralelo || numHilos <= 1 || registros.size() < numHilos) {
        return parsearRango(registros, 0, registros.size());
    }

    std::vector<std::future<std::vector<Pelicula>>> futuros;
    std::size_t total = registros.size();
    std::size_t tamanoBloque = (total + numHilos - 1) / numHilos;

    for (unsigned h = 0; h < numHilos; ++h) {
        std::size_t inicio = h * tamanoBloque;
        std::size_t fin = std::min(total, inicio + tamanoBloque);
        if (inicio >= fin) {
            break;
        }
        futuros.push_back(std::async(std::launch::async, parsearRango, std::cref(registros), inicio, fin));
    }

    std::vector<Pelicula> todas;
    todas.reserve(total);
    for (auto& f : futuros) {
        std::vector<Pelicula> bloque = f.get();
        todas.insert(todas.end(),
                     std::make_move_iterator(bloque.begin()),
                     std::make_move_iterator(bloque.end()));
    }

    return todas;
}

} 
