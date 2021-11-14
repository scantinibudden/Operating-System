#include "syscall.h"
#include "types.h"

#define TASK_FIRST_PAGE 0x08000000
#define TASK_SECOND_PAGE 0x08001000
#define TASK_THIRD_PAGE 0x08002000
#define TASK_STACK_BASE 0x08003000

// Chequear que el resultado de una operacion sea esperado.
// En caso de que falle, se generara un breakpoint.
#define CHECK_EQ(A, B)                                                         \
  do {                                                                         \
    if ((A) != (B)) {                                                          \
      __asm__ volatile("xchg %bx, %bx");                                       \
    }                                                                          \
  } while (0)

// Hacer que el Lemming produzca una excepcion.
#define SEPPUKKU(...)                                                          \
  do {                                                                         \
    __asm__ volatile("ud2");                                                   \
  } while (0)

static inline void read_addr(uintptr_t addr);
static inline void write_addr(uintptr_t addr);

void task(void) {
  // La tarea corre desde la dire virtual 0x8000000
  CHECK_EQ((uintptr_t)&task, TASK_FIRST_PAGE);

  // El stack esta en TASK_STACK_BASE
  uintptr_t stack = 0x41414141;
  __asm__ volatile("mov %%esp, %[stack]" : [stack] "=rm"(stack));
  CHECK_EQ(stack > TASK_THIRD_PAGE, true);
  CHECK_EQ(stack <= TASK_STACK_BASE, true);

  // Las tres paginas de la tarea son accesibles para lectura.
  read_addr(TASK_FIRST_PAGE);
  read_addr(TASK_SECOND_PAGE);
  read_addr(TASK_THIRD_PAGE);

  // Si llegamos hasta aca sin crashear,
  // vamos al medio de la pantalla
  // Y tratamos de escribir sobre la pagina del codigo.
  // Deberia generar una falla.

  for (int i = 0; i < 20; i++)
    syscall_move(DOWN);

  for (int i = 0; i < 20; i++)
    syscall_move(LEFT);

  write_addr(TASK_FIRST_PAGE);

  while (1) {
    __asm__ volatile("pause");
  }
}

static inline void read_addr(uintptr_t addr) {
  uint8_t discard;

  __asm__ volatile("mov (%[addr]), %[discard]"
                   : [discard] "=r"(discard)
                   : [addr] "r"(addr));
}

static inline void write_addr(uintptr_t addr) {
  uint16_t discard = 0x4141;
  __asm__ volatile("mov %[discard], (%[addr])"
                   :
                   : [discard] "r"(discard), [addr] "r" (addr));
}
