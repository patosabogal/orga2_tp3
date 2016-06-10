/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE               0x00000001
#define FALSE              0x00000000
#define ERROR                   	1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_H                 	   15
#define CANT_TAREAS_J               5
#define SIZE_W                     80
#define SIZE_H                     44
#define PAGE_SIZE			0x00001000


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 					100

#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_CODE_0_DESC			4
#define GDT_IDX_CODE_3_DESC			5
#define GDT_IDX_DATA_0_DESC			6
#define GDT_IDX_DATA_3_DESC			7
#define GDT_VIDEO_DESC				8
#define GDT_TSS_INICIAL				9
#define GDT_TSS_IDLE				10

#define GDT_TSS_COMIENZO			11
#define GDT_TSS_FIN					100

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)
#define GDT_OFF_CODE_0_DESC			(GDT_IDX_CODE_0_DESC 	<< 3)
#define GDT_OFF_CODE_3_DESC			(GDT_IDX_CODE_3_DESC 	<< 3)
#define GDT_OFF_DATA_0_DESC			(GDT_IDX_DATA_0_DESC 	<< 3)
#define GDT_OFF_DATA_3_DESC			(GDT_IDX_DATA_3_DESC 	<< 3)
#define GDT_OFF_VIDEO_DESC			(GDT_VIDEO_DESC 		<< 3)


/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO_SCREEN			0x000B8000	/* direccion fisica del buffer de video */
#define MAPA					0x400000 	/* direccion fisica del mapa */
#define CODIGO					0x8000000 	/* direccion logica de codigo de tarea */
#define CODIGO_MAPEADO			0x8001000   /* direccion logica de pagina mapeada de tarea */
#define BASE_PILA_TAREA			0x8001000	/* direccion logica de la base de la pila de la tarea */
#define KERNEL_BASE_STACK		0x27000
#define PAGE_DIRECTORY_BASE		0x27000
#define PAGE_TABLE_BASE			0x28000
#define INICIO_PAGINAS_LIBRES 	0x100000
#define CODIGO_TAREA_A			0x11000
#define CODIGO_TAREA_B			0x12000
#define CODIGO_TAREA_H			0x13000
#endif  /* !__DEFINES_H__ */
