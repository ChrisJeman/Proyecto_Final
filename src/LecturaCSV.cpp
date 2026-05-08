//
// Created by ITALO on 6/05/2026.
//

#include "LecturaCSV.h"
#include "Pelis.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

static vector<string> splitCSVLine(const string& line) {

    vector<string> fields;
    string field;
    bool inQuotes = false;

    for(size_t i = 0; i < line.size(); i++) {

        char c = line[i];

        if(c == '"') {

            if(inQuotes && i + 1 < line.size() && line[i + 1] == '"') {

                field.push_back('"');
                i++; // skip escaped quote
            }
            else {

                inQuotes = !inQuotes;
            }
        }
        else if(c == ',' && !inQuotes) {

            fields.push_back(field);
            field.clear();
        }
        else {

            field.push_back(c);
        }
    }

    fields.push_back(field);
    return fields;
}

vector<Pelis> cargarPelis(string filename) {

    vector<Pelis> pelis;
    ifstream file(filename);

    if(!file.is_open()) {

        cout << "Error abriendo el archivo." << endl;
        return pelis;
    }

    string line;

    // Skip header
    if(!getline(file, line)) {

        return pelis;
    }

    while(true) {

        if(!getline(file, line)) {
            break;
        }

        string record = line;
        int quoteCount = count(record.begin(), record.end(), '"');

        while((quoteCount % 2) != 0 && getline(file, line)) {

            record += '\n';
            record += line;
            quoteCount += count(line.begin(), line.end(), '"');
        }

        vector<string> fields = splitCSVLine(record);

        if(fields.size() < 8) {
            continue;
        }

        string yearStr = fields[0];
        string title = fields[1];
        string origin = fields[2];
        string director = fields[3];
        string cast = fields[4];
        string genre = fields[5];
        string wikiPage = fields[6];
        string plot = fields[7];

        int year = 0;
        try {
            year = stoi(yearStr);
        }
        catch(...) {
            year = 0;
        }

        Pelis peli(year, title, origin, director, cast, genre, wikiPage, plot);
        pelis.push_back(peli);
    }

    file.close();
    return pelis;
}