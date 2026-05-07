#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "LecturaCSV.h"
#include "Pelis.h"
#include "Trie.h"

using namespace std;

string convertirMinusculas(string texto) {

    transform(
        texto.begin(),
        texto.end(),
        texto.begin(),
        ::tolower
    );

    return texto;
}

vector<string> separarPalabras(string texto) {

    vector<string> palabras;

    stringstream ss(texto);

    string palabra;

    while(ss >> palabra) {

        palabras.push_back(palabra);
    }

    return palabras;
}

int main() {

    vector<Pelis> pelis;

    Trie trie;

    pelis = cargarPelis("wiki_movie_plots_deduped.csv");

    cout << "Cantidad de peliculas cargadas: "
         << pelis.size() << endl;

    for(int i = 0; i < 5000 && i < pelis.size(); i++) {

        vector<string> palabrasTitulo;
        vector<string> palabrasPlot;
        vector<string> palabrasGenero;
        vector<string> palabrasDirector;
        vector<string> palabrasCast;

        palabrasTitulo =
            separarPalabras(
                convertirMinusculas(
                    pelis[i].getTitulo()
                )
            );

        palabrasPlot =
            separarPalabras(
                convertirMinusculas(
                    pelis[i].getPlot()
                )
            );

        palabrasGenero =
            separarPalabras(
                convertirMinusculas(
                    pelis[i].getGenero()
                )
            );

        palabrasDirector =
            separarPalabras(
                convertirMinusculas(
                    pelis[i].getDirector()
                )
            );

        palabrasCast =
            separarPalabras(
                convertirMinusculas(
                    pelis[i].getCast()
                )
            );

        for(string palabra : palabrasTitulo) {

            for(int j = 0; j < palabra.size(); j++) {

                string subpalabra =
                    palabra.substr(j);

                trie.insertar(subpalabra, i);
            }
        }

        for(string palabra : palabrasPlot) {

            trie.insertar(palabra, i);
        }

        for(string palabra : palabrasGenero) {

            trie.insertar(palabra, i);
        }

        for(string palabra : palabrasDirector) {

            trie.insertar(palabra, i);
        }

        for(string palabra : palabrasCast) {

            trie.insertar(palabra, i);
        }
    }

    string busqueda;

    cout << "\nIngrese busqueda: ";

    getline(cin, busqueda);

    busqueda = convertirMinusculas(busqueda);

    vector<int> resultados;

    resultados = trie.buscar(busqueda);

    cout << "\n===== RESULTADOS =====\n";

    if(resultados.empty()) {

        cout << "No se encontraron peliculas.\n";
    }

    else {

        int limite = 5;

        for(int i = 0;
            i < resultados.size() && i < limite;
            i++) {

            int id = resultados[i];

            pelis[id].mostrarPeli();
        }
    }

    return 0;
}
