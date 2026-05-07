#include <iostream>
#include <vector>

#include "LecturaCSV.h"
#include "PELIS.h"

using namespace std;

int main() {

    vector<Pelis> pelis;

    // Cargar archivo TSV
    pelis = cargarPelis("../Repo_Pelis.tsv");
    // Mostrar cantidad de películas
    cout << "Cantidad de peliculas cargadas: "
         << pelis.size() << endl;

    // Mostrar primeras 5 películas
    cout << "\n===== PELICULAS =====\n" << endl;

    for(int i = 0; i < 5 && i < pelis.size(); i++) {

        pelis[i].mostrarPeli();
    }

    return 0;
}