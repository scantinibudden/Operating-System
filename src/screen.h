/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "colors.h"
#include "defines.h"
#include "stdint.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s
{
  uint8_t c;
  uint8_t a;
} ca;

typedef struct registers
{
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
  uint32_t esi;
  uint32_t edi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t eip;
  uint32_t cs;
  uint32_t ds;
  uint32_t es;
  uint32_t fs;
  uint32_t gs;
  uint32_t ss;
  uint32_t eflags;
  uint32_t cr0;
  uint32_t cr2;
  uint32_t cr3;
  uint32_t cr4;
  uint32_t err;
  uint32_t stack1;
  uint32_t stack2;
  uint32_t stack3;
  int8_t exception;
  uint8_t debugger;
} regs_t;

void print(const char *text, uint32_t x, uint32_t y, uint16_t attr);
void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y,
               uint16_t attr);
void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               uint16_t attr);

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize,
                     uint32_t cSize, uint8_t character, uint8_t attr);

void printScanCode(uint8_t scancode);
void print_color(uint32_t x, uint32_t y, uint16_t attr);

void imprimirReloj(uint16_t tarea);

void matarReloj(uint8_t indice, int current);
void ganoJuego(int current);

uint8_t tieneErr(uint32_t code);

#endif //  __SCREEN_H__
