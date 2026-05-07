//
// Created by ACER on 7/05/2026.
//

#ifndef PROYECTO_PROGRA_III_PELIS_H
#define PROYECTO_PROGRA_III_PELIS_H

#include <string>
#include <vector>

using namespace std;

class Pelis {

    int fechaCreacion;

    string titulo;

    string origen;

    string director;

    string cast;

    string genero;

    string wikiPage;

    string plot;

public:
    Pelis();

    Pelis(int fechaCreacion, string titulo, string origen, string director, string cast, string genero, string wikiPage, string plot);

    int getfechaCreacion();

    string getTitulo();

    string getOrigen();

    string getDirector();

    string getCast();

    string getGenero();

    string getWikiPage();

    string getPlot();

    void mostrarPeli();

};

#endif //PROYECTO_PROGRA_III_PELIS_H
