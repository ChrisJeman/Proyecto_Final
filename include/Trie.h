// Trie.h
//
// PROGRAMACION GENERICA:
// En vez de tener una clase Trie distinta y casi-duplicada para titulo,
// para cada Tag (director/cast/genero) y para la sinopsis, definimos UN
// solo template Trie<T> donde T es el "dato" que vive en cada nodo (en
// este proyecto, T = MapaRanking, es decir un unordered_map<id,puntaje>).
// El mismo Trie<T> se instancia 4 veces (texto, director, cast, genero)
// en IndiceBusqueda, y se vuelve a instanciar N veces mas (una por shard)
// para la construccion en paralelo. Ademas, el metodo insertar() es a su
// vez una plantilla sobre el "combinador": una funcion/lambda que decide
// COMO fusionar un nuevo aporte dentro de T (acumular puntaje, agregar a
// un vector, etc.), igual que hacen los algoritmos de <algorithm> con
// funtores definidos por quien los llama.
#pragma once

#include <memory>
#include <string_view>
#include <unordered_map>

namespace streaming {

template <typename T>
struct NodoTrie {
    std::unordered_map<char, std::unique_ptr<NodoTrie<T>>> hijos;
    T datos{};
    bool finPalabra = false;
};

template <typename T>
class Trie {
public:
    Trie() : raiz_(std::make_unique<NodoTrie<T>>()) {}

    // Inserta 'clave' en el trie y aplica 'combinar' sobre el dato T del
    // nodo correspondiente.
    //
    // marcarCamino = false (por defecto): solo el nodo FINAL de 'clave'
    //   recibe el aporte -> sirve para indexar palabras completas (plot,
    //   tags). Es mas barato en memoria porque no "contamina" los nodos
    //   intermedios.
    // marcarCamino = true: TODOS los nodos del camino reciben el aporte ->
    //   permite luego encontrar 'clave' como PREFIJO de lo insertado, que
    //   combinado con insertar todos los sufijos de una palabra (ver
    //   IndiceBusqueda) habilita la busqueda por sub-cadena en cualquier
    //   posicion (ej. "bar" dentro de "barco"). Se usa solo para el titulo,
    //   que es corto; en sinopsis encarecería demasiado la memoria.
    template <typename Combinador>
    void insertar(std::string_view clave, const Combinador& combinar, bool marcarCamino = false) {
        NodoTrie<T>* actual = raiz_.get();
        for (char c : clave) {
            auto& hijo = actual->hijos[c];
            if (!hijo) {
                hijo = std::make_unique<NodoTrie<T>>();
            }
            actual = hijo.get();
            if (marcarCamino) {
                combinar(actual->datos);
            }
        }
        actual->finPalabra = true;
        if (!marcarCamino) {
            combinar(actual->datos);
        }
    }

    // Devuelve un puntero al dato T asociado a 'clave', o nullptr si la
    // cadena no existe en el trie. No copia T (importante: T puede ser un
    // unordered_map potencialmente grande).
    const T* buscar(std::string_view clave) const {
        const NodoTrie<T>* actual = raiz_.get();
        for (char c : clave) {
            auto it = actual->hijos.find(c);
            if (it == actual->hijos.end()) {
                return nullptr;
            }
            actual = it->second.get();
        }
        return &actual->datos;
    }

private:
    std::unique_ptr<NodoTrie<T>> raiz_;
};

} // namespace streaming
