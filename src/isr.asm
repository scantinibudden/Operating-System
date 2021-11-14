; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones

%include "print.mac"

BITS 32

sched_task_offset:     dd 0xFFFFFFFF
sched_task_selector:   dw 0xFFFF

;; PIC
extern pic_finish1

;; Sched
extern sched_next_task
extern print_excepcion
extern printScanCode
extern imprimirReloj
extern desalojar
extern toggleDebugger
extern getDebugger
extern moverLemming
extern lemmingPuente
extern loadRegs
extern explotarLemming
extern guardarMov
extern obtenerMov
extern pageFault
extern tieneErr

%define TSS_IDLE_OFF 0x68


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    pushad

    mov eax, %1
    push eax
    call tieneErr
    add esp, 4
    cmp al, 1
    je .err

    popad
    sub esp, 4
    mov dword [esp], 0xDEAD                     ;usamos dead para los que no tienen error code
    pushad

    .err:
    push ds
    push es
    push fs
    push gs
    push ss
    mov eax, [esp + 72]
    mov ecx, [eax + 4]
    mov edx, [eax + 8]
    mov eax, [eax]
    push eax
    push ecx
    push edx
    mov eax, %1
    push eax
    call loadRegs
    add esp, 36
    mov eax, %1
    push eax
    call print_excepcion
    add esp, 4
    mov eax, %1
    cmp eax, 14
    jne .continue
    ;llamamos a la funcion pageFault
    call pageFault
    jmp .fin
    .continue:
    call desalojar
    .fin:
    mov word [sched_task_selector], 0x68
    jmp far [sched_task_offset]
    popad
    add esp, 4
    iret

%endmacro

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
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
        pushad
        call pic_finish1        ;se recibio la interrupcion
        call getDebugger
        cmp ax, 0x00
        jne .fin
        
        call next_clock         ;imprimir el reloj del sistema
        call sched_next_task

        str cx
        cmp ax, cx
        je .fin

        mov word [sched_task_selector], ax

        push ax
        call imprimirReloj
        add esp, 2
        
        jmp far [sched_task_offset]
        .fin:
        popad
        iret

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
        pushad
        in al, 0x60
        cmp eax, 0x15           ; si el input == y
        jne .next
        call toggleDebugger
        jmp .fin
        .next:
        push eax
        call printScanCode
        add esp, 4
        .fin:
        call pic_finish1
        popad
        iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
resultado: dd 0x0

global _isr88
_isr88:
    ;xchg bx, bx 
    pushad 
    push eax
    call moverLemming
    push eax
    call guardarMov
    add esp, 8
    mov word [sched_task_selector], 0x68
    jmp far [sched_task_offset]
    call obtenerMov
    mov [resultado], eax
    popad
    mov eax, [resultado]
    iret

global _isr98
_isr98:
    pushad
    call explotarLemming
    mov word [sched_task_selector], 0x68
    jmp far [sched_task_offset]
    popad
    iret

global _isr108
_isr108:
    pushad
    push eax
    call lemmingPuente
    add esp, 4
    mov word [sched_task_selector], 0x68
    jmp far [sched_task_offset]
    popad
    iret

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
next_clock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
        popad
        ret
