//
// Created by Saul Saavedra on 8/05/26.
//

#ifndef PROYECTO_PROGRA_III_INTERFAZ_H
#define PROYECTO_PROGRA_III_INTERFAZ_H

#include <iostream>
#include <vector>
#include <string>
#include "Pelis.h"
#include "Trie.h"

using namespace std;

class Interfaz {
private:
    vector<Pelis>& pelis;
    Trie& trie;

    string convertirMinusculas(string texto);
    void mostrarMenu();
    void buscarPelicula();
    void mostrarResultados(vector<int> resultados);

public:
    Interfaz(vector<Pelis>& pelis, Trie& trie);
    void iniciar();
};

#endif //PROYECTO_PROGRA_III_INTERFAZ_H