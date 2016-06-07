/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

game_state GAME;
unsigned int DEBUG_MODE = TRUE;

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
		1,		// y inicial
	};

	jugador jug_B = {
		15,		// vidas
		0,		// puntos
		79,		// x inicial
		1,		// y inicial
	};

	GAME.A = jug_A;
	GAME.B = jug_B;
}

void atender_teclado(const char tecla_fea){
	unsigned int tecla = (unsigned int) tecla_fea;
	// print_hex(tecla, 2, 78, 0, C_FG_WHITE | C_BG_RED);	// Imprimir en esquina derecha el codigo presionado

	switch(tecla){
		case 0x11:	// W
			game_mover_cursor(&GAME.A, arriba);
			break;
		case 0x1e:	// A
			game_mover_cursor(&GAME.A, izquierda);
			break;
		case 0x1f:	// S
			game_mover_cursor(&GAME.A, abajo);
			break;
		case 0x20:	// D
			game_mover_cursor(&GAME.A, derecha);
			break;
		case 0x17:	// I
			game_mover_cursor(&GAME.B, arriba);
			break;
		case 0x24:	// J
			game_mover_cursor(&GAME.B, izquierda);
			break;
		case 0x25:	// K
			game_mover_cursor(&GAME.B, abajo);
			break;
		case 0x26:	// L
			game_mover_cursor(&GAME.B, derecha);
			break;
		case 0x2a:	// LShift
			game_mover_cursor(&GAME.B, derecha);
			// Lanzar tarea
			break;
		case 0x36:	// RShift
			game_mover_cursor(&GAME.B, derecha);
			// Lanzar tarea
			break;			
		case 0x15:	// Y
			if (DEBUG_MODE){
				DEBUG_MODE = FALSE;
			} else {
				DEBUG_MODE = TRUE;
			}
			// Lanzar tarea
			break;
	}

}

void game_mover_cursor(jugador* j, direccion dir){
	switch(dir){
		case arriba:
			if (j->y == 1){
				j->y = SIZE_H;
			} else {
				j->y = j->y - 1;
			}
			break;

		case izquierda:
			if (j->x == 0){
				j->x = SIZE_W - 1;
			} else {
				j->x = j->x - 1;
			}
			break;

		case abajo:
			if (j->y == SIZE_H){
				j->y = 1;
			} else{
				j->y = j->y + 1;
			}
			break;

		case derecha:
			if (j->x == SIZE_W - 1){
				j->x = 0;
			} else {
				j->x = j->x + 1;				
			}
			break;
	}

}