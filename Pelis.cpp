//
// Created by ITALO on 6/05/2026.
//

#include "Pelis.h"
#include <iostream>


using namespace std;

Pelis::Pelis() {

}

Pelis::Pelis(int fechaCreacion, string titulo, string origen, string director, string cast, string genero, string wikiPage, string plot)
    {

    this->fechaCreacion=fechaCreacion;
    this->titulo=titulo;
    this->origen=origen;
    this->director=director;
    this-> cast = cast;
    this-> genero = genero;
    this-> wikiPage = wikiPage;
    this->plot=plot;
}


int Pelis::getfechaCreacion() {
    return fechaCreacion;
}

string Pelis::getTitulo() {
    return titulo;
}

string Pelis::getOrigen() {
    return origen;
}

string Pelis::getDirector() {
    return director;
}

string Pelis::getCast() {
    return cast;
}

string Pelis::getGenero() {
    return genero;
}

string Pelis::getWikiPage() {
    return wikiPage;
}

string Pelis::getPlot() {
    return plot;
}

void Pelis::mostrarPeli() {
    cout << "\n=========================\n";

    cout << "Titulo: " << titulo << endl;

    cout << "Año: " << fechaCreacion << endl;

    cout << "Origen: " << origen << endl;

    cout << "Director: " << director << endl;

    cout << "Cast: " << cast << endl;

    cout << "Genero: " << genero << endl;

    cout << "Plot: " << plot << endl;

    cout << "Wiki: " << wikiPage << endl;

    cout << "\n=========================\n";
}
