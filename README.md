# Plataforma de Streaming — Proyecto Final (Programación III)

Buscador e indexador de un catálogo de ~35,000 películas
con búsqueda por palabra/frase/sub-palabra, búsqueda por Tag (Director,
Cast, Género), Like, Ver más tarde, y recomendaciones por similitud.

## 1. Organizacion del proyecto ("librerias")

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

- Busqueda por palabra, frase y sub-palabra
- Busqueda por Tag: Director, Cast, Genero 
- Paginacion de resultados de 5 en 5
- Seleccionar una pelicula de los resultados para ver su ficha completa.
- Like y Ver-mas-tarde desde la ficha de cualquier pelicula
- Al iniciar el programa, se muestra automaticamente la lista de
  Ver-mas-tarde y las recomendaciones basadas en Likes de la sesion
  anterior (persistidos en `data/likes.txt` y `data/ver_mas_tarde.txt`).
- Recomendaciones: "porque le diste Like a algo similar", recalculadas
  cada vez que se da un nuevo Like.

---

## 4. Algoritmo de busqueda e importancia

Cada palabra del titulo, del plot y de cada Tag se inserta en un Trie con
un peso distinto (`Comun.h`):

El puntaje final de una pelicula es la **suma** de los pesos de todas las
coincidencias (titulo + tags + plot), para todas las palabras de la
consulta. 

**Decision de diseño (eficiencia espacio/tiempo):** la busqueda por
sub-palabra solo se habilita para el **titulo**, indexando todos los 
sufijos de cada palabra del titulo y marcando *todo* el camino del Trie (ver comentario en `Trie.h`, parametro `marcarCamino`). 
Para el **plot** y los **tags** se indexa solo la palabra completa: generar todos los sufijos de cada palabra de
~35,000 sinopsis multiplicaria el tamano del indice varias veces sin aportar tanto valor

---

## 5. Patrones de diseño 

### 1. Builder en `PeliculaBuilder`
`Pelicula` tiene 8 campos. En vez de un constructor posicional de 8
parametros, `PeliculaBuilder` los arma con
metodos nombrados y encadenables. 

### 2. Strategy en `EstrategiaRanking` y `EstrategiaSimilitud`
Dos aplicaciones:
- `IEstrategiaRanking` / `RankingPorPesoAcumulado`: decide el orden de importancia de los resultados de busqueda.
- `IEstrategiaSimilitud` / `SimilitudPorGeneroYDirector`: decide que tan parecidas son dos peliculas 

Ambas interfaces permiten cambiar el algoritmo sin tocar quien las usa (`IndiceBusqueda` / `MotorRecomendaciones`).

### 3. Facade — `IndiceBusqueda`
Por dentro hay hasta 4 `Trie<MapaRanking>` por cada shard de
paralelismo. 

### 4. Singleton — `GestorSesion` (via `Singleton<T>` generico, CRTP)
Solo debe existir un punto de verdad para los Likes/Ver-mas-tarde
durante la ejecucion. En vez de escribir el boilerplate de Singleton a
mano, se definio `Singleton<Derivada>` una sola vez (`Singleton.h`) y
`GestorSesion` simplemente hereda de `Singleton<GestorSesion>`.

### 5. Observer — `GestorSesion` (sujeto) / `MotorRecomendaciones` (observador)
`GestorSesion::darLike` no sabe nada de "recomendaciones": solo notifica
a quien este suscrito (`IObservadorLikes`). `MotorRecomendaciones`
implementa esa interfaz y recalcula sus sugerencias cada vez que recibe
la notificacion. 

---

## 6. Programacion paralela

1. Parseo del CSV: una vez leidos los
   registros de texto (E/S secuencial), convertirlos en objetos
   `Pelicula` 
2. Construccion del indice: el catalogo se reparte en N bloques; 
   cada hilo construye de punta a punta un `ShardIndice` completo (4 
   Trie) usando solo su bloque. 
3. Calculo de recomendaciones: Estilo map-reduce simple — cada hilo calcula el top-5 local de su
   bloque del catalogo (`std::partial_sort`), y solo esas listas
   pequenas se combinan al final.




