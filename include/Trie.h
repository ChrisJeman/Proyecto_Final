// Trie.h
//
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

    // Busca la clave en el trie y devuelve un puntero al valor asociado, o nullptr 
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

}
