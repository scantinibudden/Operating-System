/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas asociadas al juego.
*/

#ifndef __GAME_H__
#define __GAME_H__

void game_tick(void);
void game_init(void);

typedef enum direction_e {
  DIR_UP = 0,
  DIR_RIGHT = 1,
  DIR_DOWN = 2,
  DIR_LEFT = 3
} direction_e;

typedef enum move_result_e {
  MOVE_RESULT_SUCCESS = 0,
  MOVE_RESULT_WALL = 1,
  MOVE_RESULT_LAKE = 2,
  MOVE_RESULT_BORDER = 3,
  MOVE_RESULT_LEMMING = 4
} move_result_e;


#endif //  __GAME_H__
