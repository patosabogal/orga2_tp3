/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

page_entries_set* PDE = (page_entries_set*) PAGE_DIRECTORY_BASE;
page_entries_set* PTE = (page_entries_set*) PAGE_TABLE_BASE;

void mmu_inicializar() {
	unsigned int i = 0;
	while (i < 1024){ 
		PDE->page_entries[i].p = 0x0;
		
		PTE->page_entries[i].p = 0x01;
		PTE->page_entries[i].rw = 0x01;
		PTE->page_entries[i].basuras = 0x0000;
		PTE->page_entries[i].base_page_addr = i;
		i++;
	}

	PDE->page_entries[0].p = 0x1;
	PDE->page_entries[0].rw = 0x1;
	PDE->page_entries[0].basuras = 0x00;
	PDE->page_entries[0].base_page_addr = PAGE_TABLE_BASE >> 12;
}