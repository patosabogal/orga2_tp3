; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones


ADDR_SIZE   equ 4
OFFSET_ECX  equ 7

%include "imprimir.mac"

BITS 32


;; PIC
extern fin_intr_pic1

;; reloj
extern screen_proximo_reloj

;; Sched
extern sched_proximo_indice

;; Game
extern screen_inicializar
extern atender_teclado
extern game_mapear
extern game_donde
extern game_soy
extern game_matar


intmsg0: db 'Divide Error'
intlen0 equ    $ - intmsg0
intmsg1: db 'RESERVED'
intlen1 equ    $ - intmsg1
intmsg2: db 'NMI Interrupt'
intlen2 equ    $ - intmsg2
intmsg3: db 'Breakpoint'
intlen3 equ    $ - intmsg3
intmsg4: db 'Overflow'
intlen4 equ    $ - intmsg4
intmsg5: db 'BOUND Range Exceeded'
intlen5 equ    $ - intmsg5
intmsg6: db 'Invalid Opcode (Undefined Opcode)'
intlen6 equ    $ - intmsg6
intmsg7: db 'Device Not Available'
intlen7 equ    $ - intmsg7
intmsg8: db 'Double Fault'
intlen8 equ    $ - intmsg8
intmsg9: db 'Coprocessor Segment Overrun (reserved)'
intlen9 equ    $ - intmsg9
intmsg10: db 'Invalid TSS'
intlen10 equ    $ - intmsg10
intmsg11: db 'Segment Not Present'
intlen11 equ    $ - intmsg11
intmsg12: db 'Stack-Segmentation Fault'
intlen12 equ    $ - intmsg12
intmsg13: db 'General Protection'
intlen13 equ    $ - intmsg13
intmsg14: db 'Page Fault'
intlen14 equ    $ - intmsg14
intmsg16: db 'x87 FPU Floating-Point Error (Math Fault)'
intlen16 equ    $ - intmsg16
intmsg17: db 'Alignment Check'
intlen17 equ    $ - intmsg17
intmsg18: db 'Machine Check'
intlen18 equ    $ - intmsg18
intmsg19: db 'SIMD Floating-Point Exception'
intlen19 equ    $ - intmsg19



;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;


%macro ISR_EC 1
global _isr%1

_isr%1:
    ;Mas arriba estan error code, eip, cs, eflags, esp, ss
    lea esp,[esp+4] ;Piso el error code
    push eax
    push ecx
    push edx
    push ebx
    push ebp
    push esi
    push edi
    push ds
    push es
    push fs
    push gs
    mov eax,cr0
    push eax
    mov eax,cr2
    push eax
    mov eax,cr3
    push eax
    mov eax,cr4
    push eax
    push esp ;Mostrar pantallita recibe lista entera
    call game_matar

    mov eax, %1
    imprimir_texto_mp intmsg%1, intlen%1, 0x07, 0, 0
    mov ax,0x48;Tarea inicial dummy
    ltr ax

    sti
    jmp 0x50:0x69 ;Idle
%endmacro

%macro ISR 1
global _isr%1

_isr%1:
    ;Mas arriba estan eip, cs, eflags, esp, ss
    push eax
    push ecx
    push edx
    push ebx
    push ebp
    push esi
    push edi
    push ds
    push es
    push fs
    push gs
    mov eax,cr0
    push eax
    mov eax,cr2
    push eax
    mov eax,cr3
    push eax
    mov eax,cr4
    push eax
    push esp ;Mostrar pantallita recibe lista entera

    call game_matar

    mov eax, %1
    imprimir_texto_mp intmsg%1, intlen%1, 0x07, 0, 0
    mov ax,0x48;Tarea inicial dummy
    ltr ax

    sti
    jmp 0x50:0x69 ;Idle
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR_EC 8
ISR 9
ISR_EC 10
ISR_EC 11
ISR_EC 12
ISR_EC 13
ISR_EC 14
ISR 16
ISR_EC 17
ISR 18
ISR 19


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00
screen_timer:   dw 0

global _isr32
_isr32:
    pushad
    ;inc word [screen_timer]
    ;cmp word [screen_timer],5
    ;jne .noActualizar
    ;mov word [screen_timer],0

    ;call screen_inicializar

    .noActualizar:
    call fin_intr_pic1
    call proximo_reloj
    call sched_proximo_indice

    ;mov ax,0
    cmp ax, 0
    je  .end
        mov [sched_tarea_selector], ax
        jmp far [sched_tarea_offset]
	    jmp .end
	
	.end:
        popad
        iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33
_isr33:

    pushad
    in al, 0x60
    push eax
    call atender_teclado
    pop eax

    call fin_intr_pic1
    popad
    iret

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;


%define DONDE  0x124
%define SOY    0xA6A
%define MAPEAR 0xFF3

%define VIRUS_ROJO 0x841
%define VIRUS_AZUL 0x325


global _isr102
_isr102:
    pushad
    push ebx

    cmp eax,DONDE
    jne .SOY
    call game_donde
    jmp .fin


    .SOY:
    cmp eax,SOY
    jne .MAPEAR
    call game_soy
    jmp .fin

    .MAPEAR:
    cmp eax, MAPEAR
    jne .MELLAMARONMAL
    push ecx
    call game_mapear
    pop ecx
    jmp .fin

    .MELLAMARONMAL:
    ;mov dword [esp + ADDR_SIZE*OFFSET_ECX], 0x42

    .fin:
    jmp 0x50:0x69 ;Idle

    pop ebx
    popad
    iret

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79
                popad
        ret
        
        
