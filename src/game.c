/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

game_state GAME = {
	15,
	15,
	0,
	0
};

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
	GAME.vidas_A = 15;
	GAME.vidas_B = 15;
	GAME.puntos_A = 0;
	GAME.puntos_B = 0;
}
