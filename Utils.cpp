#include "Utils.h"

#include <algorithm>

using namespace std;

string trim(string s) {

    s.erase(
        0,
        s.find_first_not_of(" \t\r\n")
    );

    s.erase(
        s.find_last_not_of(" \t\r\n") + 1
    );

    return s;
}

bool comillasBalanceadas(const string& texto) {

    int cantidad = 0;

    for(char c : texto) {

        if(c == '"') {
            cantidad++;
        }
    }

    return cantidad % 2 == 0;
}

vector<string> parseCSVLine(const string& line) {

    vector<string> campos;

    string campo;

    bool dentroComillas = false;

    for(char c : line) {

        if(c == '"') {

            dentroComillas =
                !dentroComillas;
        }

        else if(
            c == ',' &&
            !dentroComillas
        ) {

            campos.push_back(
                trim(campo)
            );

            campo.clear();
        }

        else {

            campo += c;
        }
    }

    campos.push_back(
        trim(campo)
    );

    return campos;
}