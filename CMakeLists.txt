cmake_minimum_required(VERSION 4.0)
project(PROYECTO_PROGRA_III)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProyectoFinal main.cpp
        LecturaCSV.cpp
        LecturaCSV.h
        Pelis.cpp
        Pelis.h
        Trie.cpp
        Trie.h
        TrieNode.cpp
        TrieNode.h
        Interfaz.h
        Interfaz.cpp
        Utils.h
        Utils.cpp)
