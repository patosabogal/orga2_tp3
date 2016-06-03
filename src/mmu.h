/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"
#include "screen.h"
#include "pila.h"
typedef struct str_page_entry{
	unsigned int	attr:12;
	unsigned int	base_page_addr:20;

} __attribute__((__packed__)) page_entry;

typedef struct str_page_entries_set {
	page_entry page_entries[1024];
} __attribute__((__packed__)) page_entries_set;


//unsigned int IdleTasksPDE[15];

extern page_entries_set* PDE;
void mmu_inicializar();
void mmu_inicializar_dir_kernel();
unsigned int mmu_proxima_pagina_fisica_libre();
void mmu_mapear_pagina(unsigned int virtu, unsigned int cr3, unsigned int fisica);
unsigned int mmu_inicializar_dir_tarea();

#endif	/* !__MMU_H__ */




