// IndiceBusqueda.cpp
#include "IndiceBusqueda.h"
#include "Texto.h"

#include <algorithm>
#include <chrono>
#include <future>
#include <thread>

namespace streaming {

IndiceBusqueda::IndiceBusqueda(std::shared_ptr<IEstrategiaRanking> estrategia, unsigned numShards)
    : estrategia_(std::move(estrategia)) {
    numShardsConfigurado_ = (numShards == 0) ? std::max(1u, std::thread::hardware_concurrency()) : numShards;
}

void IndiceBusqueda::indexarPelicula(ShardIndice& shard, const Pelicula& peli) const {
    const IdPelicula id = peli.getId();

    // --- Titulo: Suffix Trie con peso alto, marcando TODO el camino para
    //     habilitar busqueda por sub-palabra (ver Trie.h, marcarCamino). ---
    for (const std::string& palabra : texto::tokenizarCampoMultivaluado(peli.getTitulo())) {
        for (std::size_t j = 0; j < palabra.size(); ++j) {
            std::string_view sufijo(palabra.data() + j, palabra.size() - j);
            shard.trieTexto.insertar(sufijo,
                                      [id](MapaRanking& m) { m[id] += PESO_TITULO; },
                                      /*marcarCamino=*/true);
        }
    }

    // --- Tags: Trie estandar (solo palabra completa) para no explotar la
    //     memoria como pasaria con sufijos sobre la sinopsis completa. ---
    auto indexarTag = [&](const std::string& valor, Trie<MapaRanking>& trieEspecifico) {
        for (const std::string& palabra : texto::tokenizarCampoMultivaluado(valor)) {
            shard.trieTexto.insertar(palabra, [id](MapaRanking& m) { m[id] += PESO_TAG; });
            trieEspecifico.insertar(palabra, [id](MapaRanking& m) { m[id] += 1; });
        }
    };
    indexarTag(peli.getDirector(), shard.trieDirector);
    indexarTag(peli.getCast(), shard.trieCast);
    indexarTag(peli.getGenero(), shard.trieGenero);

    // --- Sinopsis: Trie estandar, palabra completa, peso bajo. ---
    for (const std::string& palabra : texto::tokenizarCampoMultivaluado(peli.getPlot())) {
        shard.trieTexto.insertar(palabra, [id](MapaRanking& m) { m[id] += PESO_PLOT; });
    }
}

double IndiceBusqueda::construir(const std::vector<Pelicula>& pelis, bool paralelo) {
    auto t0 = std::chrono::high_resolution_clock::now();
    shards_.clear();

    std::size_t total = pelis.size();
    unsigned numHilos = (!paralelo) ? 1 : numShardsConfigurado_;
    numHilos = static_cast<unsigned>(std::max<std::size_t>(1, std::min<std::size_t>(numHilos, std::max<std::size_t>(1, total))));

    std::size_t tamanoBloque = (total + numHilos - 1) / std::max<unsigned>(1, numHilos);

    // Cada tarea construye un ShardIndice COMPLETO e independiente: sin
    // memoria compartida mutable entre hilos, por lo tanto sin necesidad de
    // mutex/locks (paralelismo de datos puro).
    std::vector<std::future<std::unique_ptr<ShardIndice>>> futuros;

    for (unsigned h = 0; h < numHilos; ++h) {
        std::size_t inicio = h * tamanoBloque;
        std::size_t fin = std::min(total, inicio + tamanoBloque);
        if (inicio >= fin) {
            break;
        }

        futuros.push_back(std::async(std::launch::async, [this, &pelis, inicio, fin]() {
            auto shard = std::make_unique<ShardIndice>();
            for (std::size_t i = inicio; i < fin; ++i) {
                indexarPelicula(*shard, pelis[i]);
            }
            return shard;
        }));
    }

    for (auto& f : futuros) {
        shards_.push_back(f.get());
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

MapaRanking IndiceBusqueda::acumularSobreShards(const std::string& consulta, const SelectorTrie& selector) const {
    MapaRanking acumulado;
    std::string normalizada = texto::normalizar(consulta);

    for (const std::string& token : texto::tokenizar(normalizada)) {
        for (const auto& shard : shards_) {
            const MapaRanking* parcial = selector(*shard).buscar(token);
            if (parcial == nullptr) {
                continue;
            }
            for (const auto& [id, puntaje] : *parcial) {
                acumulado[id] += puntaje;
            }
        }
    }

    return acumulado;
}

std::vector<std::pair<IdPelicula, Puntaje>> IndiceBusqueda::paginar(const MapaRanking& acumulado,
                                                                       int pagina) const {
    std::vector<std::pair<IdPelicula, Puntaje>> ordenado = estrategia_->calcular(acumulado);

    std::size_t indiceInicio = static_cast<std::size_t>(std::max(0, pagina)) * TAM_PAGINA;
    if (indiceInicio >= ordenado.size()) {
        return {};
    }

    std::size_t indiceFin = std::min(ordenado.size(), indiceInicio + TAM_PAGINA);
    return std::vector<std::pair<IdPelicula, Puntaje>>(ordenado.begin() + indiceInicio,
                                                          ordenado.begin() + indiceFin);
}

std::vector<std::pair<IdPelicula, Puntaje>> IndiceBusqueda::buscarTexto(const std::string& consulta,
                                                                           int pagina) const {
    MapaRanking acumulado = acumularSobreShards(consulta, [](const ShardIndice& s) -> const Trie<MapaRanking>& {
        return s.trieTexto;
    });
    return paginar(acumulado, pagina);
}

std::vector<std::pair<IdPelicula, Puntaje>> IndiceBusqueda::buscarPorTag(TipoTag tipo, const std::string& consulta,
                                                                            int pagina) const {
    SelectorTrie selector;
    switch (tipo) {
        case TipoTag::DIRECTOR:
            selector = [](const ShardIndice& s) -> const Trie<MapaRanking>& { return s.trieDirector; };
            break;
        case TipoTag::CAST:
            selector = [](const ShardIndice& s) -> const Trie<MapaRanking>& { return s.trieCast; };
            break;
        case TipoTag::GENERO:
        default:
            selector = [](const ShardIndice& s) -> const Trie<MapaRanking>& { return s.trieGenero; };
            break;
    }

    MapaRanking acumulado = acumularSobreShards(consulta, selector);
    return paginar(acumulado, pagina);
}

} // namespace streaming
