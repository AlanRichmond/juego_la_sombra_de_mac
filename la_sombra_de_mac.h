#ifndef __LA_SOMBRA_DE_MAC__
#define __LA_SOMBRA_DE_MAC__

#include <stdbool.h>

#define PRIMER_NIVEL 1
#define SEGUNDO_NIVEL 2
#define TERCER_NIVEL 3

#define JUGANDO 0
#define GANO 1
#define PERDIO -1

#define MAX_FILAS 20
#define MAX_COLUMNAS 25
#define MAX_ELEMENTOS 500
#define MAX_NIVELES 3
#define MAX_PAREDES 200
#define MAX_NOMBRE 100
#define MAX_GANADAS 10
#define MAX_PARTIDAS 100
#define MAX_LINEA 100

typedef struct coordenada {
    int fila;
    int col;
}coordenada_t;

typedef struct personaje {
    coordenada_t posicion;
    int vida;
    int puntos;
    bool tiene_llave;
    bool interruptor_apretado;
}personaje_t;

typedef struct sombra {
    coordenada_t posicion;
    bool esta_viva;
}sombra_t;

typedef struct elemento {
    char tipo;
    coordenada_t coordenada;
}elemento_t;

typedef struct nivel {
    int numero_nivel;
    coordenada_t paredes[MAX_PAREDES];
    int tope_paredes;
    elemento_t obstaculos[MAX_ELEMENTOS];
    int tope_obstaculos;
    elemento_t herramientas[MAX_ELEMENTOS];
    int tope_herramientas;
}nivel_t;

typedef struct juego {
    personaje_t personaje;
    sombra_t sombra;
    nivel_t niveles[MAX_NIVELES];
    int nivel_actual;
}juego_t;

typedef struct partida{
    char jugador[MAX_NOMBRE];
    int nivel_llegado;
    int puntos;
    int vidas_restantes;
    bool gano;
}partida_t;

typedef struct elementos_config{
    int escaleras;
    int pozos;
    int interruptores;
    int portales;
    int llaves;
    int monedas;
    int velas;
}elementos_config_t;

typedef struct niveles{
    elementos_config_t primer_nivel;
    elementos_config_t segundo_nivel;
    elementos_config_t tercer_nivel;
}niveles_t;

/*
 * Procedimiento que recibe el juego e imprime toda su información por pantalla.
 */
void imprimir_terreno(juego_t juego);

/* 
 *Inicializará el juego, cargando toda la información inicial, los datos del personaje, y los 3 niveles.
 */
void inicializar_juego(juego_t* juego);

/*
 * Recibe un juego con todas sus estructuras válidas.
 *
 * El juego se dará por ganado, si terminó todos los niveles. O perdido, si el personaje queda
 * sin vida. 
 * Devolverá:
 * -> 0 si el estado es jugando. 
 * -> -1 si el estado es perdido.
 * -> 1 si el estado es ganado.
 */
int estado_juego(juego_t juego);

/*
 * El nivel se dará por terminado, si ambos personajes pasaron por la puerta teniendo la 
 * llave correspondiente.
 * Devolverá:
 * -> 0 si el estado es jugando.
 * -> 1 si el estado es ganado.
 */
int estado_nivel(juego_t juego);

/*
 * Moverá el personaje, y realizará la acción necesaria en caso de chocar con un elemento
 */
void realizar_jugada(juego_t* juego);

//Inicializará el juego, cargando toda la información inicial, los datos del personaje, y los 3 niveles con las cantidades de cada elemento_config.
void inicializar_juego_configuraciones(juego_t* juego, elementos_config_t primer_nivel, elementos_config_t segundo_nivel, elementos_config_t tercer_nivel);

//post: Carga por pantalla todo acerca de los personajes
void imprimir_informacion_de_personajes(juego_t juego);

//carga la informacion necesaria para jugar el proximo nivel
void cargar_proximo_nivel(juego_t *juego);

//Muestra por pantalla el estado del juego cuando termina, si gano o perdio.
void mostrar_estado_finalizado(int estado_juego);

#endif