// Singleton.h
//
// PATRON DE DISENO: SINGLETON + PROGRAMACION GENERICA
// En vez de escribir manualmente el boilerplate de Singleton (instancia
// estatica, constructor privado, borrar copia) cada vez que se necesite,
// se define UNA SOLA VEZ como clase base generica (CRTP: la clase
// derivada se pasa a si misma como parametro de plantilla). Cualquier
// clase del proyecto que necesite ser unica en toda la ejecucion (aqui,
// GestorSesion) simplemente hereda de Singleton<SuPropioTipo>.
#pragma once

namespace streaming {

template <typename Derivada>
class Singleton {
public:
    static Derivada& instancia() {
        static Derivada unica;
        return unica;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
};

} // namespace streaming
