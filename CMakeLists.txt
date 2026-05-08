cmake_minimum_required(VERSION 4.0)
project(PROYECTO_PROGRA_III)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(ProyectoFinal
        src/main.cpp
        src/LecturaCSV.cpp
        src/Pelis.cpp
        src/Trie.cpp
        src/TrieNode.cpp)