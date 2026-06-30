// benchmark_indexacion.cpp
//
// Programa standalone (no es parte de la app interactiva) que mide el
// tiempo de CONSTRUCCION DEL INDICE con distintas cantidades de hilos,
// para poder llenar la tabla comparativa del README con numeros reales
// medidos en la maquina donde se ejecute (no inventados).
//
// Uso: ./benchmark_indexacion [ruta_csv]
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#include "EstrategiaRanking.h"
#include "IndiceBusqueda.h"
#include "LectorCSV.h"
#include "Pelicula.h"

using namespace streaming;

int main(int argc, char** argv) {
    std::string ruta = (argc > 1) ? argv[1] : "data/wiki_movie_plots_deduped.csv";

    std::cout << "Cargando catalogo desde " << ruta << "...\n";
    std::vector<Pelicula> pelis = LectorCSV::cargar(ruta, /*paralelo=*/true);
    std::cout << "Peliculas cargadas: " << pelis.size() << "\n\n";

    if (pelis.empty()) {
        std::cout << "No se pudo cargar el catalogo.\n";
        return 1;
    }

    auto estrategia = std::make_shared<RankingPorPesoAcumulado>();

    std::vector<unsigned> configuraciones = {1, 2, 4, 8};

    std::cout << std::left << std::setw(12) << "Hilos" << std::setw(18) << "Tiempo (ms)" << "Speedup vs 1 hilo\n";
    std::cout << "--------------------------------------------------\n";

    double tiempoBase = 0.0;
    for (unsigned hilos : configuraciones) {
        IndiceBusqueda indice(estrategia, hilos);
        double ms = indice.construir(pelis, /*paralelo=*/true);
        if (hilos == 1) {
            tiempoBase = ms;
        }
        double speedup = (tiempoBase > 0.0) ? tiempoBase / ms : 1.0;

        std::cout << std::left << std::setw(12) << hilos << std::setw(18) << std::fixed << std::setprecision(1)
                   << ms << std::setprecision(2) << speedup << "x\n";
    }

    return 0;
}
