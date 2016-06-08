; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

extern screen_inicializar
extern atender_teclado
extern game_mapear
extern game_donde
extern game_soy

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


      
offset: dd 0
selector: dw 0


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

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

global _isr32
_isr32:
    pushad

	call proximo_reloj
	call sched_proximo_indice
    cmp ax, 0
    je  .nojump
        mov [sched_tarea_selector], ax
        call fin_intr_pic1
        xchg bx, bx
        jmp far [sched_tarea_offset]
        xchg bx, bx
	    jmp .end
	.nojump:
        call fin_intr_pic1
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

    call screen_inicializar
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

    .SOY:
    cmp eax,SOY
    jne .MAPEAR
    call game_soy

    .MAPEAR:
    cmp eax, MAPEAR
    jne .MELLAMARONMAL
    push ecx
    call game_mapear
    pop ecx

    .MELLAMARONMAL:
    mov dword [esp + ADDR_SIZE*OFFSET_ECX], 0x42

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
        
        
