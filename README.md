# Proyecto Final

Este proyecto implementa un sistema de búsqueda de películas usando estructuras de datos en C++ como **Trie**, además de lectura de archivos CSV.


## Estructura del proyecto
- `src/` → Código fuente en C++
- `include/` → Archivos de cabecera
- `data/` → Dataset `wiki_movie_plots_deduped.csv`
- `.vscode/` → Configuración de tareas para compilar
- `main.exe` → Ejecutable generado
## Estructura de datos
Para garantizar una búsqueda de complejidad $O(m)$ ($m$ es la longitud de la palabra), hemos implementado:
Suffix Trie (Mixto): 
- Se utiliza lógica de sufijos para los Títulos, permitiendo búsquedas parciales.
- Se utiliza lógica de Trie estándar para el Plot y Tags para optimizar el uso de memoria RAM.
- Unordered Maps: Utilizados en cada nodo del árbol para gestionar los hijos y el ranking de importancia de manera eficiente.
## Instrucciones para abrir y ejecutar

### Paso 1: Clonar repositorio
Con git clone -b feature/pseudocode2 https://github.com/ChrisJeman/Proyecto_Final.git
### Paso 2: Compilar
Utiliza Ctrl + Shift + P, Run Task y luego compilar todo el proyecto
### Paso 3: Ejecutar
Desde el proyecto ya puedes ejecutar el ./main.exe generado

