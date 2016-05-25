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


typedef struct str_page_entry{
	unsigned char	p:1;
	unsigned char	rw:1;
	unsigned short	basuras:10;
	unsigned int	base_page_addr:20;

} __attribute__((__packed__)) page_entry;

typedef struct str_page_entries_set {
	page_entry page_entries[1024];
} __attribute__((__packed__)) page_entries_set;

extern page_entries_set* PDE;
void mmu_inicializar();

#endif	/* !__MMU_H__ */




