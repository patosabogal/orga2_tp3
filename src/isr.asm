; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice


intmsg0: db 'Divide Error',0
intlen0 equ    $ - intmsg0
intmsg1: db 'RESERVED',0
intlen1 equ    $ - intmsg1
intmsg2: db 'NMI Interrupt',0
intlen2 equ    $ - intmsg2
intmsg3: db 'Breakpoint',0
intlen3 equ    $ - intmsg3
intmsg4: db 'Overflow',0
intlen4 equ    $ - intmsg4
intmsg5: db 'BOUND Range Exceeded',0
intlen5 equ    $ - intmsg5
intmsg6: db 'Invalid Opcode (Undefined Opcode)',0
intlen6 equ    $ - intmsg6
intmsg7: db 'Device Not Available',0
intlen7 equ    $ - intmsg7
intmsg8: db 'Double Fault',0
intlen8 equ    $ - intmsg8
intmsg9: db 'Coprocessor Segment Overrun (reserved)',0
intlen9 equ    $ - intmsg9
intmsg10: db 'Invalid TSS',0
intlen10 equ    $ - intmsg10
intmsg11: db 'Segment Not Present',0
intlen11 equ    $ - intmsg11
intmsg12: db 'Stack-Segmentation Fault',0
intlen12 equ    $ - intmsg12
intmsg13: db 'General Protection',0
intlen13 equ    $ - intmsg13
intmsg14: db 'Page Fault',0
intlen14 equ    $ - intmsg14
intmsg16: db 'General Protection',0
intlen16 equ    $ - intmsg16
intmsg17: db 'General Protection',0
intlen17 equ    $ - intmsg17
intmsg18: db 'General Protection',0
intlen18 equ    $ - intmsg18
intmsg19: db 'General Protection',0
intlen19 equ    $ - intmsg19


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    mov eax, %1
    imprimir_texto_mp intmsg%1, intlen%1, 0x07, 0, 0
    jmp $
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
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

%define DONDE  0x124
%define SOY    0xA6A
%define MAPEAR 0xFF3

%define VIRUS_ROJO 0x841
%define VIRUS_AZUL 0x325


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
        
        
