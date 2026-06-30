// Repositorio.h
//
// PROGRAMACION GENERICA (segundo template del proyecto, ver tambien Trie.h):
// Tanto los ids con Like como los ids en Ver-mas-tarde necesitan guardarse
// y recuperarse de disco entre ejecuciones. En vez de escribir dos pares de
// funciones casi identicas (guardarLikes/cargarLikes, guardarVerMasTarde/
// cargarVerMasTarde), se define un template Repositorio<T> reutilizable
// para cualquier tipo T que sepa leerse/escribirse con los operadores
// estandar de stream (>> y <<) -- en este proyecto, T = IdPelicula (int).
#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace streaming {

template <typename T>
class Repositorio {
public:
    explicit Repositorio(std::string ruta) : ruta_(std::move(ruta)) {}

    std::vector<T> cargar() const {
        std::vector<T> datos;
        std::ifstream entrada(ruta_);
        if (!entrada.is_open()) {
            return datos; // primera ejecucion: todavia no existe el archivo
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

} // namespace streaming
