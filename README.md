# Plataforma de Streaming — Proyecto Final (Programación III)

Buscador e indexador de un catálogo de ~35,000 películas (dataset
[Wikipedia Movie Plots](https://www.kaggle.com/datasets/jrobischon/wikipedia-movie-plots)),
con búsqueda por palabra/frase/sub-palabra, búsqueda por Tag (Director,
Cast, Género), Like, Ver más tarde, y recomendaciones por similitud.

## 1. Cómo compilar y ejecutar

### Opción A — CMake (recomendado)
```bash
mkdir -p cmake-build && cd cmake-build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j
cd ..
./cmake-build/ProyectoFinal      # IMPORTANTE: ejecutar desde la raiz del
                                  # proyecto, porque la ruta al CSV es
                                  # relativa a data/wiki_movie_plots_deduped.csv
```

### Opción B — VS Code
Usar la tarea `Compilar todo el proyecto` (`.vscode/tasks.json`, ya
actualizada con `-std=c++20 -pthread`) y luego ejecutar `main.exe` desde
la raiz del repositorio.

### Opción C — Linea de comandos directa
```bash
g++ -std=c++20 -O2 -pthread -Iinclude src/*.cpp -o ProyectoFinal
./ProyectoFinal
```

> Requisito: **C++20** y soporte de `<thread>`/`<future>` (`-pthread` en
> Linux/MSYS2; en MSVC no hace falta el flag).

---

## 2. Organizacion del proyecto ("librerias")

```
include/        Headers (.h) — un modulo por responsabilidad
src/            Implementacion (.cpp)
data/           Dataset CSV + archivos de sesion generados en runtime
                (data/likes.txt, data/ver_mas_tarde.txt)
benchmarks/     Programa standalone para medir indexacion secuencial vs paralela
docs/           Pseudocodigo original del equipo + este documento
```

| Modulo | Responsabilidad |
|---|---|
| `Comun.h` | Typedefs (`IdPelicula`, `Puntaje`, `MapaRanking`), `enum class TipoTag`, constantes de peso |
| `Texto.h/.cpp` | `namespace streaming::texto`: normalizar, tokenizar |
| `Pelicula.h/.cpp` | Entidad principal (POO) |
| `PeliculaBuilder.h/.cpp` | Construccion de `Pelicula` (patron **Builder**) |
| `LectorCSV.h/.cpp` | Lectura y parseo **paralelo** del CSV |
| `Trie.h` | Trie **generico** (template) reutilizado por todos los indices |
| `IndiceBusqueda.h/.cpp` | Motor de busqueda (patron **Facade**) + construccion del indice **en paralelo** |
| `EstrategiaRanking.h/.cpp` | Algoritmo de importancia (patron **Strategy**) |
| `EstrategiaSimilitud.h/.cpp` | Algoritmo de similitud para recomendaciones (patron **Strategy**) |
| `Singleton.h` | Base generica CRTP para el patron **Singleton** |
| `Observador.h` | Interfaz del patron **Observer** |
| `GestorSesion.h/.cpp` | Likes / Ver-mas-tarde — **Singleton** + sujeto **Observer** |
| `Repositorio.h` | Persistencia **generica** (template) en archivo de texto |
| `MotorRecomendaciones.h/.cpp` | Observador concreto; calcula similares **en paralelo** |
| `Interfaz.h/.cpp` | Menu de consola (capa de presentacion) |
| `main.cpp` | Orquestacion: carga -> indexa -> sesion -> interfaz |

---

## 3. Funcionalidades cubiertas

- ✅ Busqueda por palabra, frase (varias palabras con semantica OR
  ponderada) y **sub-palabra** (ej. `bar` encuentra *Zanzibar*, *Cabaret*,
  *Embarrassing*, *Disbarred* — no solo palabras que **empiezan** con `bar`).
- ✅ Busqueda por Tag: Director, Cast, Genero (opcion 2 del menu).
- ✅ Paginacion de resultados de 5 en 5.
- ✅ Seleccionar una pelicula de los resultados para ver su ficha completa.
- ✅ Like y Ver-mas-tarde desde la ficha de cualquier pelicula.
- ✅ Al iniciar el programa, se muestra automaticamente la lista de
  Ver-mas-tarde y las recomendaciones basadas en Likes de la sesion
  anterior (persistidos en `data/likes.txt` y `data/ver_mas_tarde.txt`).
- ✅ Recomendaciones: "porque le diste Like a algo similar", recalculadas
  cada vez que se da un nuevo Like.

---

## 4. Algoritmo de busqueda e importancia

Cada palabra del titulo, del plot y de cada Tag se inserta en un Trie con
un peso distinto (`Comun.h`):

| Campo | Peso | Por que |
|---|---|---|
| Titulo | 10 | Es la coincidencia mas relevante para el usuario |
| Tag (director/cast/genero) | 7 | Relevante pero secundaria al titulo |
| Plot (sinopsis) | 3 | La sinopsis es larga; cada palabra individual pesa menos |

El puntaje final de una pelicula es la **suma** de los pesos de todas las
coincidencias (titulo + tags + plot), para todas las palabras de la
consulta. Esto resuelve naturalmente: "busca 'barco fantasma' y muestra
peliculas donde aparezca 'barco' **y/o** 'fantasma'" — cada palabra suma
puntos independientemente, y las peliculas con mas coincidencias /
coincidencias mas importantes quedan primero.

**Decision de diseno (eficiencia espacio/tiempo):** la busqueda por
sub-palabra (ej. `bar` dentro de `embarcar`) solo se habilita para el
**titulo**, indexando todos los sufijos de cada palabra del titulo y
marcando *todo* el camino del Trie (ver comentario en `Trie.h`,
parametro `marcarCamino`). Para el **plot** y los **tags** se indexa
solo la palabra completa: generar todos los sufijos de cada palabra de
~35,000 sinopsis multiplicaria el tamano del indice varias veces sin
aportar tanto valor (raramente se busca un fragmento de una palabra
dentro de la sinopsis). Es un trade-off deliberado, no una limitacion
accidental.

---

## 5. Patrones de diseno (5; el minimo pedido es 4)

### 1. Builder — `PeliculaBuilder`
`Pelicula` tiene 8 campos. En vez de un constructor posicional de 8
parametros (facil de invertir por error), `PeliculaBuilder` los arma con
metodos nombrados y encadenables. Se usa en `LectorCSV::parsearRango`.

### 2. Strategy — `EstrategiaRanking` y `EstrategiaSimilitud`
Dos aplicaciones del mismo patron:
- `IEstrategiaRanking` / `RankingPorPesoAcumulado`: decide el orden de
  importancia de los resultados de busqueda.
- `IEstrategiaSimilitud` / `SimilitudPorGeneroYDirector`: decide que tan
  "parecidas" son dos peliculas (Jaccard sobre generos + bono por mismo
  director), usado por el motor de recomendaciones.

Ambas interfaces permiten cambiar el algoritmo sin tocar quien las usa
(`IndiceBusqueda` / `MotorRecomendaciones`).

### 3. Facade — `IndiceBusqueda`
Por dentro hay hasta 4 `Trie<MapaRanking>` **por cada shard** de
paralelismo (ver seccion 6). `IndiceBusqueda` esconde toda esa
complejidad detras de 3 metodos publicos: `construir`, `buscarTexto`,
`buscarPorTag`.

### 4. Singleton — `GestorSesion` (via `Singleton<T>` generico, CRTP)
Solo debe existir un punto de verdad para los Likes/Ver-mas-tarde
durante la ejecucion. En vez de escribir el boilerplate de Singleton a
mano, se definio `Singleton<Derivada>` una sola vez (`Singleton.h`) y
`GestorSesion` simplemente hereda de `Singleton<GestorSesion>`.

### 5. Observer — `GestorSesion` (sujeto) / `MotorRecomendaciones` (observador)
`GestorSesion::darLike` no sabe nada de "recomendaciones": solo notifica
a quien este suscrito (`IObservadorLikes`). `MotorRecomendaciones`
implementa esa interfaz y recalcula sus sugerencias cada vez que recibe
la notificacion. Agregar a futuro otro modulo que reaccione a un Like
(por ejemplo, estadisticas) no requeriria tocar `GestorSesion`.

*(Documentacion mas detallada de cada patron, con el problema puntual
que resuelve, en `docs/patrones_diseno.md`.)*

---

## 6. Programacion generica

| Template | Donde se usa | Que evita |
|---|---|---|
| `Trie<T>` | `IndiceBusqueda` lo instancia como `Trie<MapaRanking>`, 4 veces por shard | Tener 4 clases de Trie casi-duplicadas |
| `Repositorio<T>` | `GestorSesion` lo usa 2 veces (`Repositorio<int>` para likes y para ver-mas-tarde) | Duplicar guardar/cargar para cada lista |
| `Singleton<Derivada>` (CRTP) | Base de `GestorSesion` | Reescribir el boilerplate de Singleton |

Ademas, `Trie<T>::insertar` es a su vez una plantilla sobre el
**combinador** (la funcion/lambda que decide como fusionar un nuevo
aporte dentro de `T`), igual que hacen los algoritmos de `<algorithm>`
con functores.

---

## 7. Programacion paralela

Hay **tres** subsistemas paralelizados, todos con el mismo principio:
particionar los datos en bloques independientes para que cada hilo
escriba *solo* en su propia memoria (sin mutex, sin condiciones de
carrera por diseño — verificado con ThreadSanitizer, ver mas abajo).

1. **Parseo del CSV** (`LectorCSV::cargar`): una vez leidos los
   registros de texto (E/S secuencial), convertirlos en objetos
   `Pelicula` es CPU-puro e independiente por registro -> se reparte en
   bloques con `std::async`.
2. **Construccion del indice** (`IndiceBusqueda::construir`,
   "sharding"): el catalogo se reparte en N bloques; cada hilo construye
   de punta a punta un `ShardIndice` completo (4 Trie) usando solo su
   bloque. Al buscar, se consulta cada shard y se suman los puntajes
   (la fusion final es trivial comparada con construir el indice).
3. **Calculo de recomendaciones** (`MotorRecomendaciones::recalcular`):
   estilo *map-reduce* simple — cada hilo calcula el top-5 local de su
   bloque del catalogo (`std::partial_sort`), y solo esas listas
   pequenas se combinan al final.

**Verificacion de correccion:** se confirmo que el indice construido con
1 shard y con 8 shards devuelve **resultados identicos** byte a byte
para la misma consulta, y se corrio el proyecto bajo **ThreadSanitizer**
y **AddressSanitizer/UBSan** sin reportar ninguna advertencia.

### Tabla comparativa de tiempos (construccion del indice, 34,886 peliculas)

| Hilos | Tiempo (ms) | Speedup vs. 1 hilo |
|---|---|---|
| 1 | 11272.5 | 1.00x |
| 2 | 13943.0 | 0.81x |
| 4 | 12768.5 | 0.88x |
| 8 | 14810.5 | 0.76x |

**⚠️ Lectura honesta de esta tabla:** estos numeros se midieron en el
entorno de desarrollo usado para preparar esta entrega, que solo expone
**1 nucleo logico** (`std::thread::hardware_concurrency() == 1`). Con un
solo nucleo, lanzar mas hilos no puede dar speedup real — en el mejor
caso, varios hilos comparten el mismo nucleo (time-slicing) y el
overhead de crear/sincronizar hilos hace que sea *mas lento*, exactamente
lo que se ve en la tabla. El codigo paralelo es correcto (ver
verificacion arriba), pero **para reportar un speedup real en la
entrega, hay que volver a correr el benchmark en una maquina con varios
nucleos** (cualquier laptop/PC moderno sirve):

```bash
g++ -std=c++20 -O2 -pthread -Iinclude benchmarks/benchmark_indexacion.cpp \
    src/LectorCSV.cpp src/Pelicula.cpp src/PeliculaBuilder.cpp src/Texto.cpp \
    src/IndiceBusqueda.cpp src/EstrategiaRanking.cpp -o benchmark_indexacion
./benchmark_indexacion
```
(o con CMake, `cmake --build . --target benchmark_indexacion`). Reemplazar
la tabla de arriba con los numeros obtenidos en su maquina antes de
entregar — en una maquina de 4-8 nucleos reales se deberia ver un
speedup notorio (tipicamente 2x-5x con 4-8 hilos) hasta el punto donde
el costo fijo de leer y trocear el CSV empieza a dominar.

---

## 8. Uso destacado de la libreria estandar

`std::async` / `std::future` / `std::thread` (paralelismo), `std::mutex`
+ `std::lock_guard` (sincronizacion minima en `MotorRecomendaciones`),
`std::unordered_map` / `std::unordered_set` (Trie, ranking),
`std::unique_ptr` (RAII — sin `new`/`delete` manual, a diferencia de la
version original), `std::shared_ptr` (estrategias inyectadas, observer),
`std::sort` / `std::partial_sort` / `std::set_intersection` /
`std::set_union` (ranking y similitud), `std::chrono` (medicion de
tiempos), `std::string_view` (sufijos del Trie sin copiar memoria),
`std::function` (selector de Trie por categoria de Tag).

---

## 9. Integrando esto en el repositorio existente

Este directorio reorganiza y completa lo que ya estaba en la rama
`feature/pseudocode2` de `Proyecto_Final`. Para integrarlo:

1. Reemplazar las carpetas `include/` y `src/` actuales por las de aqui
   (los nombres de clase cambiaron levemente: `Pelis` -> `Pelicula`,
   con un campo `id` explicito que antes no existia).
2. Copiar `CMakeLists.txt`, `.vscode/tasks.json` y la carpeta
   `benchmarks/` tal cual.
3. El CSV (`data/wiki_movie_plots_deduped.csv`) es el mismo que ya
   tenian, no cambia.
4. `data/likes.txt` y `data/ver_mas_tarde.txt` se generan solos en la
   primera ejecucion (no hace falta crearlos a mano); conviene agregarlos
   al `.gitignore` si no se quiere versionar el progreso de cada
   integrante.
5. Los pseudocodigos originales (`docs/pseudocode_*.txt`) se mantuvieron
   sin cambios, como referencia historica de la planificacion.
