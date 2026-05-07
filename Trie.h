#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>

#include "TrieNode.h"

using namespace std;

class Trie {

private:

    TrieNode* raiz;

public:

    Trie();

    void insertar(string palabra, int idPelicula);

    vector<int> buscar(string palabra);
};

#endif
