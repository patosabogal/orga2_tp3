/* ** por compatibilidad se omiten tildes **gdt_entry
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#define PDE_INDEX(virtu) virtu >> 22
#define PTE_INDEX(virtu) (virtu << 10) >> 22
//#define ALIGN(dir)         



pila pila_libres;
page_entries_set* PDE;
page_entries_set* PTE;
unsigned int proxima_pagina_libre;

void mmu_inicializar(){
 	proxima_pagina_libre = INICIO_PAGINAS_LIBRES;
 	nueva_pila(&pila_libres,(unsigned int*)MAPA);//Base de la pila en MAPA (crece para abajo)
}

void mmu_inicializar_dir_kernel() {
 	PDE = (page_entries_set*) PAGE_DIRECTORY_BASE;
 	PTE = (page_entries_set*) PAGE_TABLE_BASE;
 	
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
	unsigned int pagina_libre;
	//if(pila_libres.cant == 0){
		pagina_libre = proxima_pagina_libre;
		proxima_pagina_libre += PAGE_SIZE;
	/*}
	else{
		pagina_libre = pop_pila(&pila_libres);
	}*/
	return pagina_libre;
}

void mmu_liberar_pagina(unsigned int pagina){
	//print_hex((unsigned int) pila_libres.tope,8,0,0,C_FG_WHITE | C_BG_BLACK);
	//push_pila(&pila_libres, pagina);
	return;
}

void mmu_mapear_pagina(unsigned int virtu, unsigned int cr3, unsigned int fisica, unsigned int user){
	page_entries_set* pd = (page_entries_set*) (cr3 & 0xFFFFF000);
	page_entries_set* pt;

	unsigned int pd_ind = PDE_INDEX(virtu);
	//LO MAS DECLARATIVO QUE EXISTE
	if(!(pd->page_entries[pd_ind].attr & PG_PRESENT)){
		pd->page_entries[pd_ind].attr = (PG_PRESENT | PG_READ_WRITE | user);
		pd->page_entries[pd_ind].base_page_addr = mmu_proxima_pagina_fisica_libre() >> 12;

		//Setear la nueva en 0
		pt = (page_entries_set*) (pd->page_entries[pd_ind].base_page_addr << 12);
		unsigned int i = 0;
		while(i < 1024){
			pt->page_entries[i].attr = 0;
			i++;
		}
	}

	pt = (page_entries_set*) (pd->page_entries[pd_ind].base_page_addr << 12);
	unsigned int pt_ind = PTE_INDEX(virtu);
	pt->page_entries[pt_ind].attr |= (PG_PRESENT | PG_READ_WRITE | user);
	pt->page_entries[pt_ind].base_page_addr = fisica >> 12;
	tlbflush();
}

unsigned int tablaVacia(page_entries_set* table_addr){

	unsigned int i = 0;
	//Corta cuando alguno esta presente
	while(i < 1024 && !(table_addr->page_entries[i].attr & PG_PRESENT)){
		i++;
	}
	return i == 1024;
}

void mmu_unmapear_pagina(unsigned int virtu, unsigned int cr3){

	page_entries_set* pd = (page_entries_set*) (cr3 & 0xFFFFF000);
	unsigned int pd_ind = PDE_INDEX(virtu);
	
	page_entries_set* pt = (page_entries_set*) (pd->page_entries[pd_ind].base_page_addr << 12);
	unsigned int pt_ind = PTE_INDEX(virtu);
	pt->page_entries[pt_ind].attr = 0;

	//Si la pt esta vacia hay que limpiar la pd en esa posicion
	if(tablaVacia(pt)){
		pd->page_entries[pd_ind].attr = 0;
		mmu_liberar_pagina((unsigned int) pt);
	}
	if(tablaVacia(pd)){
		mmu_liberar_pagina((unsigned int) pd);
	}

	tlbflush();
}

void mmu_liberar_directorio(unsigned int cr3){

	page_entries_set* pd = (page_entries_set*) cr3;
	unsigned int i = 0;

	while(i < 1024){
		if(pd->page_entries[i].attr & PG_PRESENT){
			page_entries_set* pt = (page_entries_set*) (pd->page_entries[i].base_page_addr << 12);
			mmu_liberar_pagina((unsigned int) pt);
		}
		i++;
	}
	mmu_liberar_pagina((unsigned int) pd);
}

unsigned int pointToAddr(unsigned int x,unsigned int y){
	return  MAPA + (x+SIZE_W*y)*PAGE_SIZE;
}

unsigned int mmu_inicializar_dir_tarea(unsigned int* codigo, unsigned int x, unsigned int y){
 	page_entries_set* pd = (page_entries_set*) mmu_proxima_pagina_fisica_libre();
 	unsigned int i = 0;
	while(i < 1024){
		pd->page_entries[i].attr = 0;
		i++;
	}

	//IDENTITY 4MB
	i = 0;
	while(i<1024){
		mmu_mapear_pagina(i*PAGE_SIZE,(unsigned int) pd,i*PAGE_SIZE,0);
		i++;
	}

	//CODIGO EN LA 8kk
	unsigned int* addr = (unsigned int*) pointToAddr(x,y);
	mmu_mapear_pagina(CODIGO,(unsigned int) pd,(unsigned int) addr, PG_USER);
	
	//MAPEO EN EL KERNEL LA DIRECCION TAMBIEN
	mmu_mapear_pagina((unsigned int) addr,rcr3(),(unsigned int) addr,0);
	i = 0;
	//Copio int a int
	while(i < 1024){
		addr[i] = codigo[i];
		i++;
	}
	mmu_unmapear_pagina((unsigned int) addr,rcr3());
	return (unsigned int) pd;
}