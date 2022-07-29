#include "la_sombra_de_mac.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define ERROR -1
#define PRIMER_EJECUTABLE 0
#define SEGUNDO_EJECUTABLE 1
#define TERCER_EJECUTABLE 2
#define CUARTO_EJECUTABLE 3

#define FORMATO_LECTURA "%[^;];%i;%i;%i;%[^\n]\n"
#define FORMATO_ESCRITURA "%s;%i;%i;%i;%s\n"
#define GANO_LA_PARTIDA "Si"
#define PERDIO_LA_PARTIDA "No"
#define AGREGAR_UNA_PARTIDA_AL_ARCHIVO "agregar_partida"
#define ELIMINAR_PARTIDAS_AL_ARCHIVO "eliminar_partida"
#define ORDENAR_EL_ARCHIVO_DE_PARTIDAS "ordenar_partidas"
#define ARCHIVO_AUXILIAR_DE_PARTIDAS "archivo_aux.csv"
#define NOMBRE_DEL_ARCHIVO "nombre_archivo"
#define NOMBRE_DEL_JUGADOR "nombre_jugador"
#define CONFIGURACIONES_DEL_JUEGO "config_juego"
#define POZOS "POZOS"
#define VELAS "VELAS"
#define INTERRUPTORES "INTERRUPTORES"
#define PORTALES "PORTALES"
#define ESCALERAS "ESCALERAS"
#define MONEDAS "MONEDAS"
#define LLAVES "LLAVES"

const int NIVELES_ACTUALES = 3;
const int CANTIDAD_DE_LEIDOS = 5;
const int CANTIDAD_DE_LEIDOS_CONFIG = 1;
const int VIDAS_MAXIMAS_ACTUALES = 3;

//Informara al usuario como utilizar las funciones del codigo 
void cargar_informacion_de_los_archivos(char ejecutable[MAX_NOMBRE]){
    printf("\n----------NO SE PUDO EJECUTAR BIEN EL PROGRAMA, SIGA LOS SIGUENTES PASOS----------\n");
    printf("  Si quiere %s hagalo de esta forma:\n", AGREGAR_UNA_PARTIDA_AL_ARCHIVO);
    printf("       %s %s <<%s>> \n\n", ejecutable, AGREGAR_UNA_PARTIDA_AL_ARCHIVO, NOMBRE_DEL_ARCHIVO);
    printf("  Si quiere %s hagalo de esta forma:\n", ELIMINAR_PARTIDAS_AL_ARCHIVO);
    printf("       %s %s %s <<%s>>\n\n", ejecutable, ELIMINAR_PARTIDAS_AL_ARCHIVO, NOMBRE_DEL_ARCHIVO, NOMBRE_DEL_JUGADOR);
    printf("  Si quiere %s hagalo de esta forma:\n",ORDENAR_EL_ARCHIVO_DE_PARTIDAS);
    printf("       %s %s <<%s>>\n\n", ejecutable, ORDENAR_EL_ARCHIVO_DE_PARTIDAS, NOMBRE_DEL_ARCHIVO);
    printf("  Si quiere ingresar al Juego hagalo de esta forma:\n");
    printf("       %s %s \n\n", ejecutable, CONFIGURACIONES_DEL_JUEGO);
    printf("  Si quiere ingresar al Juego con las cantidades originales:\n");
    printf("       %s \n",ejecutable);
}

// Cierra el archivo_original cuando no se pudo abrir el archivo_aux.
void cierre_de_archivo_aux(FILE* archivo_original, FILE* archivo_auxiliar){
    perror("El archivo no pudo abrirse.\n");
    fclose(archivo_original);
}

// Muestra en pantalla cuando el archivo_original no se pudo abrir.
void cierre_de_archivo_original(FILE* archivo_original, char ejecutable[MAX_NOMBRE]){
    perror("El archivo que ingreso no pudo abrirse.\nVuelva a intentar.\n");
    cargar_informacion_de_los_archivos(ejecutable);
}

//recibe un entero
//Post: Si el valor resibido en gano_la_partida es igual a GANO, el booleano  sera TRUE, de lo contrario FALSE.
void resultado_de_partidas(bool *gano, int gano_la_partida){
    if(gano_la_partida == GANO){
        *gano = true;
    }else{
        *gano = false;
    }
}

//Procedimiento que se encargara de pedirle al usuario los valores necesarios para agregarlos al archivo csv.
void pedir_partida_nueva(partida_t *partida){
    int valor_ingresado;
    printf("Cual es tu nombre jugador? (Primera letra en mayuscula y sin espacios, utilice ' _ ')\n");
    scanf("%s", partida->jugador);
    printf("Cual es el nivel que llegaste?(El juego actual tiene %i niveles)\n", NIVELES_ACTUALES);
    scanf("%i", &partida->nivel_llegado);
    printf("Cuantos fueron tus puntos?\n");
    scanf("%i", &partida->puntos);
    printf("Cuantas fueron tus vidas restantes? ( El maximo de vidas son %i)\n", VIDAS_MAXIMAS_ACTUALES);
    scanf("%i", &partida->vidas_restantes);
    printf("Usted gano la partida?(ingrese 1 si la gano, ingrese 0 si perdio la partida)\n");
    scanf("%i", &valor_ingresado);
    resultado_de_partidas(&partida->gano, valor_ingresado);
}

//Si el booleano viene como TRUE, gano_la_partida_aux sera GANO_LA_PARTIDA, de lo contrario sera PERDIO_LA_PARTIDA
void ganadores_de_partidas(bool gano, char gano_la_partida_aux[MAX_GANADAS]){
    if(gano == true){
        strcpy(gano_la_partida_aux, GANO_LA_PARTIDA);
    }else{
        strcpy(gano_la_partida_aux, PERDIO_LA_PARTIDA);
    }
}

// Los archivos tienen que estar abiertos, tanto como el original y el auxiliar,
// Recibira una tipo de partida_t ya inicializado.
// Post: Agregara la partida_t que recibe, a mi archivo auxiliar, uniendolo con mi archivo original, compara los nombres y lo ubicara en su respectivo lugar
//       y los mantendra en order alfabetico. 
void agregar_partida(FILE* archivo_original, FILE* archivo_auxiliar, partida_t partida_a_agregar){
    partida_t partida;
    char gano_la_partida[MAX_GANADAS];
    char gano_la_partida_aux[MAX_GANADAS];
    bool insertado = false;
    int leidos = fscanf(archivo_original, FORMATO_LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, &partida.vidas_restantes, gano_la_partida);
    while(leidos == CANTIDAD_DE_LEIDOS){
        if(strcmp(partida.jugador, partida_a_agregar.jugador) >= 0 && !insertado){
            ganadores_de_partidas(partida_a_agregar.gano, gano_la_partida_aux);
            fprintf(archivo_auxiliar, FORMATO_ESCRITURA, partida_a_agregar.jugador, partida_a_agregar.nivel_llegado,
                        partida_a_agregar.puntos, partida_a_agregar.vidas_restantes, gano_la_partida_aux);
            insertado = true;
        }
        fprintf(archivo_auxiliar, FORMATO_ESCRITURA, partida.jugador, partida.nivel_llegado, partida.puntos, 
                        partida.vidas_restantes, gano_la_partida);
        leidos = fscanf(archivo_original, FORMATO_LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, 
                        &partida.vidas_restantes, gano_la_partida);
    }
    printf("Se agrego la partida correctamente al archivo\n");
}

// Los archivos tienen que estar abiertos, tanto como el original y el auxiliar,
// Recibira un NOMBRE de un jugador que se quiera eliminar.
// Post: En el archivo auxiliar se eliminara el nombre ingresado, y se mantendra en orden alfabetico todo el archivo.
void eliminar_partida(FILE* archivo_original, FILE* archivo_auxiliar, char jugador_a_eliminar[MAX_NOMBRE]){
    partida_t partida;
    char gano_la_partida[MAX_GANADAS];
    int leidos = fscanf(archivo_original, FORMATO_LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, &partida.vidas_restantes, gano_la_partida);
    while(leidos == CANTIDAD_DE_LEIDOS){
        if(strcmp(jugador_a_eliminar, partida.jugador) != 0){
            fprintf(archivo_auxiliar, FORMATO_ESCRITURA, partida.jugador, partida.nivel_llegado, partida.puntos, partida.vidas_restantes, gano_la_partida);
        }
        leidos = fscanf(archivo_original, FORMATO_LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, 
                        &partida.vidas_restantes, gano_la_partida);
    }
    printf("Se elimino el jugador del archivo\n");
}

// El archivo original tiene que estar abierto.
// Leera el archivo para guardar cada linea en partidas[MAX_PARTIDAS] y sumara uno al tope por cada partida leida. 
void leer_partidas(FILE* archivo_original, partida_t partidas[MAX_PARTIDAS], int *tope){
    int leidos;
    char gano_la_partida[MAX_GANADAS];
    int i = 0;
    bool final_del_archivo = false;
    while(final_del_archivo == false){
        leidos = fscanf(archivo_original, FORMATO_LECTURA, partidas[i].jugador, &partidas[i].nivel_llegado, &partidas[i].puntos, 
                                                    &partidas[i].vidas_restantes, gano_la_partida);       
        if(strcmp(gano_la_partida, GANO_LA_PARTIDA) == 0){
            partidas[i].gano = true;
        }
        else{
            partidas[i].gano = false;
        }
        if(leidos == CANTIDAD_DE_LEIDOS){
            (*tope)++;
            i++;
        }        
        if(leidos != CANTIDAD_DE_LEIDOS){
            final_del_archivo = true;
        }
    }
}

// El archivo tiene que estar abierto y el tope inicializado y distinto de 0
// Ordenara las partidas dentro del archivo en order alfabetico. 
void ordenar_partidas(FILE* archivo_auxiliar, partida_t partidas[MAX_PARTIDAS], int tope){
    int minimo;
    partida_t partida_aux;
    char gano_la_partida[MAX_PARTIDAS];   
    for(int i = 0; i < tope -1; i++){
        minimo = i;
        for(int j = i+1; j < tope; j++){
            if(strcmp(partidas[j].jugador, partidas[minimo].jugador) < 0){
                minimo = j;
            }
        }
        partida_aux = partidas[i];
        partidas[i] = partidas[minimo];
        partidas[minimo] = partida_aux;
        ganadores_de_partidas(partidas[i].gano, gano_la_partida);
        fprintf(archivo_auxiliar, FORMATO_ESCRITURA, partidas[i].jugador, partidas[i].nivel_llegado, partidas[i].puntos, 
                        partidas[i].vidas_restantes, gano_la_partida);
    }
    printf("El archivo se ordeno de forma correcta\n");
}

// Compara el elemento con las herramientas y guarda el valor en el nivel que le corresponda
void inicializar_configuraciones_herramientas(char* elemento, elementos_config_t *nivel_ingresado, char* valor){
    if(strcmp(elemento, ESCALERAS) == 0){
        nivel_ingresado->escaleras = atoi(valor);
    }
    if(strcmp(elemento, MONEDAS) == 0){
        nivel_ingresado->monedas = atoi(valor);
    }
    if(strcmp(elemento, LLAVES) == 0){
        nivel_ingresado->llaves = atoi(valor);
    }
}

// Compara el elemento con los obstaculos, y guarda el valor en el nivel que le corresponda.
void inicializar_configuraciones_obstaculos(char* elemento, elementos_config_t *nivel_ingresado, char* valor){
    if(strcmp(elemento, POZOS) == 0){
        nivel_ingresado->pozos = atoi(valor);
    }
    if(strcmp(elemento, VELAS) == 0){
        nivel_ingresado->velas = atoi(valor);
    }
    if(strcmp(elemento, INTERRUPTORES) == 0){
        nivel_ingresado->interruptores = atoi(valor);
    }
    if(strcmp(elemento, PORTALES) == 0){
        nivel_ingresado->portales = atoi(valor);
    }
}

// El archivo tiene que estar abierto 
// Leera cada linea del archivo y guardara su cantidad en su respectivo nivel. 
void inicializar_cantidades_configuraciones(FILE* archivo_original,elementos_config_t *primer_nivel,elementos_config_t *segundo_nivel, elementos_config_t *tercer_nivel){
    char linea[MAX_LINEA];
    int leidos = fscanf(archivo_original, "%s\n", linea);
    char* nivel = strtok(linea, "N%s_");
    int nivel_leido = atoi(nivel);
    char* elemento = strtok(NULL, "=");
    char* valor = strtok(NULL,"\0");
    while(leidos == CANTIDAD_DE_LEIDOS_CONFIG){
        if(nivel_leido == PRIMER_NIVEL){
            inicializar_configuraciones_obstaculos(elemento, primer_nivel, valor);
            inicializar_configuraciones_herramientas(elemento, primer_nivel, valor);
        }
        if(nivel_leido == SEGUNDO_NIVEL){
            inicializar_configuraciones_obstaculos(elemento, segundo_nivel, valor);
            inicializar_configuraciones_herramientas(elemento, segundo_nivel, valor);
        }
        if(nivel_leido == TERCER_NIVEL){
            inicializar_configuraciones_obstaculos(elemento, tercer_nivel, valor);
            inicializar_configuraciones_herramientas(elemento, tercer_nivel, valor);
        }
        leidos = fscanf(archivo_original, "%s\n", linea);
        nivel = strtok(linea, "N%s_");
        nivel_leido = atoi(nivel);
        elemento = strtok(NULL, "=");
        valor = strtok(NULL,"\0");
    }
}

// Juego inicializado
// Mostrara por pantalla el juego y toda su informacion.
void jugando_juego(juego_t juego){
    srand ((unsigned)time(NULL)); 
    int estado_del_juego = 0;
    int estado_del_nivel = 0;
    while(estado_del_juego == JUGANDO){
        system("clear");   
        imprimir_informacion_de_personajes(juego);
        if(estado_del_nivel == GANO){
            cargar_proximo_nivel(&juego);
        }
        imprimir_terreno(juego);
        realizar_jugada(&juego);
        estado_del_juego = estado_juego(juego);
        estado_del_nivel = estado_nivel(juego);
        }
    mostrar_estado_finalizado(estado_del_juego);  
}

int main(int argc, char* argv[]){
    niveles_t nivel_config;
    partida_t partidas[MAX_PARTIDAS];
    partida_t partida;
    juego_t juego;
    int tope;
    if(argc > SEGUNDO_EJECUTABLE){
        if(strcmp(argv[SEGUNDO_EJECUTABLE], AGREGAR_UNA_PARTIDA_AL_ARCHIVO) == 0){
            FILE* archivo_original = fopen(argv[TERCER_EJECUTABLE], "r");
            if(!archivo_original){
                cierre_de_archivo_original(archivo_original, argv[PRIMER_EJECUTABLE]);
                return ERROR;
            }
            FILE* archivo_auxiliar = fopen(ARCHIVO_AUXILIAR_DE_PARTIDAS, "w");
            if(!archivo_auxiliar){
                cierre_de_archivo_aux(archivo_original, archivo_auxiliar);
                return ERROR;     
            }
            pedir_partida_nueva(&partida);
            agregar_partida(archivo_original, archivo_auxiliar, partida);
        
            rename(ARCHIVO_AUXILIAR_DE_PARTIDAS,argv[TERCER_EJECUTABLE]);
            fclose(archivo_original);
            fclose(archivo_auxiliar);
        }
        else if(strcmp(argv[SEGUNDO_EJECUTABLE],ELIMINAR_PARTIDAS_AL_ARCHIVO) == 0){
            FILE* archivo_original = fopen(argv[TERCER_EJECUTABLE], "r");
            if(!archivo_original){
                cierre_de_archivo_original(archivo_original, argv[PRIMER_EJECUTABLE]);
                return ERROR;
            }
            FILE* archivo_auxiliar = fopen(ARCHIVO_AUXILIAR_DE_PARTIDAS, "w");
            if(!archivo_auxiliar){
                cierre_de_archivo_aux(archivo_original, archivo_auxiliar);
                return ERROR;     
            }
            eliminar_partida(archivo_original, archivo_auxiliar, argv[CUARTO_EJECUTABLE]);
            rename(ARCHIVO_AUXILIAR_DE_PARTIDAS,argv[TERCER_EJECUTABLE]);
            fclose(archivo_original);
            fclose(archivo_auxiliar);
        }
        else if(strcmp(argv[SEGUNDO_EJECUTABLE], ORDENAR_EL_ARCHIVO_DE_PARTIDAS) == 0){
            FILE* archivo_original = fopen(argv[TERCER_EJECUTABLE], "r");
            if(!archivo_original){
                cierre_de_archivo_original(archivo_original, argv[PRIMER_EJECUTABLE]);
                return ERROR;
            }
            FILE* archivo_auxiliar = fopen(ARCHIVO_AUXILIAR_DE_PARTIDAS, "w");
            if(!archivo_auxiliar){
                cierre_de_archivo_aux(archivo_original, archivo_auxiliar);
                return ERROR;      
            }      
            leer_partidas(archivo_original, partidas, &tope);
            ordenar_partidas(archivo_auxiliar,  partidas, tope);
            rename(ARCHIVO_AUXILIAR_DE_PARTIDAS,argv[TERCER_EJECUTABLE]);
            fclose(archivo_original);
            fclose(archivo_auxiliar);
        }
        else if(strcmp(argv[SEGUNDO_EJECUTABLE], CONFIGURACIONES_DEL_JUEGO) == 0){
            FILE* archivo_config = fopen(argv[SEGUNDO_EJECUTABLE], "r");
            if(!archivo_config){
                inicializar_juego(&juego);
                jugando_juego(juego);
            }
            inicializar_cantidades_configuraciones(archivo_config, &nivel_config.primer_nivel, &nivel_config.segundo_nivel, &nivel_config.tercer_nivel);
            inicializar_juego_configuraciones(&juego, nivel_config.primer_nivel, nivel_config.segundo_nivel, nivel_config.tercer_nivel);
            jugando_juego(juego);
            fclose(archivo_config);    
        }
        else{
            cargar_informacion_de_los_archivos(argv[PRIMER_EJECUTABLE]);
        }
    }
    else{
        inicializar_juego(&juego);
        jugando_juego(juego);
    }

    return 0;
}