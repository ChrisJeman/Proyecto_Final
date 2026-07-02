// Singleton.h
// PATRON: SINGLETON 
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

} 
