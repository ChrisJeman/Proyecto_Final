#include "Interfaz.h"
#include <algorithm>
#include <limits>

Interfaz::Interfaz(vector<Pelis>& pelis, Trie& trie)
    : pelis(pelis), trie(trie) {}

string Interfaz::convertirMinusculas(string texto) {
    transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
    return texto;
}

void Interfaz::mostrarMenu() {
    cout << "\n========== BUSCADOR DE PELICULAS ==========\n";
    cout << "1. Buscar pelicula\n";
    cout << "2. Mostrar cantidad de peliculas cargadas\n";
    cout << "3. Salir\n";
    cout << "Seleccione una opcion: ";
}

void Interfaz::buscarPelicula() {
    string busqueda;

    cout << "\nIngrese busqueda: ";
    getline(cin, busqueda);

    busqueda = convertirMinusculas(busqueda);

    vector<int> resultados = trie.buscar(busqueda);

    mostrarResultados(resultados);
}

void Interfaz::mostrarResultados(vector<int> resultados) {
    cout << "\n===== RESULTADOS =====\n";

    if (resultados.empty()) {
        cout << "No se encontraron peliculas.\n";
        return;
    }

    int limite = 5;

    for (int i = 0; i < resultados.size() && i < limite; i++) {
        int id = resultados[i];

        if (id >= 0 && id < pelis.size()) {
            pelis[id].mostrarPeli();
        }
    }
}

void Interfaz::iniciar() {
    int opcion;
    
    mostrarMenu();
    
    do {
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nOpcion invalida. Intente nuevamente.\n";
            mostrarMenu();
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1:
                buscarPelicula();
                mostrarMenu();
                break;

            case 2:
                cout << "\nCantidad de peliculas cargadas: " << pelis.size() << endl;
                mostrarMenu();
                break;

            case 3:
                cout << "\nSaliendo del programa...\n";
                break;

            default:
                cout << "\nOpcion invalida. Intente nuevamente.\n";
                mostrarMenu();
                break;
        }

    } while (opcion != 3);
}