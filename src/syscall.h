/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  Definicion de llamadas a sistema, para usar desde las tareas.
*/

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "stdbool.h"
#include "stdint.h"

#define LS_INLINE static __inline __attribute__((always_inline))
#define NORETURN  __attribute__((noreturn))

#define UNREACHABLE()                                                          \
  while (1) {                                                                  \
    __asm__ volatile("pause" ::: "memory");                                    \
  }

/*
 * Syscalls
 */

typedef enum direction_e { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 } direction_e;

typedef enum move_result_e {
  SUCCESS = 0,
  WALL = 1,
  LAKE = 2,
  BORDER = 3,
  LEMMING = 4
} move_result_e;

LS_INLINE move_result_e syscall_move(direction_e direction) {
  int32_t ret;
  __asm__ volatile("int $88"        /* interrupt 88 */
                   : "=a"(ret)      /* return value in eax ("a") */
                   : "a"(direction) /* pass in eax ("a") arg "directon"*/
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
  return ret;
}

NORETURN LS_INLINE void syscall_explode(void) {
  __asm__ volatile("int $98" /* interrupt 98 */
                   :
                   :
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
  UNREACHABLE();
}

NORETURN LS_INLINE void syscall_bridge(direction_e direction) {
  __asm__ volatile("int $108" /* interrupt 108 */
                   :
                   : "a"(direction) /* pass in eax ("a") arg "directon"*/
                   : "memory",
                     "cc"); /* announce to the compiler that the memory and
                               condition codes have been modified */
  UNREACHABLE();
}

#endif //  __SYSCALL_H__
