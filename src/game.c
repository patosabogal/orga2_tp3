/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#define INF_ROJO 0x841
#define INF_AZUL 0x325

game_state GAME;
unsigned int DEBUG_MODE = FALSE;

void game_lanzar(id j) {
	unsigned int x = GAME.js[j].x;
	unsigned int y = GAME.js[j].y;
	unsigned int* codigo;

	if(j == A){
		codigo = (unsigned int*)CODIGO_TAREA_B;
	}else{
		codigo = (unsigned int*)CODIGO_TAREA_A;
	}

	int i = 0;
	while(i < CANT_TAREAS_J && GAME.js[j].tareas[i].vivo){
		i++;
	}
	
	if(i != CANT_TAREAS_J){
		GAME.js[j].tareas[i] = nueva_tarea(codigo,x,y,j);
		GAME.js[j].vidas --;
	}
}

void game_soy(unsigned int yoSoy) {
	switch(yoSoy){
		case INF_ROJO:
			GAME.tareaActual->virus = A;
			break;
		case INF_AZUL:
			GAME.tareaActual->virus = B;
			break;
	}
}

void game_donde(unsigned int* pos) {
	pos[0] = GAME.tareaActual->x;
	pos[1] = GAME.tareaActual->y-1;//Posicion relativa a la pantalla de juego
}

void game_mapear(unsigned int x,unsigned int y) {
	if(x >= SIZE_W || y >= SIZE_H){
		return;
	}
	
	mmu_mapear_pagina(CODIGO_MAPEADO,GAME.tareaActual->cr3,pointToAddr(x,y),PG_USER);
}


void game_inicializar(){
	jugador jug_A = {
		15,		// vidas
		0,		// x inicial
		1,		// y inicial
		0		// Tarea Actual
	};

	jugador jug_B = {
		15,		// vidas
		79,		// x inicial
		1,		// y inicial
		0		// Tarea Actual
	};

	GAME.js[A] = jug_A;
	GAME.js[B] = jug_B;
	GAME.actual = 0;
	GAME.corriendo = H;

	game_inicializar_tareas_iniciales();
	game_inicializar_tareas_jugadores();
}

void game_inicializar_tareas_iniciales(){
	int i;
	for (i = 0; i < CANT_H; ++i)
	{
		GAME.iniciales[i] = nueva_tarea((unsigned int *)CODIGO_TAREA_H, 16+i, 16-i,H);
	}
}

void game_inicializar_tareas_jugadores(){
	int i;
	for (i = 0; i < CANT_TAREAS_J; ++i)
	{
		GAME.js[A].tareas[i].vivo = FALSE;
		GAME.js[B].tareas[i].vivo = FALSE;
	}
}

tarea nueva_tarea(unsigned int* codigo, unsigned int x, unsigned int y, id tipo){
	unsigned short _selector_tss = 0;
	unsigned int _cr3 = 0;
	tss_nueva(codigo, x, y, &_selector_tss, &_cr3);
	tarea nueva_tarea;
	nueva_tarea.x = x;
	nueva_tarea.y = y;
	nueva_tarea.vivo = TRUE;
	nueva_tarea.virus = tipo;
	nueva_tarea.selector_tss = _selector_tss;
	nueva_tarea.cr3 = _cr3;
	return nueva_tarea;
}

void atender_teclado(const char tecla_fea){
	unsigned int tecla = (unsigned int) tecla_fea;
	// print_hex(tecla, 2, 78, 0, C_FG_WHITE | C_BG_RED);	// Imprimir en esquina derecha el codigo presionado

	switch(tecla){
		case 0x11:	// W
			game_mover_cursor(&GAME.js[A], arriba);
			break;
		case 0x1e:	// A
			game_mover_cursor(&GAME.js[A], izquierda);
			break;
		case 0x1f:	// S
			game_mover_cursor(&GAME.js[A], abajo);
			break;
		case 0x20:	// D
			game_mover_cursor(&GAME.js[A], derecha);
			break;
		case 0x17:	// I
			game_mover_cursor(&GAME.js[B], arriba);
			break;
		case 0x24:	// J
			game_mover_cursor(&GAME.js[B], izquierda);
			break;
		case 0x25:	// K
			game_mover_cursor(&GAME.js[B], abajo);
			break;
		case 0x26:	// L
			game_mover_cursor(&GAME.js[B], derecha);
			break;
		case 0x2a:	// LShift
			game_lanzar(A);
			// Lanzar tarea
			break;
		case 0x36:	// RShift
			game_lanzar(B);
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