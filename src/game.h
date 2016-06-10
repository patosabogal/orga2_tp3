/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"

typedef enum enum_id {
	A = 0,
	B = 1,
	H = 2
} id;

typedef struct str_tarea{
	unsigned int x;
	unsigned int y;
	unsigned short vivo;
	id virus;
	unsigned char reloz;
	unsigned int cr3;
	unsigned short selector_tss;
} tarea;

typedef struct str_jugador{
	unsigned int vidas;
	unsigned int x;
	unsigned int y;
	unsigned short actual;
	tarea tareas[5];
} jugador;

typedef struct str_game_state{
	jugador js[2];
	tarea iniciales[15];
	unsigned short actual;
	id corriendo;
	tarea* tareaActual;
} game_state;

// typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ARB = 0xA33, ABA = 0x883  } direccion;
typedef enum enum_direccion {arriba, abajo, derecha, izquierda} direccion;

//void game_lanzar(unsigned int jugador);

void game_mapear(unsigned int x, unsigned y);
void game_soy(unsigned int soy);
void game_donde(unsigned int* pos);

void game_mover_cursor(jugador* j, direccion dir);

void game_inicializar();

void atender_teclado(char tecla);

tarea nueva_tarea(unsigned int* codigo, unsigned int x, unsigned int y, id tipo);

void game_inicializar_tareas_iniciales();

void game_inicializar_tareas_jugadores();

extern game_state GAME;
extern unsigned int DEBUG_MODE;

#endif  /* !__GAME_H__ */
