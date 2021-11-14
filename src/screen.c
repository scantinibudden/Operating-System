/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones de impresion por pantalla.
*/

#include "screen.h"
#include "mapa.h"
#include "i386.h"

int lemming_a = 0xDF;
int lemming_b = 0xCF;
int puente = 0x1F;

void print(const char *text, uint32_t x, uint32_t y, uint16_t attr)
{
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  for (i = 0; text[i] != 0; i++)
  {
    p[y][x].c = (uint8_t)text[i];
    p[y][x].a = (uint8_t)attr;
    x++;
    if (x == VIDEO_COLS)
    {
      x = 0;
      y++;
    }
  }
}

//void printChar(const char text, uint32_t x, uint32_t y, uint16_t attr) {
//ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
//p[y][x].c = (uint8_t)text;
//p[y][x].a = (uint8_t)attr;
// x++;
//if (x == VIDEO_COLS) {
// x = 0;
//y++;
// }
//}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr)
{
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  uint32_t i;
  uint8_t letras[16] = "0123456789";

  for (i = 0; i < size; i++)
  {
    uint32_t resto = numero % 10;
    numero = numero / 10;
    p[y][x + size - i - 1].c = letras[resto];
    p[y][x + size - i - 1].a = attr;
  }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               uint16_t attr)
{
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  uint8_t hexa[8];
  uint8_t letras[16] = "0123456789ABCDEF";
  hexa[0] = letras[(numero & 0x0000000F) >> 0];
  hexa[1] = letras[(numero & 0x000000F0) >> 4];
  hexa[2] = letras[(numero & 0x00000F00) >> 8];
  hexa[3] = letras[(numero & 0x0000F000) >> 12];
  hexa[4] = letras[(numero & 0x000F0000) >> 16];
  hexa[5] = letras[(numero & 0x00F00000) >> 20];
  hexa[6] = letras[(numero & 0x0F000000) >> 24];
  hexa[7] = letras[(numero & 0xF0000000) >> 28];
  for (i = 0; i < size; i++)
  {
    p[y][x + size - i - 1].c = hexa[i];
    p[y][x + size - i - 1].a = attr;
  }
}

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize, uint32_t cSize, uint8_t character, uint8_t attr)
{
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = fInit; f < fInit + fSize; f++)
  {
    for (c = cInit; c < cInit + cSize; c++)
    {
      p[f][c].c = character;
      p[f][c].a = attr;
    }
  }
}

void printMap(char mapa[40][80])
{
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  for (int i = 0; i < 40; i++)
  {
    for (int j = 0; j < 80; j++)
    {
      uint8_t attr = 0xAA;
      uint8_t ch = 0x00;
      if (mapa[i][j] == 'P')
      {
        attr = 0x46;
        ch = (uint8_t)'P';
      }
      if (mapa[i][j] == 'L')
        attr = 0x11;
      if (mapa[i][j] == 'A')
      {
        attr = 0x5F;
        ch = (uint8_t)'A';
      }
      if (mapa[i][j] == 'B')
      {
        attr = 0x6F;
        ch = (uint8_t)'B';
      }
      if (mapa[i][j] == '+')
      {
        attr = 0x1F;
        ch = (uint8_t)'+';
      }
      p[i][j].c = ch;
      p[i][j].a = attr;
    }
  }
}

regs_t regs;

void loadRegs(int exception, int32_t stack3, int32_t stack2, int32_t stack1, int16_t ss, int16_t gs, int16_t fs, int16_t es, int16_t ds, int32_t edi, int32_t esi, int32_t ebp, int32_t esp, int32_t ebx, int32_t edx, int32_t ecx, int32_t eax, int32_t err, int32_t eip, int16_t cs, int32_t eflags)
{
  regs.eax = eax;
  regs.ebx = ebx;
  regs.ecx = ecx;
  regs.edx = edx;
  regs.esi = esi;
  regs.edi = edi;
  regs.ebp = ebp;
  regs.esp = esp;
  regs.eip = eip;
  regs.cs = cs;
  regs.ds = ds;
  regs.es = es;
  regs.fs = fs;
  regs.gs = gs;
  regs.ss = ss;
  regs.eflags = eflags;
  regs.cr0 = rcr0();
  regs.cr2 = rcr2();
  regs.cr3 = rcr3();
  regs.cr4 = rcr4();
  regs.err = err;
  regs.stack1 = stack1;
  regs.stack2 = stack2;
  regs.stack3 = stack3;
  regs.exception = exception;
}

uint8_t getDebugger()
{
  return regs.debugger;
}

void toggleDebugger()
{
  if (regs.debugger != 2)
  {
    regs.debugger = (regs.debugger + 1) % 2;
    if (regs.debugger == 1)
    {
      screen_draw_box(0, 19, 40, 40, 0x00, 0x00);

      if (regs.exception != -1)
      {
        print("EXCEPCION ", 20, 1, 0x0A);
        print_dec(regs.exception, 2, 30, 1, 0x0A);
      }
      else
        print("NO OCURRIO EXCEPCION", 20, 1, 0x0A);

      print("eax", 21, 5, 0x0F);
      print_hex(regs.eax, 8, 25, 5, 0x0A);
      print("ebx", 21, 7, 0x0F);
      print_hex(regs.ebx, 8, 25, 7, 0x0A);
      print("ecx", 21, 9, 0x0F);
      print_hex(regs.ecx, 8, 25, 9, 0x0A);
      print("edx", 21, 11, 0x0F);
      print_hex(regs.edx, 8, 25, 11, 0x0A);
      print("esi", 21, 13, 0x0F);
      print_hex(regs.esi, 8, 25, 13, 0x0A);
      print("edi", 21, 15, 0x0F);
      print_hex(regs.edi, 8, 25, 15, 0x0A);
      print("ebp", 21, 17, 0x0F);
      print_hex(regs.ebp, 8, 25, 17, 0x0A);
      print("esp", 21, 19, 0x0F);
      print_hex(regs.esp, 8, 25, 19, 0x0A);
      print("eip", 21, 21, 0x0F);
      print_hex(regs.eip, 8, 25, 21, 0x0A);

      print("cs", 22, 23, 0x0F);
      print_hex(regs.cs, 8, 25, 23, 0x0A);
      print("ds", 22, 25, 0x0F);
      print_hex(regs.ds, 8, 25, 25, 0x0A);
      print("es", 22, 27, 0x0F);
      print_hex(regs.es, 8, 25, 27, 0x0A);
      print("fs", 22, 29, 0x0F);
      print_hex(regs.fs, 8, 25, 29, 0x0A);
      print("gs", 22, 31, 0x0F);
      print_hex(regs.gs, 8, 25, 31, 0x0A);
      print("ss", 22, 33, 0x0F);
      print_hex(regs.ss, 8, 25, 33, 0x0A);

      print("eflags", 21, 36, 0x0F);
      print_hex(regs.eflags, 8, 28, 36, 0x0A);

      print("cr0", 46, 6, 0x0F);
      print_hex(regs.cr0, 8, 50, 6, 0x0A);
      print("cr2", 46, 8, 0x0F);
      print_hex(regs.cr2, 8, 50, 8, 0x0A);
      print("cr3", 46, 10, 0x0F);
      print_hex(regs.cr3, 8, 50, 10, 0x0A);
      print("cr4", 46, 12, 0x0F);
      print_hex(regs.cr4, 8, 50, 12, 0x0A);
      print("err", 46, 14, 0x0F);
      print_hex(regs.err, 8, 50, 14, 0x0A);

      print("stack", 40, 18, 0x0F);
      print_hex(regs.stack1, 8, 40, 20, 0x0A);
      print_hex(regs.stack2, 8, 40, 22, 0x0A);
      print_hex(regs.stack3, 8, 40, 24, 0x0A);
    }
    else
      printMap(mapa_actual);
  }
}

void printScanCode(uint8_t scancode)
{
  if (scancode > 0x01 && scancode < 0x0B)
    print_dec(scancode - 1, 1, VIDEO_COLS - 1, 0, 0x0A);
  if (scancode == 0x0B)
    print_dec(0, 1, VIDEO_COLS - 1, 0, 0x0A);
}

void print_lu()
{
  print("748/19", 38, 44, 0X0A);
  print("797/19", 38, 45, 0X0A);
  print("576/19", 38, 46, 0X0A);
}

void print_color(uint32_t x, uint32_t y, uint16_t attr)
{
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  p[y][x].c = attr;
  p[y][x].a = attr;
}

int contadores[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char *reloj[4] = {"|", "/", "-", "\\"};

void matarReloj(uint8_t indice, int current)
{
  print(0x00, ((current == 0) ? 19 : 43) + 3 * (indice + 1), 43, 0x00);
}

void imprimirReloj(uint16_t tarea)
{
  if (tarea != 0x68)
  {
    uint8_t indice = (tarea % 0x78) / 8;
    print(reloj[contadores[indice] % 4], ((indice % 2 == 0) ? 22 : 46) + 3 * (indice / 2), 43, ((indice % 2 == 0) ? 0x0D : 0x0C));
    contadores[indice]++;
  }
}

void startGame()
{
  printMap(mapa_original);
  loadRegs(-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  regs.debugger = 0;

  print("Amalin", 10, 42, 0x0D);
  print("Betarote", 60, 42, 0x0C);

  for (int i = 1; i < 6; i++)
  {
    print_dec(i, 1, 19 + i * 3, 41, 0x0D);
    print_dec(i, 1, 43 + i * 3, 41, 0x0C);
  }
}

void ganoJuego(int current)
{
  regs.debugger = 2;
  screen_draw_box(5, 20, 30, 40, ' ', 0x00);
  print("GANA EL JUEGO EL EQUIPO:", 29, 16, 0x0A);
  char *equipo_string = (current == 0) ? "AMALIN" : "BETAROTE";
  int lemming_color = (current == 0) ? 0x0D : 0x0C;
  int x = (current == 0) ? 37 : 36;
  print(equipo_string, x, 19, lemming_color);
}
uint8_t tieneErr(uint32_t code)
{
  if (code == 8 || code == 10 || code == 11 || code == 12 || code == 13 || code == 14 || code == 17 || code == 30)
    return 1;
  return 0;
}
