/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

#define INTERRUPCIONES 	0x00000200
#define PORQUESI 		0x00000002
#define USER_SEG        0x3
tss tss_inicial;
tss tss_idle;

void tss_inicializar() {
	gdt[GDT_TSS_INICIAL] = (gdt_entry) {     // descriptor de tss inicial
        (unsigned short)    sizeof(tss)-1,         /* limit[0:15]  */
		(unsigned int)   	&tss_inicial,          /* base[0:15]   */
		(unsigned int)     	&tss_inicial >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,                  /* type         */
        (unsigned char)     0x00,                  /* s            */
        (unsigned char)     0x00,                  /* dpl          */
        (unsigned char)     0x01,                  /* p            */
        (unsigned char)     0x00,                  /* limit[16:19] */
        (unsigned char)     0x00,                  /* avl          */
        (unsigned char)     0x00,                  /* l            */
        (unsigned char)     0x01,                  /* db           */
        (unsigned char)     0x00,                  /* g            */
		(unsigned int)     	&tss_inicial >> 24,    /* base[31:24]  */
    };

    gdt[GDT_TSS_IDLE] = (gdt_entry) {     // descriptor de tss idle
        (unsigned short)    sizeof(tss)-1,         /* limit[0:15]  */
		(unsigned int)		&tss_idle,             /* base[0:15]   */
		(unsigned int)		&tss_idle >> 16,       /* base[23:16]  */
        (unsigned char)     0x09,                  /* type         */
        (unsigned char)     0x00,                  /* s            */
        (unsigned char)     0x00,                  /* dpl          */
        (unsigned char)     0x01,                  /* p            */
        (unsigned char)     0x00,                  /* limit[16:19] */
        (unsigned char)     0x00,                  /* avl          */
        (unsigned char)     0x00,                  /* l            */
        (unsigned char)     0x01,                  /* db           */
        (unsigned char)     0x00,                  /* g            */
		(unsigned int)    	&tss_idle >> 24,       /* base[31:24]  */
    };

    tss_idle.eip = 0x00010000;
    tss_idle.ebp = KERNEL_BASE_STACK;
    tss_idle.esp = KERNEL_BASE_STACK;
    //tss_idle.esp0 = KERNEL_BASE_STACK;
    //tss_idle.ss0 = 0x30;
    tss_idle.cs = GDT_OFF_CODE_0_DESC;
    tss_idle.ss = GDT_OFF_DATA_0_DESC;
    tss_idle.ds = GDT_OFF_DATA_0_DESC;
    tss_idle.es = GDT_OFF_DATA_0_DESC;
    tss_idle.gs = GDT_OFF_DATA_0_DESC;
    tss_idle.fs = GDT_OFF_VIDEO_DESC;
    tss_idle.cr3 = PAGE_DIRECTORY_BASE;
    tss_idle.eflags |= (PORQUESI | INTERRUPCIONES);

    unsigned int i = 0;
	for(i = GDT_TSS_COMIENZO; i < GDT_TSS_FIN; i++){
		gdt[i].p = 0;
	}

}

unsigned int tss_entrada_disponible(){
	unsigned int i = GDT_TSS_COMIENZO;
	while( i < GDT_TSS_FIN && gdt[i].p == 1){
		i++;
	}
	return i;
}

void tss_matar(unsigned short segmento){
    tss* tarea_muerta;
    gdt_entry entry = gdt[segmento >> 3];
    tarea_muerta = (tss*)((unsigned int) entry.base_0_15 | (unsigned int) entry.base_23_16 << 16 | (unsigned int) entry.base_31_24 << 24);
    unsigned int cr3 = tarea_muerta->cr3;
    mmu_liberar_directorio(cr3); //Libero directorio
    mmu_liberar_pagina((unsigned int)tarea_muerta->esp0); //Libero pila de kernel
    mmu_liberar_pagina((unsigned int)tarea_muerta); //Libero tss
    entry.p = 0;
}

void tss_nueva(unsigned int* codigo, unsigned int x, unsigned int y, unsigned short* segmento, unsigned int* cr3){
	// codigo = (unsigned int*) 0x11000;
	unsigned int disp = tss_entrada_disponible();
    tss* tss_tn = (tss*) mmu_proxima_pagina_fisica_libre();
    unsigned int i = 0;
    unsigned char* p = (unsigned char*) tss_tn;
    while(i < sizeof(tss)){
        p[i] = 0;
        i++;
    }

    unsigned int pila0 = mmu_proxima_pagina_fisica_libre() + PAGE_SIZE;
    unsigned int _cr3 = mmu_inicializar_dir_tarea(codigo, x, y);

	tss_tn->eip = CODIGO; //Direccion de codigo mapeada a codigo de la tarea
	tss_tn->ebp = BASE_PILA_TAREA;
	tss_tn->esp = BASE_PILA_TAREA;
	tss_tn->esp0 = pila0;
	tss_tn->ss0 = GDT_OFF_DATA_0_DESC;
	tss_tn->ss = GDT_OFF_DATA_3_DESC | USER_SEG;
	tss_tn->ds = GDT_OFF_DATA_3_DESC | USER_SEG;
	tss_tn->gs = GDT_OFF_DATA_3_DESC | USER_SEG;
	tss_tn->fs = GDT_OFF_DATA_3_DESC | USER_SEG;
	tss_tn->es = GDT_OFF_DATA_3_DESC | USER_SEG;
	tss_tn->cs = GDT_OFF_CODE_3_DESC | USER_SEG;
    

	tss_tn->cr3 = _cr3;
	tss_tn->eflags |= (PORQUESI | INTERRUPCIONES);

	gdt[disp] = (gdt_entry) {
        (unsigned short)    sizeof(tss)-1,	/* limit[0:15]  */
		(unsigned int)      tss_tn,         /* base[0:15]   */
		(unsigned int)		tss_tn >> 16,	/* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x00,           /* g            */
		(unsigned int)    	tss_tn >> 24,  /* base[31:24]  */
    };

    //unsigned short _selector_tss =  (disp << 3);
    *segmento = (disp << 3);
    *cr3 = _cr3;
}