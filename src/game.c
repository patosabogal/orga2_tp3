/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"


game_state GAME;

void game_mover_cursor(int jugador, direccion dir) {
}

void game_lanzar(unsigned int jugador) {
}

void game_soy(unsigned int yoSoy) {
}

void game_donde(unsigned int* pos) {
}

void game_mapear(int x, int y) {
}

void game_inicializar(){
	jugador jug_A = {
		15,		// vidas
		0,		// puntos
		0,		// x inicial
		0,		// y inicial
	};

	jugador jug_B = {
		15,		// vidas
		0,		// puntos
		79,		// x inicial
		0,		// y inicial
	};

	GAME.A = jug_A;
	GAME.B = jug_B;
}
