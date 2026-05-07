//
// Created by ITALO on 6/05/2026.
//

#include "LecturaCSV.h"
#include "PELIS.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

vector<Pelis> cargarPelis(string filename) {

    vector<Pelis> pelis;

    ifstream file;

    file.open(filename);

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

        getline(ss, yearStr, '\t');
        getline(ss, title, '\t');
        getline(ss, origin, '\t');
        getline(ss, director, '\t');
        getline(ss, cast, '\t');
        getline(ss, genre, '\t');
        getline(ss, wikiPage, '\t');
        getline(ss, plot, '\t');

        int year = 0;

        try {

            year = stoi(yearStr);

        } catch(...) {

            year = 0;
        }

        Pelis peli(year,title,origin,director,cast,genre,wikiPage,plot);

        pelis.push_back(peli);
    }

    file.close();

    return pelis;
}