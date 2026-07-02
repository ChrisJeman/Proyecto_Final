// main.cpp
#include <iostream>
#include <memory>

#include "Comun.h"
#include "EstrategiaRanking.h"
#include "EstrategiaSimilitud.h"
#include "GestorSesion.h"
#include "IndiceBusqueda.h"
#include "Interfaz.h"
#include "LectorCSV.h"
#include "MotorRecomendaciones.h"
#include "Pelicula.h"

using namespace streaming;

int main() {
    std::cout << "Cargando catalogo de peliculas...\n";

    std::vector<Pelicula> pelis = LectorCSV::cargar("data/wiki_movie_plots_deduped.csv");
    std::cout << "Peliculas cargadas: " << pelis.size() << "\n";

    if (pelis.empty()) {
        std::cout << "No se pudo cargar el catalogo. Verifique la ruta del CSV\n";
        return 1;
    }

    // --- Construccion del indice en paralelo
    auto estrategiaRanking = std::make_shared<RankingPorPesoAcumulado>();
    IndiceBusqueda indice(estrategiaRanking);

    double msIndexacion = indice.construir(pelis, /*paralelo=*/true);
    std::cout << "Indice construido en " << msIndexacion << " ms usando " << indice.numShards()
               << " hilo(s).\n";

    // --- Sesion del usuario (Singleton + Observer, se encuentra en GestorSesion.h)
    GestorSesion& sesion = GestorSesion::instancia();
    sesion.cargarDesdeDisco();

    auto estrategiaSimilitud = std::make_shared<SimilitudPorGeneroYDirector>();
    auto recomendaciones = std::make_shared<MotorRecomendaciones>(pelis, estrategiaSimilitud);
    // MotorRecomendaciones implementa IObservadorLikes
    sesion.suscribir(recomendaciones); 

    // Sembramos las recomendaciones con los Likes de la sesion anterior, para poder mostrarlas ya al iniciar el programa 

    Interfaz interfaz(pelis, indice, sesion, *recomendaciones);
    interfaz.iniciar();

    sesion.guardarEnDisco();
    return 0;
}
