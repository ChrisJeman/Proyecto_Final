#ifndef TRIENODE_H
#define TRIENODE_H

#include <unordered_map>
#include <vector>

using namespace std;

class TrieNode {

public:

    unordered_map<char, TrieNode*> hijos;

    vector<int> peliculas;

    bool finPalabra;

    TrieNode();
};

#endif
