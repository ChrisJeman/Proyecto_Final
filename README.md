# Proyecto Final - Buscador de Películas con Suffix Trie

## Descripción

Este proyecto implementa un buscador de películas utilizando la estructura de datos:

```txt
Suffix Trie
```

El sistema permite realizar búsquedas eficientes sobre:

- títulos
- géneros
- directores
- cast
- plot

a partir de un dataset de películas almacenado en un archivo CSV.

---

# ¿Qué es un Suffix Trie?

Un Suffix Trie almacena todos los sufijos posibles de una palabra.

Ejemplo:

```txt
Batman
```

genera:

```txt
Batman
atman
tman
man
an
n
```

Esto permite realizar búsquedas parciales dentro de palabras.

Por ejemplo:

```txt
man
```

puede encontrar:

```txt
Batman
Superman
Ironman
```

---
# Ventaja del Suffix Trie frente a un Trie normal

Un Trie normal permite buscar palabras usando únicamente prefijos.

Ejemplo:

```txt
Batman
```

permite encontrar:

```txt
Bat
```

pero no:

```txt
man
```

En cambio, el Suffix Trie almacena todos los sufijos posibles de cada palabra:

```txt
Batman
atman
tman
man
an
n
```

Esto permite realizar búsquedas parciales dentro de las palabras, haciendo más flexible y eficiente la búsqueda de coincidencias en títulos de películas.

# Funcionalidades

- Lectura de dataset CSV
- Procesamiento de películas
- Construcción de Suffix Trie
- Búsqueda rápida de coincidencias
- Interfaz de consola interactiva

---

# Mejoras implementadas en el arhivo CSV (base de datos)

El archivo CSV fue mejorado y depurado para soportar correctamente:

- comas dentro de texto
- campos entre comillas
- plots multilínea
- líneas vacías
- limpieza de espacios

Estas mejoras evitan corrupción de datos durante la carga de películas.

---

# Tecnologías utilizadas

- C++20
- CLion
- CMake

---

# Estructura del proyecto

```txt
Proyecto_Final/
│
├── main.cpp
├── CMakeLists.txt
│
├── LecturaCSV.cpp
├── LecturaCSV.h
│
├── Pelis.cpp
├── Pelis.h
│
├── Trie.cpp
├── Trie.h
│
├── TrieNode.cpp
├── TrieNode.h
│
├── Interfaz.cpp
├── Interfaz.h
│
├── Utils.cpp
├── Utils.h
│
└── wiki_movie_plots_deduped.csv
```

---

# Requisitos

Para ejecutar el proyecto se necesita:

- CMake
- Compilador compatible con C++20
- CLion (recomendado)

---

# Configuración del CSV

El archivo:

```txt
wiki_movie_plots_deduped.csv
```

debe colocarse en la raíz del proyecto.

---

# Ejecución

## Desde CLion

1. Abrir el proyecto
2. Reload CMake Project
3. Ejecutar `main.cpp`

---

# Problema común en Windows 11

En algunos casos Windows puede bloquear el ejecutable generado por CLion/CMake mostrando errores como:

```txt
CreateProcess error=4551
Una directiva de Control de aplicaciones bloqueó este archivo
```

Esto ocurre por:

- Smart App Control
- Windows Defender
- Control de aplicaciones de Windows

---

# Solución

## Opción 1

Desactivar temporalmente:

```txt
Seguridad de Windows
→ Control de aplicaciones y navegador
→ Smart App Control
```

---

## Opción 2

Mover el proyecto a una ruta más simple, por ejemplo:

```txt
C:/CLionProjects/ProyectoFinal
```

---

## Opción 3

Eliminar:

```txt
cmake-build-debug
```

y recompilar el proyecto.

---


# Dataset utilizado

El proyecto utiliza el dataset:

```txt
wiki_movie_plots_deduped.csv
```

que contiene información sobre películas, incluyendo:

- título
- año
- director
- cast
- género
- plot
- enlace de Wikipedia

---
