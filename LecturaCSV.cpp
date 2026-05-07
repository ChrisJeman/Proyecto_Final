#include "LecturaCSV.h"
#include "Pelis.h"

#include <iostream>
#include <fstream>
#include <sstream>
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

    while(getline(file, line)) {

        stringstream ss(line);

        string yearStr;
        string title;
        string origin;
        string director;
        string cast;
        string genre;
        string wikiPage;
        string plot;

        getline(ss, yearStr, ',');
        getline(ss, title, ',');
        getline(ss, origin, ',');
        getline(ss, director, ',');
        getline(ss, cast, ',');
        getline(ss, genre, ',');
        getline(ss, wikiPage, ',');
        getline(ss, plot);

        int year = 0;

        try {

            year = stoi(yearStr);
        }

        catch(...) {

            year = 0;
        }

        Pelis peli(
            year,
            title,
            origin,
            director,
            cast,
            genre,
            wikiPage,
            plot
        );

        pelis.push_back(peli);
    }

    file.close();

    return pelis;
}
