# Patrones de diseno — detalle

Este documento amplia la seccion 5 del README: para cada patron, el
problema puntual que se intento resolver, donde vive el codigo, y por
que se eligio ESE patron y no otro.

## 1. Builder — `include/PeliculaBuilder.h`

**Problema:** `Pelicula` tiene 8 campos (id, anio, titulo, origen,
director, cast, genero, wikiPage, plot). Un constructor con 8 parametros
posicionales del mismo tipo (`string`) es una fuente comun de bugs: es
muy facil invertir, por ejemplo, `cast` y `genero` sin que el compilador
avise nada.

**Solucion:** `PeliculaBuilder` expone un metodo por campo
(`conTitulo(...)`, `conDirector(...)`, etc.), cada uno devuelve
`*this` para poder encadenarlos, y `construir()` devuelve el objeto
final. El sitio de la llamada (`LectorCSV::parsearRango`) queda
auto-documentado:

```cpp
Pelicula peli = PeliculaBuilder()
                    .conId(id)
                    .conAnioTexto(campos[0])
                    .conTitulo(campos[1])
                    ...
                    .construir();
```

**Por que no otro patron:** un Factory tendria sentido si hubiera
*distintos tipos* de Pelicula a crear; aqui el problema es puramente de
legibilidad/orden de un objeto con muchos campos, que es exactamente el
caso de uso clasico de Builder.

## 2. Strategy — `EstrategiaRanking` y `EstrategiaSimilitud`

**Problema (ranking):** el enunciado pide "un algoritmo que determine
que pelicula tiene mas importancia en una busqueda". Si esa logica
viviera hardcodeada dentro de `IndiceBusqueda`, cambiarla mas adelante
(por ejemplo, para ponderar tambien por anio o por cantidad de Likes de
otros usuarios) implicaria modificar la clase que ya construye y
consulta los Trie — alto riesgo de romper algo que funciona.

**Solucion:** `IEstrategiaRanking::calcular(MapaRanking)` es la unica
superficie de contacto. La implementacion actual,
`RankingPorPesoAcumulado`, ordena descendente por puntaje acumulado.
`IndiceBusqueda` recibe la estrategia por `shared_ptr` en su
constructor (inyeccion de dependencias) y nunca conoce los detalles.

**Problema (similitud):** mismo razonamiento para "que tan parecidas
son dos peliculas", usado por las recomendaciones. `SimilitudPorGeneroYDirector`
es una implementacion concreta de `IEstrategiaSimilitud`, intercambiable
sin tocar `MotorRecomendaciones`.

**Por que no otro patron:** Template Method serviria si el algoritmo
fuera "casi el mismo con un paso variable"; aqui se preferio Strategy
porque el algoritmo completo cambia de raiz entre implementaciones
posibles, y porque se necesita poder *inyectar* la estrategia desde
afuera (en `main.cpp`) sin herencia.

## 3. Facade — `IndiceBusqueda`

**Problema:** la busqueda real involucra: elegir el shard correcto,
elegir el Trie correcto dentro del shard (texto general vs. tag
especifico), tokenizar la consulta, acumular puntajes de varios shards,
aplicar la estrategia de ranking, y paginar. Si `Interfaz` tuviera que
hacer todo eso, dependeria directamente de `Trie<T>`, de la estructura
interna `ShardIndice` y de los detalles de paralelismo.

**Solucion:** `IndiceBusqueda` expone solo `construir`, `buscarTexto`,
`buscarPorTag`. Toda la complejidad interna (incluida la decision de
*cuantos* shards usar y como repartir el trabajo) queda oculta.

**Por que no otro patron:** no hay "familias de objetos intercambiables"
(eso seria Abstract Factory) ni "un objeto que envuelve a otro objeto
del mismo tipo" (Decorator); es literalmente una fachada simplificada
sobre un subsistema mas complejo, el caso de uso textbook de Facade.

## 4. Singleton — `GestorSesion` (via `Singleton<T>` generico)

**Problema:** los Likes y la lista de Ver-mas-tarde son estado *global*
de la sesion del usuario: cualquier parte del programa que necesite
consultarlos o modificarlos (Interfaz, MotorRecomendaciones a traves del
Observer) debe ver siempre el mismo estado. Pasar una referencia a
traves de 4-5 capas de llamadas es ruidoso y propenso a errores
(¿que pasa si alguien crea una segunda instancia por accidente?).

**Solucion:** `GestorSesion::instancia()` garantiza una unica instancia
durante toda la ejecucion. En vez de escribir el patron a mano otra vez
(constructor privado, `static` local, borrar copia), se extrajo la parte
generica a `Singleton<Derivada>` (CRTP) una sola vez; `GestorSesion`
solo aporta su logica especifica.

**Por que no otro patron:** este es exactamente el problema que
Singleton resuelve (acceso global controlado a una unica instancia); la
alternativa de pasar el `GestorSesion` por parametro a cada funcion que
lo necesite agregaria acoplamiento sin beneficio real en un programa de
consola de un solo usuario.

## 5. Observer — `GestorSesion` (sujeto) + `MotorRecomendaciones` (observador)

**Problema:** cuando el usuario da Like, las recomendaciones deben
recalcularse. Pero `GestorSesion` (que sabe de Likes) no deberia
*conocer* a `MotorRecomendaciones` (que sabe de similitud) — son
responsabilidades distintas, y mezclar esa logica en un solo metodo
("darLike, y de paso recalcular recomendaciones, y de paso quien sabe
que mas en el futuro") crece mal con el tiempo.

**Solucion:** `GestorSesion::darLike` solo notifica a quien implemente
`IObservadorLikes`. `MotorRecomendaciones` se suscribe en `main.cpp`
(`sesion.suscribir(motorRecomendaciones)`), sin que `GestorSesion` sepa
que existe un "motor de recomendaciones".

**Por que no otro patron:** podria pensarse en simplemente llamar a
`recomendaciones.recalcular()` despues de cada `sesion.darLike()` desde
`Interfaz` — pero eso obliga a *recordar* hacerlo en cada lugar donde se
pueda dar un Like, y a que `Interfaz` conozca ambos modulos. Observer
centraliza esa relacion en un solo punto (`main.cpp`, al suscribir) y es
imposible "olvidarse" de notificar.
