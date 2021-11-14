/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__
/* MMU */
/* -------------------------------------------------------------------------- */

#define MMU_P (1 << 0)
#define MMU_W (1 << 1)
#define MMU_U (1 << 2)

#define MMU_FLAG_PRESENT 1
#define MMU_FLAG_SUPERVISOR 0
#define MMU_FLAG_READWRITE 1

#define PAGE_SIZE 4096
#define INICIO_DE_PAGINAS 0x100000

/* Misc */
/* -------------------------------------------------------------------------- */
// Y Filas
#define SIZE_N 40
#define ROWS   SIZE_N

// X Columnas
#define SIZE_M 80
#define COLS   SIZE_M

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC 0
#define GDT_IDX_CS_0 8
#define GDT_IDX_DS_0 9
#define GDT_IDX_CS_3 10
#define GDT_IDX_DS_3 11
#define GDT_IDX_VS_0 12
#define TSS_IDLE 13
#define TSS_INIT 14
#define LEMMING_0 15
#define LEMMING_1 16
#define LEMMING_2 17
#define LEMMING_3 18
#define LEMMING_4 19
#define LEMMING_5 20
#define LEMMING_6 21
#define LEMMING_7 22
#define LEMMING_8 23
#define LEMMING_9 24
#define GDT_COUNT 35



/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC (GDT_IDX_NULL_DESC << 3)
#define GDT_OFF_CS_0 (GDT_IDX_CS_0 << 3)
#define GDT_OFF_DS_0 (GDT_IDX_DS_0 << 3)
#define GDT_OFF_CS_3 (GDT_IDX_CS_3 << 3)
#define GDT_OFF_DS_3 (GDT_IDX_DS_3 << 3)
#define GDT_OFF_VS_0 (GDT_IDX_VS_0 << 3)
#define TSS_IDLE_OFF (TSS_IDLE << 3)
#define TSS_IDLE_INIT (TSS_INIT << 3)
#define LEMMING_0_OFF (LEMMING_0 << 3)
#define LEMMING_1_OFF (LEMMING_1 << 3)
#define LEMMING_2_OFF (LEMMING_2 << 3)
#define LEMMING_3_OFF (LEMMING_3 << 3)
#define LEMMING_4_OFF (LEMMING_4 << 3)
#define LEMMING_5_OFF (LEMMING_5 << 3)
#define LEMMING_6_OFF (LEMMING_6 << 3)
#define LEMMING_7_OFF (LEMMING_7 << 3)
#define LEMMING_8_OFF (LEMMING_8 << 3)
#define LEMMING_9_OFF (LEMMING_9 << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */

// direccion fisica de comienzo del bootsector (copiado)
#define BOOTSECTOR 0x00001000
// direccion fisica de comienzo del kernel
#define KERNEL 0x00001200
// direccion fisica del buffer de video
#define VIDEO 0x000B8000

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */

// direccion virtual del codigo
#define TASK_CODE_VIRTUAL 0x08000000
#define TASK_CODE_PAGES   2
#define TASK_STACK_BASE   0x08003000


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<X>_PHY_START.
 */
#define USER_TASK_SIZE (PAGE_SIZE * 2)

#define TASK_LEMMING_A_CODE_START (0x00018000)
#define TASK_LEMMING_B_CODE_START (0x0001A000)
#define TASK_IDLE_START           (0x0001C000)

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR     (0x00025000)
#define KERNEL_PAGE_TABLE_0 (0x00026000)
#define KERNEL_STACK        (0x00025000)

#endif //  __DEFINES_H__
