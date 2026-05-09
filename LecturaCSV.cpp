#include "LecturaCSV.h"
#include "Pelis.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

vector<Pelis> cargarPelis(string filename) {

    vector<Pelis> pelis;

    ifstream file(filename);

    if(!file.is_open()) {

        cout << "Error abriendo el archivo." << endl;

        return pelis;
    }

    string line;

    getline(file, line);

    string registroCompleto;

    while(getline(file, line)) {

        if(trim(line).empty()) {
            continue;
        }

        registroCompleto += line;

        if(
            !comillasBalanceadas(
                registroCompleto
            )
        ) {

            registroCompleto += "\n";

            continue;
        }

        vector<string> campos =
            parseCSVLine(
                registroCompleto
            );

        registroCompleto.clear();

        if(campos.size() < 8) {

            cout
                << "Linea invalida detectada."
                << endl;

            continue;
        }

        int year = 0;

        try {

            year = stoi(campos[0]);
        }

        catch(...) {

            year = 0;
        }

        Pelis peli(

            year,
            campos[1],
            campos[2],
            campos[3],
            campos[4],
            campos[5],
            campos[6],
            campos[7]
        );

        pelis.push_back(peli);
    }

    file.close();

    return pelis;
}