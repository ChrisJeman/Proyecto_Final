#include "Trie.h"

Trie::Trie() {

    raiz = new TrieNode();
}

void Trie::insertar(string palabra, int idPelicula) {

    TrieNode* actual = raiz;

    for(char c : palabra) {

        if(actual->hijos.count(c) == 0) {

            actual->hijos[c] = new TrieNode();
        }

        actual = actual->hijos[c];
    }

    actual->finPalabra = true;

    bool existe = false;

    for(int id : actual->peliculas) {

        if(id == idPelicula) {

            existe = true;
        }
    }

    if(!existe) {

        actual->peliculas.push_back(idPelicula);
    }
}

vector<int> Trie::buscar(string palabra) {

    TrieNode* actual = raiz;

    for(char c : palabra) {

        if(actual->hijos.count(c) == 0) {

            return {};
        }

        actual = actual->hijos[c];
    }

    return actual->peliculas;
}
