#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "la_sombra_de_mac.h"
#include "utiles.h"

const char ARRIBA = 'W';
const char DERECHA = 'D';
const char ABAJO = 'S';
const char IZQUIERDA = 'A';
const char COMPRAR_VIDA = 'V';

const char SOMBRA = 'S';
const char MAC = 'M';
const char LLAVE = 'L';
const char PUERTA = 'D';
const char ESCALERAS = 'E';
const char MONEDA = 'C';
const char POZOS = 'W';
const char INTERRUPTORES = 'O';
const char PORTAL = 'P';
const char VELAS = 'V';
const char PARED = '@';
const char VACIO = '-';
const char VIDA = 'V';
const int VIDAS_MAC = 3;
const int PUNTOS_MAC = 0;
const int MINIMO_DE_LA_MATRIZ = 0;

const int VELAS_NIVEL_1 = 5;
const int POZOS_NIVEL_1 = 15;
const int PORTALES_NIVEL_1 = 0;
const int INTERRUPTOR_NIVEL_1 = 1;
const int MONEDAS_NIVEL_1 = 10;
const int ESCALERAS_NIVEL_1 = 10;

const int VELAS_NIVEL_2 = 10;
const int POZOS_NIVEL_2 = 20;
const int PORTALES_NIVEL_2 = 2;
const int INTERRUPTOR_NIVEL_2 = 2;
const int MONEDAS_NIVEL_2 = 15;
const int ESCALERAS_NIVEL_2 = 15;

const int VELAS_NIVEL_3 = 12;
const int POZOS_NIVEL_3 = 30;
const int PORTALES_NIVEL_3 = 4;
const int INTERRUPTOR_NIVEL_3 = 4;
const int ESCALERAS_NIVEL_3 = 15;
const int MONEDAS_NIVEL_3 = 15;

const int PUERTA_DE_LOS_NIVELES = 1;
const int CON_LLAVE = 1;
const int SIN_LLAVE = 0;
const int MINIMO_MONEDA = 10;
const int MAXIMO_MONEDA = 10;
const int COSTO_REVIVIR_SOMBRA = 50;
const int VALOR_DE_VIDA = 200;
const int VUELVE_DOS_POSICIONES = 2;
const int VUELVE_TRES_POSICIONES = 3;
const int DESCONTAR_VIDA = 1;
const int SUMAR_UNA_VIDA = 1;
const int MAXIMO_DE_VIDAS = 3;
const int RANGO_MANHATTAN = 1;

// Función que recibe dos coordenadas, devuelve true si las
// coordenadas son iguales, sino false.
bool coordenadas_iguales(coordenada_t a, coordenada_t b){
	return ((a.fila == b.fila) && (a.col == b.col));
}

// Procedimiento que dejará la estructura personaje_t en un
// estado válido para iniciar el juego.
// 'arranque_personaje' es un parámetro que representa la posición 
// en la arrancará el personaje.
void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje){
	ref_personaje->vida = VIDAS_MAC;
    ref_personaje->puntos = PUNTOS_MAC;
    ref_personaje->tiene_llave = false;
    ref_personaje->interruptor_apretado = false;
    ref_personaje->posicion.fila = arranque_personaje.fila; 
    ref_personaje->posicion.col = arranque_personaje.col;    
}

// Procedimiento que dejará una estructura elemento_t que
// represente a la sombra, en un estado válido para iniciar el juego. 
// 'arranque_sombra' es un parámetro que representa la posición 
// en la arrancará la sombra.
void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){	
	ref_sombra->esta_viva = true;
	ref_sombra->posicion.fila = arranque_sombra.fila;
    ref_sombra->posicion.col = arranque_sombra.col;
}

// La función recibe un caracter y devuelve true
// si el movimiento recibido concuerda con la convención
// propuesta, sino false.
// -> W: Si el personaje debe moverse para la arriba. 
// -> A: Si el personaje debe moverse para la izquierda.
// -> S: Si el personaje debe moverse para la abajo.
// -> D: Si el personaje debe moverse para la derecha.
// -> V: Si el personaje obstedra una vida, si no tiene mas de MAXIMO_DE_VIDAS, y tiene que tener un minimo de VALOR_DE_VIDA puntos
bool es_movimiento_valido(char movimiento){
	return( 
		movimiento == DERECHA || 
	    movimiento == IZQUIERDA || 
	    movimiento == ARRIBA || 		
	    movimiento == ABAJO ||
	    movimiento == VIDA
	);
}

//post: Carga por pantalla caracteres validos que puede ingresar el usuario.
void cargar_opciones_de_movimiento(){
	printf("Cual es el movimiento que quieres hacer?       (solo en mayusculas, maximo 3 vidas y costo 200 puntos)\n OPCIONES:\n ARRIBA = '%c'\n ABAJO = '%c'\n DERECHA = '%c'\n IZQUIERDA = '%c'\n SI QUIERES COMPRAR UNA VIDA INGRESE = '%c'\n", ARRIBA, ABAJO, DERECHA, IZQUIERDA, VIDA);
}

// Procedimineto que se encargará de pedirle al usuario
// que ingrese un movimiento hasta que sea válido.
void pedir_movimiento(char* ref_movimiento){
    cargar_opciones_de_movimiento();
    scanf(" %c", ref_movimiento);     
    while(es_movimiento_valido(* ref_movimiento) == false){
    	system("clear");
    	printf("ESE NO ES UN MOVIMIENTO VALIDO\n");
    	cargar_opciones_de_movimiento();
        scanf(" %c", ref_movimiento);
    }
    printf("tu movimiento es el siguiente: %c. \n", *ref_movimiento);
}

// Recibe el vector de paredes dentro del nivel_t y tambien una variable de tipo coordenada_t que representa la posicion que se encuentran los personajes
// El tope inicializado, mayor a 0.
// post: Compara la coordenada con las coordenadas del vector de las parades dentro del struct de nivel_t, 
//       devuelve falso si no se intersecan con las paredes de lo contrario devolvera verdadero.
bool se_intersecan_con_paredes(nivel_t nivel_paredes, coordenada_t coordenada, int tope_paredes){
	bool devuelve = false;
	for(int i = 0; i < tope_paredes; i++){
		if(coordenadas_iguales(coordenada, nivel_paredes.paredes[i]) == true){	
			devuelve = true;
		}
	}
	return devuelve;
}

// Las ESCALERAS tienen que estar inicializadas correctamente sobre las PAREDES
// El tope inicializado y mayor a 0  
// Post: Eliminara las ESCALERAS del vector, si mac se posiciona sombre ellas, y quedara PARED en la posicion que habia ESCALERAS
void eliminar_escaleras(personaje_t *personaje, elemento_t herramientas[MAX_ELEMENTOS], int *tope_herramientas){
	for(int i = 0; i < *tope_herramientas; i++){
		if(herramientas[i].tipo == ESCALERAS){
			if(coordenadas_iguales(personaje->posicion, herramientas[i].coordenada) == true){     
		        herramientas[i] = herramientas[*tope_herramientas -1];
	    	    (*tope_herramientas)--; 			    
		    }
		}
	}
}
//Las ESCALERAS tienen que estar inicializadas correctamente sobre PAREDES
//El tope inicializado y mayor a 0.
//Post: Devuelve true si las coordenadas del personaje coincide con las coordenadas de las ESCALERAS, de lo contrario false.
bool interseccion_con_escaleras(personaje_t *personaje, elemento_t herramientas[MAX_ELEMENTOS], int tope_herramientas){
	bool devuelve = false;
	for(int i = 0; i < tope_herramientas; i++){
		if(herramientas[i].tipo == ESCALERAS){
			if(coordenadas_iguales(personaje->posicion, herramientas[i].coordenada) == true){
				devuelve = true;
		    }
	    } 
    }
    return devuelve;
}

// El procedimiento recibe un caracter valido ingresado por el usuario y un juego ya inicializado
//post: Verifica cuando el personaje se posiciona sombre una escalera, si los personajes chocan contra una pared no se mueven, si no se chocan contra un MAX_FILAS o MAX_COLUMNAS
//      el personaje avanzara una posicion, de lo contrario se quedada en el lugar
void mover_arriba(juego_t *juego, char movimiento){
	if((juego->personaje.posicion.fila > MINIMO_DE_LA_MATRIZ)){
		juego->personaje.posicion.fila--;
		if(interseccion_con_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas) == true && 
			                                    juego->personaje.posicion.fila > MINIMO_DE_LA_MATRIZ){			
			eliminar_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
			juego->personaje.posicion.fila--;
			if(interseccion_con_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas) == true && 
				                                juego->personaje.posicion.fila > MINIMO_DE_LA_MATRIZ){
				eliminar_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
		    	juego->personaje.posicion.fila--;
		    	if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			        juego->personaje.posicion.fila = juego->personaje.posicion.fila + VUELVE_TRES_POSICIONES;
		        }
		    }
		    if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
				juego->personaje.posicion.fila = juego->personaje.posicion.fila + VUELVE_DOS_POSICIONES;
			}
		}
		else if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->personaje.posicion.fila++;
		}
	}
	if((juego->sombra.posicion.fila > MINIMO_DE_LA_MATRIZ) && juego->sombra.esta_viva == true){
		juego->sombra.posicion.fila--;
		if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->sombra.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->sombra.posicion.fila++;
		}
	}
}

// El procedimiento recibe un caracter valido ingresado por el usuario y un juego ya inicializado
//post: Verifica cuando el personaje se posiciona sombre una escalera, si los personajes chocan contra una pared no se mueven, si no se chocan contra un MAX_FILAS O MAX_COLUMNAS
//      el personaje avanzara una posicion, de lo contrario se quedada en el lugar
void mover_abajo(juego_t *juego, char movimiento){
	if((juego->personaje.posicion.fila < MAX_FILAS-1)){
		juego->personaje.posicion.fila++;
		if(interseccion_con_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas) == true &&
		                                        juego->personaje.posicion.fila < MAX_FILAS-1){
			eliminar_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
			juego->personaje.posicion.fila++;
			if(interseccion_con_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas) == true &&
				                                juego->personaje.posicion.fila < MAX_FILAS-1){
		    	eliminar_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
		    	juego->personaje.posicion.fila++;
		    	if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			         juego->personaje.posicion.fila = juego->personaje.posicion.fila - VUELVE_TRES_POSICIONES;
		        }
		    }
			if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			    juego->personaje.posicion.fila = juego->personaje.posicion.fila - VUELVE_DOS_POSICIONES;
		    }
		}
		else if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->personaje.posicion.fila--;
		}
	}
	if((juego->sombra.posicion.fila < MAX_FILAS-1) && juego->sombra.esta_viva == true){
		juego->sombra.posicion.fila++;
		if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->sombra.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->sombra.posicion.fila--;
		}
	}
}	

// El procedimiento recibe un caracter valido ingresado por el usuario y un juego ya inicializado
//post: Verifica cuando el personaje se posiciona sombre una escalera, si los personajes chocan contra una pared no se mueven, si no se chocan contra un MAX_FILAS O MAX_COLUMNAS
//      el personaje avanzara una posicion, de lo contrario se quedada en el lugar.
//      El personaje se movera para la derecha y la sombra a su inversa, Si el personaje tiene el INTEERRUPTOR_APRETADO en true, cambiara la forma de mover a sombra igual que el personaje,
//      De lo contrario se volvera a mover en su inversa.
void mover_derecha(juego_t *juego, char movimiento){
	if((juego->personaje.posicion.col < MAX_COLUMNAS-1)){
	    juego->personaje.posicion.col++;
		if(interseccion_con_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas) == true && 
			                                    juego->personaje.posicion.col < MAX_COLUMNAS-1){
			eliminar_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
			juego->personaje.posicion.col++;
			if(interseccion_con_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas) == true && 
				                                juego->personaje.posicion.col < MAX_COLUMNAS-1){
		    	eliminar_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
		    	juego->personaje.posicion.col++;
		    	if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			         juego->personaje.posicion.col = juego->personaje.posicion.col - VUELVE_TRES_POSICIONES;
		        }
		    }
			if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			    juego->personaje.posicion.col = juego->personaje.posicion.col - VUELVE_DOS_POSICIONES;
		    }
		}
		else if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->personaje.posicion.col--;
		}
	}
	if((juego->sombra.posicion.col > MINIMO_DE_LA_MATRIZ) && (juego->personaje.interruptor_apretado == false) && juego->sombra.esta_viva == true){
		juego->sombra.posicion.col--;
		if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->sombra.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->sombra.posicion.col++;
		}
	}
	if((juego->sombra.posicion.col < MAX_COLUMNAS-1) && (juego->personaje.interruptor_apretado == true) && juego->sombra.esta_viva == true){
		juego->sombra.posicion.col++;
		if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->sombra.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->sombra.posicion.col--;
		}		
	}
}

// El procedimiento recibe un caracter valido ingresado por el usuario y un juego ya inicializado
//post: Verifica cuando el personaje se posiciona sombre una escalera, si los personajes chocan contra una pared no se mueven, si no se chocan contra un MAX_FILAS O MAX_COLUMNAS
//      el personaje avanzara una posicion, de lo contrario se quedada en el lugar.
//      El personaje se movera para la izquierda y la sombra a su inversa, Si el personaje tiene el INTEERRUPTOR_APRETADO en true, cambiara la forma de mover a sombra igual que el personaje,
//      De lo contrario se volvera a mover en su inversa. 
void mover_izquierda(juego_t *juego, char movimiento){	
	if((juego->personaje.posicion.col > MINIMO_DE_LA_MATRIZ)){
	    juego->personaje.posicion.col--;
		if(interseccion_con_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas) == true && 
			                            juego->personaje.posicion.col > MINIMO_DE_LA_MATRIZ){
			eliminar_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
			juego->personaje.posicion.col--;
			if(interseccion_con_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas) == true &&
			                            juego->personaje.posicion.col > MINIMO_DE_LA_MATRIZ){
		    	eliminar_escaleras(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
		    	juego->personaje.posicion.col--;
		    	if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			         juego->personaje.posicion.col = juego->personaje.posicion.col + VUELVE_TRES_POSICIONES;
		        }
		    }
			if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			    juego->personaje.posicion.col = juego->personaje.posicion.col + VUELVE_DOS_POSICIONES;
		    }
		}
		else if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->personaje.posicion.col++;
		}
	}
	if((juego->sombra.posicion.col < MAX_COLUMNAS-1) && juego->personaje.interruptor_apretado == false && juego->sombra.esta_viva == true){
		juego->sombra.posicion.col++;
		if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->sombra.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->sombra.posicion.col--;
		}
	}
	if((juego->sombra.posicion.col > MINIMO_DE_LA_MATRIZ) && juego->personaje.interruptor_apretado == true && juego->sombra.esta_viva == true){
		juego->sombra.posicion.col--;
		if(se_intersecan_con_paredes(juego->niveles[juego->nivel_actual-1],juego->sombra.posicion, juego->niveles[juego->nivel_actual-1].tope_paredes) == true){
			juego->sombra.posicion.col++;
		}		
	}
}

//Recibe un caracter valido, y un struct personaje_t y dentro sus vidas y sus puntos.
//Post: Si mac tiene menos de MAXIMO_DE_VIDAS, y mas o igual a VALOR_DE_VIDA se le sumara una vida al struct de personaje_t y se le restaran los puntos 
//        usados al struct personaje_t, de lo contrario no se le sumaran VIDAS ni restaran PUNTOS al struct.
void recuperar_una_vida(personaje_t *personaje, char movimiento){
	if(personaje->vida < MAXIMO_DE_VIDAS){
		if(personaje->puntos >= VALOR_DE_VIDA){
		    personaje->vida = personaje->vida + SUMAR_UNA_VIDA;
		    personaje->puntos = personaje->puntos - VALOR_DE_VIDA;
	    }
	}
}

// El procedimiento recibe el personaje y la sombra por referencia y el movimiento,
// y según este último, los mueve acordemente.
// El personaje se mueve de la forma:
// -> W: Si el personaje debe moverse para la arriba. 
// -> A: Si el personaje debe moverse para la izquierda.
// -> S: Si el personaje debe moverse para la abajo.
// -> D: Si el personaje debe moverse para la derecha.
// Si el personaje tiene menos de MAXIMO_DE_VIDAS y puntos COSTO_DE_VIDA necesarios.
// -> V: Si el personaje recupera una vida.
//   La sombra se mueve de igual forma para el eje vertical, pero
//   para el eje horizontal se moverá de forma inversa:
// -> A: Si el personaje debe moverse para la derecha.
// -> D: Si el personaje debe moverse para la izquierda.
void mover_personaje(juego_t* juego, char movimiento){
	if(movimiento == ARRIBA){
		mover_arriba(juego, movimiento);
	}
	if(movimiento == ABAJO){
		mover_abajo(juego, movimiento);
	}
	if(movimiento == DERECHA){
		mover_derecha(juego, movimiento);
	}	
	if(movimiento == IZQUIERDA){
		mover_izquierda(juego, movimiento);
    }
    if(movimiento == COMPRAR_VIDA){
    	recuperar_una_vida(&juego->personaje, movimiento);
    }
}

// Llega toda la estructura del juego validado, y sus paredes ya validadas
//post: Recorre el vector de paredes y asigna una posicion a cada pared al terreno.
void asignar_paredes(juego_t juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
	for(int i = 0; i < juego.niveles[juego.nivel_actual-1].tope_paredes; i++){ 
		terreno[juego.niveles[juego.nivel_actual-1].paredes[i].fila][juego.niveles[juego.nivel_actual-1].paredes[i].col] = PARED;
	}
}

// Llega toda la estructura del juego validado, con sus obstaculos inicializados
//post:  Recorre el vector de obstaculos y asigna cada obstaculo al terreno.
void asignar_obstaculos(juego_t juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
	for(int i = 0; i < juego.niveles[juego.nivel_actual-1].tope_obstaculos; i++){
		if(juego.niveles[juego.nivel_actual-1].obstaculos[i].tipo == POZOS)
			terreno[juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.col] = POZOS;
		else if(juego.niveles[juego.nivel_actual-1].obstaculos[i].tipo == VELAS)
			terreno[juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.col] = VELAS;
		else if(juego.niveles[juego.nivel_actual-1].obstaculos[i].tipo == INTERRUPTORES)
			terreno[juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.col] = INTERRUPTORES;
	    else if(juego.niveles[juego.nivel_actual-1].obstaculos[i].tipo == PORTAL)
	    	terreno[juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.col] = PORTAL;
	}
}

// Llega toda la estructura del juego validado, con sus herramientas inicializadas
//post: Recorre el vector de herramientas y asigna cada herramienta al terreno.
void asignar_herramientas(juego_t juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
	for(int i = 0; i < juego.niveles[juego.nivel_actual-1].tope_herramientas; i++){		
		if(juego.niveles[juego.nivel_actual-1].herramientas[i].tipo == ESCALERAS)
			terreno[juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.col] = ESCALERAS;
		else if(juego.niveles[juego.nivel_actual-1].herramientas[i].tipo == MONEDA)
			terreno[juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.col] = MONEDA;
		else if(juego.niveles[juego.nivel_actual-1].herramientas[i].tipo == PUERTA)
			terreno[juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.col] = PUERTA;
		else if(juego.niveles[juego.nivel_actual-1].herramientas[i].tipo == LLAVE)
			terreno[juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.col] = LLAVE;
    }
}

// Procedimiento que recibe un terreno (una matriz) con sus maximos.
//post: Rellena la matriz con caracteres VACIO.
void rellenar_matriz(char terreno[MAX_FILAS][MAX_COLUMNAS]){
	for(int i = 0; i < MAX_FILAS; i++){
		for(int j = 0; j < MAX_COLUMNAS; j++){
			terreno[i][j] = VACIO;
		}
	}
}

// Procedimiento que recibe una matriz (un terreno) rellena con VACIO.
//post: Imprime el terreno por pantalla relleno.
void mostrar_terreno(char terreno[MAX_FILAS][MAX_COLUMNAS]){	
	for(int i = 0; i < MAX_FILAS; i++){
		for(int j = 0; j < MAX_COLUMNAS; j++){
			printf(" %c ", terreno[i][j]);
		}
		printf("\n");
	}
}

// Recibe los personajes validados dentro de juego, y su terreno
//post: Asigna la ubicacion de los personajes dentro del terreno
void asignar_personajes(juego_t juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
	terreno[juego.personaje.posicion.fila][juego.personaje.posicion.col] = MAC;
	terreno[juego.sombra.posicion.fila][juego.sombra.posicion.col] = SOMBRA;
}

void imprimir_terreno(juego_t juego){
	char terreno[MAX_FILAS][MAX_COLUMNAS];
	rellenar_matriz(terreno);
	asignar_paredes(juego, terreno);
	asignar_obstaculos(juego, terreno);
	asignar_herramientas(juego, terreno);
	asignar_personajes(juego, terreno);
	mostrar_terreno(terreno);
}
  
// Recibe una variable de tipo coordenada_t puntero.
//post: Devuelve una coordenada random se encuentre dentro del rango de la matriz.
void asignar_coordenada_aleatoria(coordenada_t *coordenada){
	coordenada->fila = rand() % MAX_FILAS;
	coordenada->col = rand() % MAX_COLUMNAS;
}

// Recibe en tipo elemento_t y una tipo de variable coordenada_t.
// El tope inicializado y mayor a 0
//post: Compara la variable coordenada_t con el vector de elementos, devolvera verdadero si no se intersecan con ninguno elemento del vector,
//      de lo contrario devolvera falso.
int buscar_elemento(elemento_t elemento[MAX_ELEMENTOS], coordenada_t coordenada, int tope){
	bool devuelve = 0;
	for(int i = 0; i < tope; i++){
		if(coordenadas_iguales(coordenada, elemento[i].coordenada) == true){
			devuelve = 1;	
		}
	}
	return devuelve;
}

// Recibe el juego y los tipos de datos anteriormente validados del vector de herramientas.
//post: Asigna una coordenada aleatoria valida a cada tipo de herramienta fijandose que no se superpongan los distintos campos(paredes, herramientas y obstaculos).
void inicializar_herramientas_segun_su_nivel(nivel_t *nivel, int numero_nivel, int cantidad_escaleras, int cantidad_monedas,int cantidad_puerta, int cantidad_llave){
	coordenada_t escaleras, moneda, llave, puerta;
	int sumatoria = cantidad_monedas + cantidad_escaleras;
	int sumatoria_dos = sumatoria + cantidad_puerta;
	int sumatoria_total = sumatoria_dos + cantidad_llave;
	int i;
	for(i = 0; i < cantidad_escaleras; i++){	
		asignar_coordenada_aleatoria(&escaleras);
	    while( se_intersecan_con_paredes(*nivel, escaleras, nivel->tope_paredes) == false){
	        asignar_coordenada_aleatoria(&escaleras);
        }
	    nivel->herramientas[i].coordenada = escaleras;
	}	
	for(i = cantidad_escaleras; i < sumatoria; i++){
		asignar_coordenada_aleatoria(&moneda);
        while(buscar_elemento(nivel->obstaculos, moneda, nivel->tope_obstaculos) == 1 || buscar_elemento(nivel->herramientas, moneda, nivel->tope_herramientas) == 1 || 
        	(se_intersecan_con_paredes(*nivel, moneda, nivel->tope_paredes) == true)){
	        asignar_coordenada_aleatoria(&moneda);
        }
        nivel->herramientas[i].coordenada = moneda;
	}
    for(i = sumatoria; i < sumatoria_dos; i++){
		asignar_coordenada_aleatoria(&puerta);
	    while(buscar_elemento(nivel->obstaculos, puerta, nivel->tope_obstaculos) == 1 || buscar_elemento(nivel->herramientas, puerta, nivel->tope_herramientas) == 1 || 
	    	(se_intersecan_con_paredes(*nivel, puerta, nivel->tope_paredes) == true)){
	        asignar_coordenada_aleatoria(&puerta);
	    }
        nivel->herramientas[i].coordenada = puerta;
   	}
	if(numero_nivel > PRIMER_NIVEL){
		for(i = sumatoria_dos; i < sumatoria_total; i++){
		    asignar_coordenada_aleatoria(&llave);
	        while(buscar_elemento(nivel->obstaculos, llave, nivel->tope_obstaculos) == 1 || buscar_elemento(nivel->herramientas, llave, nivel->tope_herramientas) == 1 || 
	    	    (se_intersecan_con_paredes(*nivel, llave, nivel->tope_paredes) == true)){
	            asignar_coordenada_aleatoria(&llave);
            }
            nivel->herramientas[i].coordenada = llave;
	    }
    } 
}

// Recibe el juego, el vector de obstaculos anteriormente validado y las cantidades de cada obstaculo. 
//post: Asigna una coordenada aleatoria valida a cada tipo de obstaculo fijandose que no se superpongan los distintos campos(paredes, herramientas y obstaculos).
void inicializar_obstaculos_segun_su_nivel(nivel_t *nivel, int cantidad_pozos, int cantidad_velas, int cantidad_interruptor, int cantidad_portales){
    coordenada_t interruptor, pozos, velas, portal;
    int sumatoria = cantidad_pozos + cantidad_velas;
    int sumatoria_dos = sumatoria + cantidad_interruptor;
    int sumatoria_total = sumatoria_dos + cantidad_portales;
	int i;
	for(i = 0; i < cantidad_pozos; i++){
		asignar_coordenada_aleatoria(&pozos);
	    while(buscar_elemento(nivel->obstaculos, pozos, nivel->tope_obstaculos) == 1 || buscar_elemento(nivel->herramientas, pozos, nivel->tope_herramientas) == 1 || 
	    	(se_intersecan_con_paredes(*nivel, pozos, nivel->tope_paredes) == true)){
		    asignar_coordenada_aleatoria(&pozos);
	    }
	    nivel->obstaculos[i].coordenada = pozos;				
	}
	for(i = cantidad_pozos; i < sumatoria; i++){
		asignar_coordenada_aleatoria(&velas);  
	    while(buscar_elemento(nivel->obstaculos, velas, nivel->tope_obstaculos) == 1 || buscar_elemento(nivel->herramientas, velas, nivel->tope_herramientas) == 1 || 
	    	(se_intersecan_con_paredes(*nivel, velas, nivel->tope_paredes) == true)){
		    asignar_coordenada_aleatoria(&velas);
	    }
	    nivel->obstaculos[i].coordenada = velas;
	}
	for(i = sumatoria; i < sumatoria_dos; i++){
		asignar_coordenada_aleatoria(&interruptor);
	    while(buscar_elemento(nivel->obstaculos, interruptor, nivel->tope_obstaculos) == 1 || buscar_elemento(nivel->herramientas, interruptor, nivel->tope_herramientas) == 1 || 
	    	(se_intersecan_con_paredes(*nivel, interruptor, nivel->tope_paredes) == true)){
		    asignar_coordenada_aleatoria(&interruptor); 
	    }
	    nivel->obstaculos[i].coordenada = interruptor;
	}
	for(i = sumatoria_dos; i < sumatoria_total; i++){
		asignar_coordenada_aleatoria(&portal);
	    while(buscar_elemento(nivel->obstaculos, portal, nivel->tope_obstaculos) == 1 || buscar_elemento(nivel->herramientas, portal, nivel->tope_herramientas) == 1  ||
	        (se_intersecan_con_paredes(*nivel, portal, nivel->tope_paredes) == true)){
		    asignar_coordenada_aleatoria(&portal);
	    }
	    nivel->obstaculos[i].coordenada = portal;
	}	
}

// Recibe los tipo de datos del vector de obstaculos.
//post: Ingresa los datos al vector de obstaculos y inicializa el tope.
void inicializa_vector_obstaculos(nivel_t *nivel, int cantidad_pozos, int cantidad_velas, int cantidad_interruptor, int cantidad_portales){	
	nivel->tope_obstaculos = cantidad_pozos + cantidad_velas + cantidad_interruptor + cantidad_portales;	
	int sumatoria = cantidad_pozos + cantidad_velas;
	int sumatoria_dos = sumatoria + cantidad_interruptor;
	int sumatoria_total = sumatoria_dos + cantidad_portales;
	int i;
	for(i = 0; i < cantidad_pozos; i++){
		nivel->obstaculos[i].tipo = POZOS;
	}	
	for(i = cantidad_pozos; i < sumatoria; i++){
		nivel->obstaculos[i].tipo = VELAS;
	}
	for(i = sumatoria; i < sumatoria_dos; i++){
		nivel->obstaculos[i].tipo = INTERRUPTORES;
	}
	for(i = sumatoria_dos; i < sumatoria_total; i++){
		nivel->obstaculos[i].tipo = PORTAL;
	} 	
}

// Recibe los tipo de datos del vector de herramientas y sus cantidades
//post: Ingresa los datos al vector de herramientas y inicializa el tope
void inicializa_vector_herramientas(nivel_t *nivel, int cantidad_escaleras, int cantidad_monedas, int cantidad_puerta, int cantidad_llave){
	nivel->tope_herramientas = cantidad_escaleras + cantidad_monedas + cantidad_llave + cantidad_puerta;	
	int sumatoria = cantidad_escaleras + cantidad_monedas;
	int sumatoria_dos = sumatoria + cantidad_puerta;
	int sumatoria_total = sumatoria_dos + cantidad_llave;
	int i;
	for(i = 0; i < cantidad_escaleras; i++){
		nivel->herramientas[i].tipo = ESCALERAS;
	}
	for(i = cantidad_escaleras; i < sumatoria; i++){
		nivel->herramientas[i].tipo = MONEDA;
	} 
	for(i = sumatoria; i < sumatoria_dos; i++){
		nivel->herramientas[i].tipo = PUERTA;
	} 
	for(i = sumatoria_dos; i < sumatoria_total; i++){
		nivel->herramientas[i].tipo = LLAVE;
	} 	
}

//post: Devuelve un entero que se utilizara para la coordenada columna de sombra.
int columna_de_sombra(coordenada_t arranque_personaje){
	return((MAX_COLUMNAS -1) - arranque_personaje.col);
}

//Recibe el juego y las cantidades de los objetos y herramientas.
//post: Inicializa los vectores dependiendo el nivel y los topes para herramientas y para obstaculos.
void inicializa_vectores_segun_nivel(nivel_t *nivel, int numero_nivel, int cantidad_pozos, int cantidad_velas, int cantidad_interruptor,
                            int cantidad_portales, int cantidad_monedas, int cantidad_escaleras, int cantidad_puerta, int cantidad_llave){			
	inicializa_vector_obstaculos(&(*nivel), cantidad_pozos, cantidad_velas, cantidad_interruptor, cantidad_portales);
	inicializa_vector_herramientas(&(*nivel), cantidad_escaleras, cantidad_monedas, cantidad_puerta, cantidad_llave);
}

// Recibe los personajes dentro de juego.
// Procedimiento que dejará a los personajes en un estado válido para iniciar el juego.
// post: Asigna coordendas correctas para los personajes, sin que se superpongan con las paredes, ni obstaculos y herramientas. 
void inicializa_los_personajes(juego_t *juego){	
    bool coordenadas_asignadas = false;
    coordenada_t arranque_personaje;
    coordenada_t arranque_sombra;
    while(coordenadas_asignadas == false){
        asignar_coordenada_aleatoria(&arranque_personaje);
	    while(se_intersecan_con_paredes((juego->niveles[juego->nivel_actual-1]), arranque_personaje, juego->niveles[juego->nivel_actual-1].tope_paredes) == true || 
	    	buscar_elemento(juego->niveles[juego->nivel_actual-1].obstaculos, arranque_personaje, juego->niveles[juego->nivel_actual-1].tope_obstaculos) == 1 || 
	    	buscar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, arranque_personaje, juego->niveles[juego->nivel_actual-1].tope_herramientas) == 1 ){
		        asignar_coordenada_aleatoria(&arranque_personaje);
	    }
	    juego->personaje.posicion = arranque_personaje;
        arranque_sombra.fila = juego->personaje.posicion.fila;
        arranque_sombra.col = columna_de_sombra(juego->personaje.posicion);       
        if(se_intersecan_con_paredes((juego->niveles[juego->nivel_actual-1]), arranque_sombra, juego->niveles[juego->nivel_actual-1].tope_paredes) == false && 
            buscar_elemento(juego->niveles[juego->nivel_actual-1].obstaculos, arranque_sombra, juego->niveles[juego->nivel_actual-1].tope_obstaculos) != 1 && 
            buscar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, arranque_sombra, juego->niveles[juego->nivel_actual-1].tope_herramientas) != 1 ){
		        juego->sombra.posicion = arranque_sombra;
		        coordenadas_asignadas = true;
	    }
    }
}

// Recibe el juego, el numero del nivel y todas las cantidades de ese nivel
//post: Deja al nivel en un estado para poder iniciar el juego, inicializa los niveles completos
void inicializar_nivel(nivel_t* nivel, int numero_nivel, int cantidad_pozos, int cantidad_velas, int cantidad_interruptor, 
	                                    int cantidad_portales, int cantidad_monedas,int cantidad_escaleras,int cantidad_puerta, int cantidad_llave){  
    obtener_mapa(nivel->paredes, &(nivel->tope_paredes));
	inicializa_vectores_segun_nivel(&(*nivel), numero_nivel, cantidad_pozos, cantidad_velas, cantidad_interruptor, cantidad_portales, cantidad_monedas, 
		                                cantidad_escaleras, cantidad_puerta, cantidad_llave);  
    inicializar_obstaculos_segun_su_nivel(&(*nivel), cantidad_pozos, cantidad_velas, cantidad_interruptor, cantidad_portales);
    inicializar_herramientas_segun_su_nivel(&(*nivel), numero_nivel, cantidad_escaleras, cantidad_monedas, cantidad_puerta, cantidad_llave);
}

void inicializar_juego(juego_t* juego){
	cargar_mapas();
    juego->nivel_actual = PRIMER_NIVEL;
	inicializar_nivel(&(juego->niveles[PRIMER_NIVEL-1]), PRIMER_NIVEL, POZOS_NIVEL_1, VELAS_NIVEL_1, INTERRUPTOR_NIVEL_1,
	                                    PORTALES_NIVEL_1, MONEDAS_NIVEL_1, ESCALERAS_NIVEL_1, PUERTA_DE_LOS_NIVELES, SIN_LLAVE);    
    inicializar_nivel(&(juego->niveles[SEGUNDO_NIVEL-1]), SEGUNDO_NIVEL, POZOS_NIVEL_2, VELAS_NIVEL_2, INTERRUPTOR_NIVEL_2, 
    	                                PORTALES_NIVEL_2, MONEDAS_NIVEL_2, ESCALERAS_NIVEL_2, PUERTA_DE_LOS_NIVELES, CON_LLAVE);
	inicializar_nivel(&(juego->niveles[TERCER_NIVEL-1]), TERCER_NIVEL, POZOS_NIVEL_3, VELAS_NIVEL_3, INTERRUPTOR_NIVEL_3 , 
		                                PORTALES_NIVEL_3, MONEDAS_NIVEL_3, ESCALERAS_NIVEL_3, PUERTA_DE_LOS_NIVELES, CON_LLAVE);
	inicializa_los_personajes(&(*juego));
	inicializar_personaje(&juego->personaje, (juego->personaje.posicion));
	inicializar_sombra(&juego->sombra, (juego->sombra.posicion));
}

//post: Devuelve un valor entre MINIMO_MONEDA y MAXIMO_MONEDA
int valor_de_moneda(int valor_maximo, int valor_minimo){
	return rand() % (valor_maximo+1) + valor_minimo;
}

// Recibe un struct de personaje_t y uno de sombra_t con sus coordenadas validas, ambos personajes vivos.
// El tope inicializado y mayor a 0.
//post: Si los personajes se posicionan por encima de la herramienta MONEDA se le sumara puntos al personaje, un valor entre MINIMO_MONEDA y MAXIMO_MONEDA puntos.
//      Se eliminara del vector de herramientas las monedas utilizadas por los personajes, y se le restara uno al tope, por cada moneda usada.
void manejar_interseccion_personajes_monedas(personaje_t *personaje, sombra_t *sombra, elemento_t herramientas[MAX_ELEMENTOS], int *tope_herramientas){
	for(int i = 0; i < *tope_herramientas; i++){
		if(herramientas[i].tipo == MONEDA){
			if((coordenadas_iguales(personaje->posicion, herramientas[i].coordenada) == true) ||
			   (coordenadas_iguales(sombra->posicion, herramientas[i].coordenada) == true)){
	    		    personaje->puntos = personaje->puntos + valor_de_moneda(MAXIMO_MONEDA, MINIMO_MONEDA);	   
	    	    	herramientas[i] = herramientas[*tope_herramientas -1];
	    	    	(*tope_herramientas)--;
			}
		}
	}
}

// Recibe el juego inicializado, dentro un struct de personaje_t, donde la VIDA tiene que ser mayor o igual a 1 
// Tope inicializado y mayor a 0
//post: Si PERSONAJE se posiciona por encima de un POZO se le restara una vida y se le asignara una nueva coordenada valida para ambos personajes.
void manejar_interseccion_personaje_pozos(juego_t *juego, int tope_obstaculos){
	for(int i = 0; i < tope_obstaculos; i++){
		if(juego->niveles[juego->nivel_actual-1].obstaculos[i].tipo == POZOS){
			if(coordenadas_iguales(juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].obstaculos[i].coordenada) == true){	
	    		    inicializa_los_personajes(&(*juego));
	                juego->personaje.vida = juego->personaje.vida - DESCONTAR_VIDA;
		    }
	    }
    }
}

//Recibe un struct de personaje y uno de sombra, con sus coordenadas validas, el personaje y la sombra ambos vivos.
// Tope inicializado y mayor a 0.
//post: Si sombra se posiciona por encima del obstaculo(PORTAL), se le asignaran las coordenadas de mac, y a mac las coordenadas de sombra.
void manejar_interseccion_sombra_portales(sombra_t *sombra, personaje_t *personaje, elemento_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos){
	coordenada_t posicion_de_mac;
	for(int i = 0; i < tope_obstaculos; i++){
		if(obstaculos[i].tipo == PORTAL){
			if(coordenadas_iguales(sombra->posicion, obstaculos[i].coordenada) == true){			
		            posicion_de_mac = personaje->posicion;
			        personaje->posicion = sombra->posicion;
			        sombra->posicion = posicion_de_mac;
		    }
	    }
	}
}

// Recibe un personaje inicializado, con el booleano TIENE_LLAVE en false que esta dentro del struct personaje_t, y el personaje VIVO
// Tope inicializado y mayor a 0.
//post: Cambia el booleano del struct del personaje, TIENE_LLAVE a true y elimina el elemento LLAVE del vector de herramientas y disminuye el tope.
//      Deja el juego en una instancia que los personajes apartir del nivel SEGUNDO_NIVEL puedan pasar al siguiente nivel
void manejar_interseccion_personaje_llave(personaje_t *personaje, elemento_t herramientas[MAX_ELEMENTOS], int *tope_herramientas){
	for(int i = 0; i < *tope_herramientas; i++){
		if(herramientas[i].tipo == LLAVE){
			if(coordenadas_iguales(personaje->posicion, herramientas[i].coordenada) == true){
		       	personaje->tiene_llave = true;
		        herramientas[i] = herramientas[*tope_herramientas -1];
			    (*tope_herramientas)--;
			}
	    }
    }
}

//Recibe un struct de personaje_t inicializado, dentro de este struct un booleano INTERRUPTOR_APRETADO(en false o true), y el personaje VIVO
// Tope inicializado y mayor a 0
//post: Si el personaje pisa un INTERRUPTOR, cambiara el booleano INTERRUPTOR_APRETADO dentro del struct de personaje_t, si esta en false a true, si no el contrario.
//      Si se encuentra el TRUE ambos personajes se moveran horizontalmente igual, si esta en false, se moveran inversos.
void manejar_interseccion_personaje_interruptor(personaje_t *personaje, elemento_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos){
	for(int i = 0; i < tope_obstaculos; i++){
		if(obstaculos[i].tipo == INTERRUPTORES){
			if(coordenadas_iguales(personaje->posicion, obstaculos[i].coordenada) == true){
				if(personaje->interruptor_apretado == false)
					personaje->interruptor_apretado = true;
				else{
					personaje->interruptor_apretado = false;
				}
			}
		}
	}
}

//Recibe un struct sombra_t inicializado con sus coordenadas correctas, y un elemento_t con sus coordenadas validas.
//post: Si las coordenadas de sombra son iguales a las coordenadas de una matriz 3x3 del elemento_t devuelve true, si no false.
bool matriz_de_vela(sombra_t *sombra, elemento_t vela){
	bool devuelve = false;
	if((sombra->posicion.fila == vela.coordenada.fila   && sombra->posicion.col == vela.coordenada.col)  || 
	   (sombra->posicion.fila == vela.coordenada.fila+1 && sombra->posicion.col == vela.coordenada.col)  ||
	   (sombra->posicion.fila == vela.coordenada.fila-1 && sombra->posicion.col == vela.coordenada.col)  ||
	   (sombra->posicion.fila == vela.coordenada.fila   && sombra->posicion.col == vela.coordenada.col+1)|| 
	   (sombra->posicion.fila == vela.coordenada.fila   && sombra->posicion.col == vela.coordenada.col-1)||
	   (sombra->posicion.fila == vela.coordenada.fila-1 && sombra->posicion.col == vela.coordenada.col-1)||
	   (sombra->posicion.fila == vela.coordenada.fila+1 && sombra->posicion.col == vela.coordenada.col+1)||
	   (sombra->posicion.fila == vela.coordenada.fila-1 && sombra->posicion.col == vela.coordenada.col+1)||
	   (sombra->posicion.fila == vela.coordenada.fila+1 && sombra->posicion.col == vela.coordenada.col-1)){
		    devuelve = true;
	}
	return devuelve;
}

// Recibe los struct de personaje_t y sombra_t inicializados, el personaje con vida.
// El tope inicializado y mayor que 0.
// post: si sombra se posiciona en la matriz(3x3) de VELA, el booleano ESTA_VIVA dentro del struct de sombra_t se vuelve false, 
//       si mac se posiciona en la coordenada del obstaculo VELA, la elimina del vector de obstaculos y se resta uno al tope de obstaculos.  
//       Si mac pisa a sombra luego de eliminar la VELA del vector, el booleano ESTA_VIVA de sombra_t se vuelve true y se le descuentan COSTO_REVIVIR_SOMBRA 
//       a los PUNTOS dentro del struct de personaje_t.
void manejar_interseccion_personajes_velas(personaje_t *personaje, sombra_t *sombra, elemento_t obstaculos[MAX_ELEMENTOS], int *tope_obstaculos){
	for(int i = 0; i < *tope_obstaculos; i++){
		if(obstaculos[i].tipo == VELAS){
			if(matriz_de_vela(sombra, obstaculos[i]) == true){
				sombra->esta_viva = false;
			}
			if(coordenadas_iguales(personaje->posicion, obstaculos[i].coordenada) == true){
				obstaculos[i] = obstaculos[*tope_obstaculos -1];
			    (*tope_obstaculos)--;
			}
			if(coordenadas_iguales(personaje->posicion, sombra->posicion) == true){
				if(sombra->esta_viva == false){
					personaje->puntos =- 50;
					if(matriz_de_vela(sombra, obstaculos[i]) == false){
				        sombra->esta_viva = true;
			        }
			    }
			}
		}
	}
}

void realizar_jugada(juego_t* juego){
	char movimiento;
    pedir_movimiento(&movimiento);
    mover_personaje(juego, movimiento);   
    manejar_interseccion_personaje_pozos(juego, juego->niveles[juego->nivel_actual-1].tope_obstaculos);
    manejar_interseccion_personaje_llave(&juego->personaje, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
    manejar_interseccion_sombra_portales(&juego->sombra, &juego->personaje, juego->niveles[juego->nivel_actual-1].obstaculos, juego->niveles[juego->nivel_actual-1].tope_obstaculos);
    manejar_interseccion_personajes_monedas(&juego->personaje, &juego->sombra, juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas);
    manejar_interseccion_personaje_interruptor(&juego->personaje, juego->niveles[juego->nivel_actual-1].obstaculos, juego->niveles[juego->nivel_actual-1].tope_obstaculos);
    manejar_interseccion_personajes_velas(&juego->personaje, &juego->sombra, juego->niveles[juego->nivel_actual-1].obstaculos, &juego->niveles[juego->nivel_actual-1].tope_obstaculos);
}

//Recibe las coordenadas validas de los struct de personaje_t y sombra_t.
// El tope inicializado y mayor a 0.
//Post: Devuelve true si los personajes estan a una distancia manhattan menor a uno, de la herramienta PUERTA, falso si son diferentes.
bool distancia_manhattan(personaje_t *personaje, sombra_t *sombra, elemento_t herramientas[MAX_ELEMENTOS], int tope_herramientas){
	bool retorno = false;
	for(int i = 0; i < tope_herramientas; i++){
	    if(herramientas[i].tipo == PUERTA){
	    	if((abs(personaje->posicion.fila - herramientas[i].coordenada.fila) + 
		        abs(personaje->posicion.col - herramientas[i].coordenada.col)) <= RANGO_MANHATTAN && 
	           (abs(sombra->posicion.fila - herramientas[i].coordenada.fila) + 
	   	        abs(sombra->posicion.col - herramientas[i].coordenada.col)) <= RANGO_MANHATTAN){
	            retorno = true;
            }
        }
    }
    return retorno;
}

//Las coordenadas de la PUERTA y los personajes tienen que estar validadas 
//Post: Devuelve true si los personajes estan en rango con la PUERTA, false de lo contrario
bool rango_de_puerta(juego_t *juego){
	bool retorno = false;
	if(distancia_manhattan(&juego->personaje, &juego->sombra, juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas) == true){
	    retorno = true;
	}
	return retorno;
}

int estado_nivel(juego_t juego){
	int estado = 0;
	if(juego.nivel_actual == PRIMER_NIVEL){
		if(rango_de_puerta(&juego) == true && juego.sombra.esta_viva == true){
	        estado = 1;
	    }
	}
	else{
        if(rango_de_puerta(&juego) == true && juego.personaje.tiene_llave == true && juego.sombra.esta_viva == true){
	        estado = 1;
	    }
    }
    if(estado == 1){
    	inicializa_los_personajes(&juego);
    }

	return estado;
}

int estado_juego(juego_t juego){
	int estado = 0;
	if(juego.personaje.vida <= 0){
 	    estado = -1;
	}
	if((rango_de_puerta(&juego) == true) && juego.personaje.tiene_llave == true && juego.sombra.esta_viva == true && juego.nivel_actual == TERCER_NIVEL){
		estado = 1;
	}
	return estado;
}

void inicializar_juego_configuraciones(juego_t* juego, elementos_config_t primer_nivel, elementos_config_t segundo_nivel, elementos_config_t tercer_nivel){
	cargar_mapas();
    juego->nivel_actual = PRIMER_NIVEL;
	inicializar_nivel(&(juego->niveles[PRIMER_NIVEL-1]), PRIMER_NIVEL, primer_nivel.pozos, primer_nivel.velas, primer_nivel.interruptores,
	                                    primer_nivel.portales, primer_nivel.monedas, primer_nivel.escaleras, PUERTA_DE_LOS_NIVELES, primer_nivel.llaves); 

    inicializar_nivel(&(juego->niveles[SEGUNDO_NIVEL-1]), SEGUNDO_NIVEL, segundo_nivel.pozos, segundo_nivel.velas, segundo_nivel.interruptores, 
    	                                segundo_nivel.portales, segundo_nivel.monedas, segundo_nivel.escaleras, PUERTA_DE_LOS_NIVELES, segundo_nivel.llaves);

	inicializar_nivel(&(juego->niveles[TERCER_NIVEL-1]), TERCER_NIVEL, tercer_nivel.pozos, tercer_nivel.velas, tercer_nivel.interruptores, 
		                                tercer_nivel.portales, tercer_nivel.monedas, segundo_nivel.escaleras, PUERTA_DE_LOS_NIVELES, tercer_nivel.llaves);
	inicializa_los_personajes(&(*juego));
	inicializar_personaje(&juego->personaje, (juego->personaje.posicion));
	inicializar_sombra(&juego->sombra, (juego->sombra.posicion));
}

void imprimir_informacion_de_personajes(juego_t juego){
    printf("--------------------------BIENVENIDO-----------------------\n");
	printf(" MAC: fila %i col %i                    SOMBRA: fila %i col %i\n ", juego.personaje.posicion.fila, juego.personaje.posicion.col, 
                                    juego.sombra.posicion.fila, juego.sombra.posicion.col);
    if(juego.personaje.tiene_llave == true){
        printf("TIENE LLAVE ");
    }
    else{
        printf("SIN LLAVE ");
    }
    printf("\n PUNTOS: %i  \n VIDAS: %i\n",juego.personaje.puntos, juego.personaje.vida);
    if(juego.personaje.interruptor_apretado == false){
        printf(" INTERRUPTOR APAGADO\n");
    }
    else{
        printf(" INTERRUPTOR PRENDIDO\n");
    }
}

void cargar_proximo_nivel(juego_t *juego){
    juego->nivel_actual++; 
    juego->personaje.interruptor_apretado = false;
    juego->personaje.tiene_llave = false;
}

void mostrar_estado_finalizado(int estado_juego){
    if (estado_juego == GANO){ 
        printf("\t\t\t \n\n\n");
        printf("\n"

"██╗░░░██╗░█████╗░██╗░░░██╗  ░██╗░░░░░░░██╗██╗███╗░░██╗██╗\n"
"╚██╗░██╔╝██╔══██╗██║░░░██║  ░██║░░██╗░░██║██║████╗░██║██║\n"
"░╚████╔╝░██║░░██║██║░░░██║  ░╚██╗████╗██╔╝██║██╔██╗██║██║\n"
"░░╚██╔╝░░██║░░██║██║░░░██║  ░░████╔═████║░██║██║╚████║╚═╝\n"
"░░░██║░░░╚█████╔╝╚██████╔╝  ░░╚██╔╝░╚██╔╝░██║██║░╚███║██╗\n"
"░░░╚═╝░░░░╚════╝░░╚═════╝░  ░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚══╝╚═╝\n"
            );
        } else if (estado_juego == PERDIO){ 
            printf("\t\t\t \n\n\n");
            printf(
            
"░██████╗░░█████╗░███╗░░░███╗███████╗  ░█████╗░██╗░░░██╗███████╗██████╗░\n"
"██╔════╝░██╔══██╗████╗░████║██╔════╝  ██╔══██╗██║░░░██║██╔════╝██╔══██╗\n"
"██║░░██╗░███████║██╔████╔██║█████╗░░  ██║░░██║╚██╗░██╔╝█████╗░░██████╔╝\n"
"██║░░╚██╗██╔══██║██║╚██╔╝██║██╔══╝░░  ██║░░██║░╚████╔╝░██╔══╝░░██╔══██╗\n"
"╚██████╔╝██║░░██║██║░╚═╝░██║███████╗  ╚█████╔╝░░╚██╔╝░░███████╗██║░░██║\n"
"░╚═════╝░╚═╝░░╚═╝╚═╝░░░░░╚═╝╚══════╝  ░╚════╝░░░░╚═╝░░░╚══════╝╚═╝░░╚═╝\n"
        );
    }
}
