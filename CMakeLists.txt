cmake_minimum_required(VERSION 3.16)
project(PROYECTO_PROGRA_III)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimizaciones por defecto en Release 
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(include)

find_package(Threads REQUIRED)

file(GLOB SOURCES_APP "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(ProyectoFinal ${SOURCES_APP})
target_link_libraries(ProyectoFinal PRIVATE Threads::Threads)

# Target separado para el benchmark de indexacion secuencial vs paralela

list(REMOVE_ITEM SOURCES_APP "${CMAKE_SOURCE_DIR}/src/main.cpp")
add_executable(benchmark_indexacion benchmarks/benchmark_indexacion.cpp ${SOURCES_APP})
target_link_libraries(benchmark_indexacion PRIVATE Threads::Threads)
