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
- Se utiliza lógica de sufijos para los Títulos, permitiendo búsquedas parciales. Al insertar todos los sufijos de los títulos de las películas, el sistema puede encontrar coincidencias en cualquier posición de la palabra (por ejemplo buscamos "man" y encontram+os "Batman").
- Se utiliza lógica de Trie estándar para el Plot y Tags para optimizar el uso de la memoria RAM. El plot y los tags se indexan como palabras completas para evitar el crecimiento exponencial de nodos.
## Instrucciones para abrir y ejecutar

### Paso 1: Clonar repositorio
Con git clone -b feature/pseudocode2 https://github.com/ChrisJeman/Proyecto_Final.git
### Paso 2: Compilar
Utiliza Ctrl + Shift + P, Run Task y luego compilar todo el proyecto
### Paso 3: Ejecutar
Desde el proyecto ya puedes ejecutar el ./main.exe generado

