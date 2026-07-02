// Repositorio.h

#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace streaming {

template <typename T>
// Clase generica para persistir un vector de datos en disco.
class Repositorio {
public:
    explicit Repositorio(std::string ruta) : ruta_(std::move(ruta)) {}

    std::vector<T> cargar() const {
        std::vector<T> datos;
        std::ifstream entrada(ruta_);
        if (!entrada.is_open()) {
            return datos; 
        }
        T valor;
        while (entrada >> valor) {
            datos.push_back(valor);
        }
        return datos;
    }

    void guardar(const std::vector<T>& datos) const {
        std::ofstream salida(ruta_, std::ios::trunc);
        for (const T& valor : datos) {
            salida << valor << '\n';
        }
    }

private:
    std::string ruta_;
};

} 
