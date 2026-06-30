// IndiceBusqueda.h
//
// PATRON DE DISENO: FACADE
// Por dentro, el motor de busqueda esta compuesto de varios Trie<MapaRanking>
// (uno para texto general, uno por cada tipo de Tag) repetidos por cada
// "shard" en que se particiona el catalogo (ver mas abajo). IndiceBusqueda
// expone solo 3 operaciones simples (construir, buscarTexto, buscarPorTag) y
// esconde toda esa complejidad interna: Interfaz/Comando jamas tocan un Trie
// directamente.
//
// PROGRAMACION PARALELA (parte 2 de 2):
// La construccion del indice (tokenizar titulo/plot/tags de cada pelicula e
// insertarlo en un Trie) es la parte mas costosa del programa porque se hace
// una vez por cada una de las ~35000 peliculas. La estrategia elegida es
// "sharding": el catalogo se reparte en N bloques contiguos y cada hilo
// construye, de punta a punta, un SHARD completo (su propio conjunto de 4
// Trie) usando SOLO su bloque de peliculas. Como cada hilo escribe
// exclusivamente en memoria que el creo (su shard), no hace falta ningun
// mutex ni sincronizacion durante la construccion: es paralelismo de datos
// puro. Al buscar, simplemente se consulta cada shard (son pocos, p.ej. 4
// u 8) y se suman los puntajes - el costo de esa fusion final es
// insignificante comparado con construir el indice.
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Comun.h"
#include "EstrategiaRanking.h"
#include "Pelicula.h"
#include "Trie.h"

namespace streaming {

struct ShardIndice {
    Trie<MapaRanking> trieTexto;     // titulo (sufijos, peso 10) + tags (peso 7) + plot (peso 3)
    Trie<MapaRanking> trieDirector;  // busqueda especifica por director
    Trie<MapaRanking> trieCast;      // busqueda especifica por actor/actriz
    Trie<MapaRanking> trieGenero;    // busqueda especifica por genero
};

class IndiceBusqueda {
public:
    explicit IndiceBusqueda(std::shared_ptr<IEstrategiaRanking> estrategia, unsigned numShards = 0);

    // Construye el indice en paralelo. Devuelve el tiempo que tomo
    // (en milisegundos) por si se quiere reportar/loggear.
    double construir(const std::vector<Pelicula>& pelis, bool paralelo = true);

    // Busqueda general por palabra, frase o sub-palabra (titulo/plot/tags).
    std::vector<std::pair<IdPelicula, Puntaje>> buscarTexto(const std::string& consulta, int pagina) const;

    // Busqueda restringida a una categoria de Tag especifica.
    std::vector<std::pair<IdPelicula, Puntaje>> buscarPorTag(TipoTag tipo, const std::string& consulta,
                                                                int pagina) const;

    unsigned numShards() const { return static_cast<unsigned>(shards_.size()); }

private:
    using SelectorTrie = std::function<const Trie<MapaRanking>&(const ShardIndice&)>;

    void indexarPelicula(ShardIndice& shard, const Pelicula& peli) const;
    MapaRanking acumularSobreShards(const std::string& consulta, const SelectorTrie& selector) const;
    std::vector<std::pair<IdPelicula, Puntaje>> paginar(const MapaRanking& acumulado, int pagina) const;

    std::vector<std::unique_ptr<ShardIndice>> shards_;
    std::shared_ptr<IEstrategiaRanking> estrategia_;
    unsigned numShardsConfigurado_ = 1;
};

} // namespace streaming
