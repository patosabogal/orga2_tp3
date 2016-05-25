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

typedef struct str_game_state{
	unsigned int vidas_A;
	unsigned int vidas_B;
	unsigned int puntos_A;
	unsigned int puntos_B;
} game_state;

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ARB = 0xA33, ABA = 0x883  } direccion;

void game_lanzar(unsigned int jugador);

void game_soy(unsigned int soy);

void game_donde(unsigned int* pos);

void game_mover_cursor(int jugador, direccion dir);

void game_inicializar();

extern game_state GAME;
#endif  /* !__GAME_H__ */
