/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#define PDE_INDEX(virtu) virtu >> 22
#define PTE_INDEX(virtu) (virtu << 10) >> 22
//#define ALIGN(dir)         
#define PG_READ_WRITE	0x00000002
#define PG_USER			0x00000004
#define PG_PRESENT		0x00000001

#define PAGE_SIZE		0x00001000

page_entries_set* PDE = (page_entries_set*) PAGE_DIRECTORY_BASE;
page_entries_set* PTE = (page_entries_set*) PAGE_TABLE_BASE;
unsigned int proxima_pagina_libre = INICIO_PAGINAS_LIBRES;

void mmu_inicializar() {
	unsigned int i = 0;
	unsigned int wr_p = PG_READ_WRITE | PG_PRESENT;
	while (i < 1024){ 
		PDE->page_entries[i].attr = 0;
		PTE->page_entries[i].attr = wr_p;
		PTE->page_entries[i].base_page_addr = i;
		i++;
	}

	PDE->page_entries[0].attr = wr_p;
	PDE->page_entries[0].base_page_addr = PAGE_TABLE_BASE >> 12;
}

unsigned int mmu_proxima_pagina_fisica_libre() {
	unsigned int pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	
	return pagina_libre;
}

void mmu_mapear_pagina(unsigned int virtu, unsigned int cr3, unsigned int fisica){
	page_entries_set* pd = (page_entries_set*) (cr3 & 0xFFFFF000);

	unsigned int pd_ind = PDE_INDEX(virtu);
	//LO MAS DECLARATIVO QUE EXISTE
	if(!(pd->page_entries[pd_ind].attr & PG_PRESENT)){
		pd->page_entries[pd_ind].attr |= PG_PRESENT;
		pd->page_entries[pd_ind].base_page_addr = mmu_proxima_pagina_fisica_libre() >> 12;
	}

	page_entries_set* pt = (page_entries_set*) (pd->page_entries[pd_ind].base_page_addr << 12);
	unsigned int pt_ind = PTE_INDEX(virtu);
	pt->page_entries[pt_ind].attr |= PG_PRESENT;
	pt->page_entries[pt_ind].base_page_addr = fisica >> 12;
}

void mmu_inicializar_dir_tarea(){

}

