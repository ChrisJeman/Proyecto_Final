#ifndef PROYECTO_PROGRA_III_UTILS_H
#define PROYECTO_PROGRA_III_UTILS_H

#include <string>
#include <vector>

using namespace std;

string trim(string s);

vector<string> parseCSVLine(const string& line);

bool comillasBalanceadas(const string& texto);

#endif