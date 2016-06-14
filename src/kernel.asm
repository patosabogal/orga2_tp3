; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

extern GDT_DESC

extern IDT_DESC
extern idt_inicializar

extern PDE
extern mmu_inicializar
extern mmu_inicializar_dir_kernel

extern tss_inicializar

extern habilitar_pic
extern resetear_pic

extern screen_inicializar

extern sched_inicializar
;TEST
extern mmu_mapear_pagina
extern game_inicializar

extern screen_inicializar
extern mostrar_pantallita
extern tss_nueva
global start


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
    
    ; Habilitar A20
    call habilitar_A20

    ; Cargar la GDT
    LGDT [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax,cr0
    or eax,1
    mov cr0,eax

    ; Saltar a modo protegido
    jmp 0x20:modoprotegido
    modoprotegido:

    BITS 32
    ; Establecer selectores de segmentos
    mov ax,0x30
    mov ds,ax ;Segmento de datos de kernel
    mov ss,ax ;Segmento de datos de kernel
    mov es,ax
    mov gs,ax

    mov ax,0x40 ;Video de kernel
    mov fs,ax

    ; Establecer la base de la pila
    mov ebp,0x27000 ;Base segun el dibujito
    mov esp,0x27000

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar pantalla
    call limpiarPantalla
    ; Inicializar el manejador de memoria
    call mmu_inicializar
    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    mov eax, [PDE]
    mov cr3, eax
    

    ; Habilitar paginacion
    mov eax,cr0
    or eax,0x80000000
    mov cr0,eax


    ; Inicializar tss
    ; Inicializar tss de la tarea Idle
    call tss_inicializar

    ; Inicializar el scheduler
    call sched_inicializar

    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones  
    call resetear_pic
    call habilitar_pic

    ; Cargar tarea inicial
    mov ax,0x48;Tarea inicial
    ltr ax

    call game_inicializar
    call screen_inicializar

    ; Habilitar interrupciones
    sti
    
    ;call screen_inicializar
    ;call mostrar_pantallita

    ; Saltar a la primera tarea: Idle
    ;jmp 0x50:0x69 ;Idle

    ; Ciclar infinitamente (por si algo sale mal...)




    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

limpiarPantalla:
    mov ecx,0
    .ciclo:
        mov word [fs:ecx],0x07DB
        add ecx,2
        cmp ecx,0x1F40 ;80*50*2bytes
        jne .ciclo

    ret


%include "a20.asm"
