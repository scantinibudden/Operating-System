/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "defines.h"
#include "kassert.h"
#include "mmu.h"
#include "gdt.h"

tss_t tss_initial = {0};
tss_t tss_idle = {
    .ss1 = 0,
    .eip = 0x1c000, //comienzo del codigo de la tarea idle
    .cr3 = 0x25000, //el mismo mapa de paginacion del kernel
    .esp = 0x25000, //misma pila del kernel
    .ebp = 0x25000,
    .cs = GDT_OFF_CS_0, //selector de segmento de codigo nivel cero
    .ds = GDT_OFF_DS_0, //selector de segmento de datos de nivel cero
    .es = GDT_OFF_DS_0,
    .fs = GDT_OFF_DS_0,
    .gs = GDT_OFF_DS_0,
    .ss = GDT_OFF_DS_0,
    .eflags = 0x202, //interrupciones activas
    .iomap = 0x68
};

tss_t tss_array[10] = {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};
int16_t gdt_array[10] = {LEMMING_0,LEMMING_1,LEMMING_2,LEMMING_3,LEMMING_4,LEMMING_5,LEMMING_6,LEMMING_7,LEMMING_8,LEMMING_9};

void tss_init(void){

  gdt[TSS_INIT].base_15_0 = (uint32_t)&tss_initial | 0;
  gdt[TSS_INIT].base_23_16 = ((uint32_t)(&tss_initial) >> 16) | 0;
  gdt[TSS_INIT].base_31_24 = ((uint32_t)(&tss_initial) >> 24) | 0;

  gdt[TSS_IDLE].base_15_0 = (uint32_t)&tss_idle | 0;
  gdt[TSS_IDLE].base_23_16 = ((uint32_t)(&tss_idle) >> 16) | 0;
  gdt[TSS_IDLE].base_31_24 = ((uint32_t)(&tss_idle) >> 24) | 0;

}

void task_init(int8_t num){
  
  uint32_t cr3 = mmu_init_task_dir(((num % 2) == 0) ? TASK_LEMMING_A_CODE_START : TASK_LEMMING_B_CODE_START);

  tss_array[num].ss0 = GDT_OFF_DS_0;
  tss_array[num].esp0 = mmu_next_free_kernel_page() + PAGE_SIZE - 1;

  tss_array[num].eip = TASK_CODE_VIRTUAL;
  tss_array[num].esp = TASK_STACK_BASE;
  tss_array[num].cr3 = cr3;

  tss_array[num].cs = GDT_OFF_CS_3 | 3;
  tss_array[num].ds = GDT_OFF_DS_3 | 3;
  tss_array[num].es = GDT_OFF_DS_3 | 3;
  tss_array[num].fs = GDT_OFF_DS_3 | 3;
  tss_array[num].gs = GDT_OFF_DS_3 | 3;
  tss_array[num].ss = GDT_OFF_DS_3 | 3;
  
  tss_array[num].ebp = TASK_STACK_BASE;
  tss_array[num].eflags = 0x202;
  tss_array[num].iomap = 0x68;

  gdt[gdt_array[num]].base_15_0 = (uint32_t)&tss_array[num] | 0;
  gdt[gdt_array[num]].base_23_16 = ((uint32_t)(&tss_array[num]) >> 16) | 0;
  gdt[gdt_array[num]].base_31_24 = ((uint32_t)(&tss_array[num]) >> 24) | 0;
}