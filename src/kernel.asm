; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

extern GDT_DESC
extern idt_init
extern print_excepcion
extern IDT_DESC
extern pic_enable
extern pic_reset
extern mmu_init_kernel_dir
extern print_lu
extern mmu_init
extern mmu_map_page
extern mmu_unmap_page
extern mmu_init_task_dir
extern print_color
extern tss_init
extern task_init
extern sched_init
extern startGame

%define CS_0 0x40
%define DS_0 0x48
%define CS_3 0x50
%define DS_3 0x58
%define VS_0 0x60
%define TSS_IDLE 0x68
%define TSS_INIT 0x70
%define LEMMING_0 0x78
%define LEMMING_1 0x80
%define LEMMING_2 0x88
%define LEMMING_3 0x90
%define LEMMING_4 0x98
%define LEMMING_5 0xA0
%define LEMMING_6 0xA8
%define LEMMING_7 0xB0
%define LEMMING_8 0xB8
%define LEMMING_9 0xC0
%define EBP_INIT 0x25000
%define ESP_INIT 0x25000


global start


BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

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

    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0

    ; --------------------------
    ; BREAKPOINT INICIO
    ; xchg bx, bx
    ; --------------------------

    ; Habilitar A20

    call A20_disable
    call A20_check
    call A20_enable
    call A20_check  

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Saltar a modo protegido
    jmp CS_0:modoProtegido
    

BITS 32
modoProtegido:
    ; Establecer selectores de segmentos

    mov ax, DS_0
    mov ds, ax
    mov ax, CS_3
    mov es, ax
    mov ax, DS_3
    mov fs, ax
    mov ax, VS_0
    mov gs, ax
    mov ax, DS_0
    mov ss, ax

    ; --------------------------
    ; SREG
    ; cs -> cs0
    ; ds -> ds0
    ; es -> cs3
    ; fs -> ds3
    ; gs -> vs0
    ; ss -> ds0
    ; --------------------------

    ; Establecer la base de la pila
    mov ebp, EBP_INIT
    mov esp, ESP_INIT

    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 0, 0

    ; Inicializar pantalla
    ; cada pixel es de 2 bytes, la pantalla completa es de 80x50 y el mapa de 80x40
    mov ecx, 0

    ;aca pintamos el mapa
    .cicloMapa:
    cmp ecx, 6400
    je .seguirCicloPrimerEquipo
    mov byte [gs:0x0000 + ecx], 0x00
    inc ecx
    mov byte [gs:0x0000 + ecx], 0xBB
    inc ecx
    jmp .cicloMapa

    ;ancho = 10
    ;altura = 4
    ;aca pintamos los rectangulos para cada equipo,cada uno de 10x4
    ;para el primero 240 x 2 + 10 x2 -> 6400 + 480 + 20 = 6900
    .seguirCicloPrimerEquipo:
    mov ecx, 6900
    mov ebx, 0

    .ciclo:
    cmp ebx, 4
    je .pantallaColorida
    mov eax, ecx
    add eax, 100
    mov edx, 0

    .cicloPrimerEquipo:
    cmp edx, 20
    je .sigFila
    mov byte [gs:0x0000 + ecx + edx], 0x00          ;rect1
    mov byte [gs:0x0000 + eax + edx], 0x00          ;rect2
    inc edx
    mov byte [gs:0x0000 + ecx + edx], 0xDD          ;rect1
    mov byte [gs:0x0000 + eax + edx], 0xEE          ;rect2
    inc edx 
    jmp .cicloPrimerEquipo

    .sigFila:
    inc ebx
    add ecx, 160
    jmp .ciclo

    .pantallaColorida:
    
    ; Inicializar el manejador de memoria
    call mmu_init
     
    ; Inicializar el directorio de paginas
    call mmu_init_kernel_dir
    
    ; Cargar directorio de paginas
    mov eax, 0x25000
    mov cr3, eax
    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    call startGame

    call print_lu
    
    ; Inicializar tss
    ; Inicializar tss de la tarea Idle
    call tss_init

    ; Inicializar el scheduler
    call sched_init
 
    ; Inicializar la IDT
    call idt_init           ;inicializamos idt
 
    ; Cargar IDT
    lidt [IDT_DESC]         ;cargamos idt

    ; Configurar controlador de interrupciones
    call pic_reset           
    call pic_enable

    ; Cargar tarea inicial
    xor eax, eax
    mov ax, TSS_INIT
    ltr ax
    sti                     ;habilitamos interrupciones

    call pic_reset
    call pic_enable

    ;inicializar en cero ??????
    ;push ecx
    ;mov ecx, 0
    ;.initPag:
    ;mov byte [0x400000], 0x0
    ;add ecx, 1
    ;cmp ecx, 4096
    ;jne .initPag
    ;pop ecx


    ;Saltar a la primera tarea: Idle
    jmp TSS_IDLE:0x0

    ; --------------------------------------------
    ; xchg bx, bx
    ; push 0x00018000
    ; push 4
    ; call task_init
    ; add esp, 4*2
    ; jmp LEMMING_4:0x0
    ; ---------------------------------------------
    ; PRUEBA MAP PAGE
    ; push 3                ;attrs 011 
    ; push 0x00400000       ;phy
    ; push 0x0050E000       ;alinear a 4k
    ; mov eax, cr3
    ; push eax  
    ; call mmu_map_page
    ; add esp, 4*4
    ; mov byte [0x0050E027], 0x1
    ; ------------------------------
    ; PRUEBA UNMAP PAGE
    ; push 0x0050E000       ;alinear a 4k
    ; mov eax, cr3
    ; push eax  
    ; call mmu_unmap_page
    ; add esp, 4*2
    ; ------------------------------
    ; PRUEBA INIT TASK DIR
    ; xchg bx, bx
    ; push 0x00600000
    ; call mmu_init_task_dir
    ; add esp, 4
    ; mov esp, 0x08003000
    ; ------------------------------
    ; EJERCICIO 5)D)
    ; mov edx, cr3
    ; mov cr3, eax
    ; push 0x0A
    ; push 0
    ; push 0
    ; call print_color
    ; add esp, 4*3
    ; mov cr3, edx
    ; mov esp, 25000
    ; ------------------------------------------------

    ; Ciclar infinitamente (por si algo sale mal...)
    
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"