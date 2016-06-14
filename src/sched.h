/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"
#include "gdt.h"
#include "game.h"

typedef enum enum_id {
	A = 0,
	B = 1,
	H = 2
} id;

typedef struct str_tarea{
	unsigned int x;
	unsigned int y;
	unsigned short vivo;
	unsigned int x_map;
	unsigned int y_map;
	unsigned short map;
	
	id virus;
	unsigned char reloj;
	unsigned int cr3;
	unsigned short selector_tss;
} tarea;

typedef struct str_sched_j{
	tarea tareas[5];
	unsigned int actual;
} schedJ;

typedef struct str_scheduler{
	id corriendo;
	unsigned short idle;
	tarea* tareaActual;
	unsigned int actual;
	tarea tareas[15];
	schedJ js[2];
} scheduler;

unsigned short sched_proximo_indice();
tarea sched_nueva_tarea(unsigned int* codigo, unsigned int x, unsigned int y, id tipo);
void sched_inicializar();
void sched_inicializar_tareas_iniciales();
void sched_inicializar_tareas_jugadores();
extern scheduler SCHED;

#endif	/* !__SCHED_H__ */
