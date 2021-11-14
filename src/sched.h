/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Dyeclaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"


//0 = segmento, 1 = vejez, 2 = x, 3 = y
typedef struct team{
  int32_t lemmings[5][4];
  int8_t index;
  int8_t next;
  int8_t total;
  // // direcciones[i][0] = direccion fisica, direccines[i][1] = cr3
  // int32_t direcciones[4096][2];
  // int next_dir;
} __attribute__((__packed__, aligned(8))) team_t;

typedef struct sched{
team_t a;
team_t b;
int8_t current_team;
int32_t idle;
int32_t quantum_counter;
} __attribute__((__packed__, aligned(8))) sched_t;

void printTotales();


void sched_init();
uint16_t sched_next_task();
int32_t moverLemming(uint32_t dir);
void explotarLemming();
void desalojar();
void guardarMov(int eax);
int obtenerMov();
void pageFault();

#endif //  __SCHED_H__
