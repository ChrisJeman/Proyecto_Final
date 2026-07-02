// IndiceBusqueda.h
//
// PROGRAMACION PARALELA:
// La construccion del indice es la parte mas costosa del programa. 
// La estrategia elegida es "sharding"
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
    Trie<MapaRanking> trieTexto;     
    Trie<MapaRanking> trieDirector;  
    Trie<MapaRanking> trieCast;      
    Trie<MapaRanking> trieGenero;    
};

class IndiceBusqueda {
public:
    explicit IndiceBusqueda(std::shared_ptr<IEstrategiaRanking> estrategia, unsigned numShards = 0);


    double construir(const std::vector<Pelicula>& pelis, bool paralelo = true);

  
    std::vector<std::pair<IdPelicula, Puntaje>> buscarTexto(const std::string& consulta, int pagina) const;


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

} 
