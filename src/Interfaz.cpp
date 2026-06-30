// Interfaz.cpp
#include "Interfaz.h"

#include <iostream>
#include <limits>

namespace streaming {

Interfaz::Interfaz(std::vector<Pelicula>& pelis, IndiceBusqueda& indice, GestorSesion& sesion,
                     MotorRecomendaciones& recomendaciones)
    : pelis_(pelis), indice_(indice), sesion_(sesion), recomendaciones_(recomendaciones) {}

void Interfaz::mostrarMenu() const {
    std::cout << "\n========== PLATAFORMA DE STREAMING ==========\n";
    std::cout << "1. Buscar por palabra / frase / sub-palabra\n";
    std::cout << "2. Buscar por Tag (Director, Cast, Genero)\n";
    std::cout << "3. Ver mas tarde\n";
    std::cout << "4. Recomendaciones (similares a tus Likes)\n";
    std::cout << "5. Salir\n";
    std::cout << "Seleccione una opcion: ";
}

void Interfaz::iniciar() {
    // Requisito del enunciado: al iniciar, mostrar Ver-mas-tarde y similares.
    mostrarListaSimple(sesion_.obtenerVerMasTarde(), "Ver mas tarde (sesion anterior)");
    mostrarListaSimple(recomendaciones_.obtenerRecomendaciones(), "Porque le diste Like a algo similar");

    while (true) {
        mostrarMenu();

        int opcion = 0;
        if (!(std::cin >> opcion)) {
            if (std::cin.eof()) {
                // Entrada finalizada de forma inesperada (ej. redireccion de
                // archivo, pipe cerrado): salimos en vez de quedar en un
                // bucle leyendo de un stream que ya no tiene datos.
                std::cout << "\nEntrada finalizada. Saliendo...\n";
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nOpcion invalida. Intente nuevamente.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcion == 5) {
            std::cout << "\nGuardando sesion y saliendo...\n";
            break;
        }

        switch (opcion) {
            case 1:
                opcionBuscarTexto();
                break;
            case 2:
                opcionBuscarPorTag();
                break;
            case 3:
                opcionVerMasTarde();
                break;
            case 4:
                opcionRecomendaciones();
                break;
            default:
                std::cout << "\nOpcion invalida. Intente nuevamente.\n";
                break;
        }
    }
}

void Interfaz::mostrarListaSimple(const std::vector<IdPelicula>& ids, const std::string& tituloSeccion) {
    if (ids.empty()) {
        return;
    }
    std::cout << "\n----- " << tituloSeccion << " -----\n";
    for (std::size_t i = 0; i < ids.size(); ++i) {
        IdPelicula id = ids[i];
        if (id >= 0 && static_cast<std::size_t>(id) < pelis_.size()) {
            pelis_[id].mostrarResumen(static_cast<int>(i + 1));
        }
    }
}

void Interfaz::mostrarResultadosPaginados(
    const std::string& descripcionConsulta,
    const std::function<std::vector<std::pair<IdPelicula, Puntaje>>(int)>& obtenerPagina) {

    int pagina = 0;
    while (true) {
        std::vector<std::pair<IdPelicula, Puntaje>> resultados = obtenerPagina(pagina);

        std::cout << "\n----- Resultados para \"" << descripcionConsulta << "\" (pagina " << (pagina + 1)
                   << ") -----\n";

        if (resultados.empty()) {
            std::cout << (pagina == 0 ? "No se encontraron peliculas.\n" : "No hay mas resultados.\n");
            break;
        }

        for (std::size_t i = 0; i < resultados.size(); ++i) {
            IdPelicula id = resultados[i].first;
            if (id >= 0 && static_cast<std::size_t>(id) < pelis_.size()) {
                std::cout << "  " << (i + 1) << ". " << pelis_[id].getTitulo() << " (" << pelis_[id].getAnio()
                           << ")  [puntaje: " << resultados[i].second << "]\n";
            }
        }

        std::cout << "\nEscriba 1-" << resultados.size()
                   << " para ver el detalle, 'n' para la siguiente pagina, o 'q' para volver: ";
        std::string entrada;
        std::getline(std::cin, entrada);

        if (entrada == "q" || entrada == "Q" || entrada.empty()) {
            break;
        }
        if (entrada == "n" || entrada == "N") {
            ++pagina;
            continue;
        }

        try {
            int seleccion = std::stoi(entrada);
            if (seleccion >= 1 && static_cast<std::size_t>(seleccion) <= resultados.size()) {
                manejarSeleccion(resultados[static_cast<std::size_t>(seleccion - 1)].first);
            } else {
                std::cout << "Numero fuera de rango.\n";
            }
        } catch (...) {
            std::cout << "Entrada no reconocida.\n";
        }
    }
}

void Interfaz::opcionBuscarTexto() {
    std::cout << "\nIngrese palabra, frase o sub-palabra a buscar: ";
    std::string consulta;
    std::getline(std::cin, consulta);

    if (consulta.empty()) {
        std::cout << "Busqueda vacia.\n";
        return;
    }

    mostrarResultadosPaginados(consulta,
                                 [this, consulta](int pagina) { return indice_.buscarTexto(consulta, pagina); });
}

void Interfaz::opcionBuscarPorTag() {
    std::cout << "\nBuscar por Tag:\n  1. Director\n  2. Cast\n  3. Genero\nSeleccione: ";
    std::string opcionTag;
    std::getline(std::cin, opcionTag);

    TipoTag tipo;
    std::string nombreTipo;
    if (opcionTag == "1") {
        tipo = TipoTag::DIRECTOR;
        nombreTipo = "Director";
    } else if (opcionTag == "2") {
        tipo = TipoTag::CAST;
        nombreTipo = "Cast";
    } else if (opcionTag == "3") {
        tipo = TipoTag::GENERO;
        nombreTipo = "Genero";
    } else {
        std::cout << "Opcion invalida.\n";
        return;
    }

    std::cout << "Ingrese el valor de " << nombreTipo << " a buscar: ";
    std::string consulta;
    std::getline(std::cin, consulta);

    if (consulta.empty()) {
        std::cout << "Busqueda vacia.\n";
        return;
    }

    mostrarResultadosPaginados(nombreTipo + ": " + consulta, [this, tipo, consulta](int pagina) {
        return indice_.buscarPorTag(tipo, consulta, pagina);
    });
}

void Interfaz::opcionVerMasTarde() {
    const std::vector<IdPelicula>& ids = sesion_.obtenerVerMasTarde();
    if (ids.empty()) {
        std::cout << "\nTu lista de Ver mas tarde esta vacia.\n";
        return;
    }

    mostrarListaSimple(ids, "Ver mas tarde");
    std::cout << "\nEscriba un numero para ver el detalle, o Enter para volver: ";
    std::string entrada;
    std::getline(std::cin, entrada);
    if (entrada.empty()) {
        return;
    }

    try {
        int seleccion = std::stoi(entrada);
        if (seleccion >= 1 && static_cast<std::size_t>(seleccion) <= ids.size()) {
            manejarSeleccion(ids[static_cast<std::size_t>(seleccion - 1)]);
        } else {
            std::cout << "Numero fuera de rango.\n";
        }
    } catch (...) {
        std::cout << "Entrada no reconocida.\n";
    }
}

void Interfaz::opcionRecomendaciones() {
    std::vector<IdPelicula> ids = recomendaciones_.obtenerRecomendaciones();
    if (ids.empty()) {
        std::cout << "\nTodavia no hay recomendaciones (dale Like a alguna pelicula primero).\n";
        return;
    }

    mostrarListaSimple(ids, "Porque le diste Like a algo similar");
    std::cout << "\nEscriba un numero para ver el detalle, o Enter para volver: ";
    std::string entrada;
    std::getline(std::cin, entrada);
    if (entrada.empty()) {
        return;
    }

    try {
        int seleccion = std::stoi(entrada);
        if (seleccion >= 1 && static_cast<std::size_t>(seleccion) <= ids.size()) {
            manejarSeleccion(ids[static_cast<std::size_t>(seleccion - 1)]);
        } else {
            std::cout << "Numero fuera de rango.\n";
        }
    } catch (...) {
        std::cout << "Entrada no reconocida.\n";
    }
}

void Interfaz::manejarSeleccion(IdPelicula id) {
    if (id < 0 || static_cast<std::size_t>(id) >= pelis_.size()) {
        return;
    }

    const Pelicula& peli = pelis_[static_cast<std::size_t>(id)];
    peli.mostrarDetalle();

    if (sesion_.tieneLike(id)) {
        std::cout << "[Ya tienes Like en esta pelicula]\n";
    }
    if (sesion_.estaEnVerMasTarde(id)) {
        std::cout << "[Ya esta en tu lista de Ver mas tarde]\n";
    }

    std::cout << "\n1. Like\n2. Ver mas tarde\n3. Volver\nSeleccione una opcion: ";
    std::string entrada;
    std::getline(std::cin, entrada);

    if (entrada == "1") {
        sesion_.darLike(id);
        std::cout << "Le diste Like a \"" << peli.getTitulo() << "\".\n";
    } else if (entrada == "2") {
        sesion_.agregarVerMasTarde(id);
        std::cout << "\"" << peli.getTitulo() << "\" se agrego a tu lista de Ver mas tarde.\n";
    }
}

} // namespace streaming
