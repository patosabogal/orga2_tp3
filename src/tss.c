/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

#define INTERRUPCIONES 	0x00000200
#define PORQUESI 		0x00000002
tss tss_inicial;
tss tss_idle;


void tss_inicializar() {
	gdt[GDT_TSS_INICIAL] = (gdt_entry) {     // descriptor de video con privilegio 0
        (unsigned short)    sizeof(tss)-1,         /* limit[0:15]  */
		(unsigned int)   	&tss_inicial,           /* base[0:15]   */
		(unsigned int)     	&tss_inicial >> 16,                  /* base[23:16]  */
        (unsigned char)     0x09,                  /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x00,           /* g            */
		(unsigned int)     	&tss_inicial >> 24,           /* base[31:24]  */
    };
    gdt[GDT_TSS_IDLE] = (gdt_entry) {     // descriptor de video con privilegio 0
        (unsigned short)    sizeof(tss)-1,         /* limit[0:15]  */
		(unsigned int)		&tss_idle,           /* base[0:15]   */
		(unsigned int)		&tss_idle >> 16,/* base[23:16]  */
        (unsigned char)     0x09,                  /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x00,           /* g            */
		(unsigned int)    	&tss_idle >> 24,           /* base[31:24]  */
    };

    tss_idle.eip = 0x00010000;
    tss_idle.ebp = KERNEL_BASE_STACK;//BANANOIDEçç
    tss_idle.esp = KERNEL_BASE_STACK;//BANANOIDEçç
    tss_idle.cs = 0x20; //BANANOIDEçç
    tss_idle.ss = 0x30;
    tss_idle.ds = 0x30;
    tss_idle.es = 0x30;
    tss_idle.gs = 0x30;
    tss_idle.fs = 0x80;
    tss_idle.cr3 = PAGE_DIRECTORY_BASE;
    tss_idle.eflags |= (PORQUESI);

}
