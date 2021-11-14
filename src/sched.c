/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

// Cada 401 ciclos de reloj, si algún equipo tiene menos de 5 Lemmings en juego, el sistema creará
// un nuevo Lemming para ese equipo, posicionándolo en la dirección inicial del mapa para su equipo.

// Cada 2001 ciclos de reloj, si algún equipo tiene 5 Lemmings en juego, el sistema desalojará al
// Lemming más antiguo y creará uno nuevo, posicionandolo en la dirección inicial del mapa para su
// equipo.

// Si por algún motivo la dirección inicial del mapa para un equipo está ocupada, y esto no le permite
// crear el Lemming, la creación falla y no será creado un Lemming.

// lemming 1A - mov
// corre hasta jump far -> guardamos el eax antes
// lemming 1B - mov 
// corre hasta jmp far -> guardamos
// corren todos los q faltan
// lemming 1A - mov
// retoma despues del jmp far, tenemos que devolver el eax anterior 


#include "sched.h"
#include "tss.h"
#include "i386.h"
#include "screen.h"
#include "mapa.h"
#include "syscall.h"
#include "mmu.h"

#define TASK_SHARED_MEM_START 0x00400000

sched_t sched;

char mapa_actual[40][80];

void inicializarMapaActual(){
  for (int i = 0; i < 40; i++){
    for (int j = 0; j < 80; j++){
      mapa_actual[i][j] = mapa_original[i][j];
    } 
  }
}

int resultadosA[5] = {0,0,0,0,0};
int resultadosB[5] = {0,0,0,0,0};

void guardarMov(int eax){
  if (sched.current_team == 0){
    resultadosA[sched.a.index] = eax;
  } else {
    resultadosB[sched.b.index] = eax;
  }
}

int obtenerMov(){
  if(sched.current_team == 0)
    return resultadosA[sched.a.index];
  else
    return resultadosB[sched.b.index];
}

//GANO JUEGO
bool gano(team_t* team, int current){
  if (current == 0){
    if(team->lemmings[team->index][2] == 79){
      return true;
    };
  } else {
    if (team->lemmings[team->index][2] == 0){
      return true;
    }
  }
  return false;
}
//PRINT ANTERIOR
void print_anterior(int x, int y)
{
  if (mapa_original[y][x] == '.'){
    char *posChar = ".";
    print(posChar, x, y, 0x22);
  }
  else if (mapa_original[y][x] == '+'){
    char *posChar = "+";
    print(posChar, x, y, 0x1F);
  }
  else if (mapa_original[y][x] == 'L'){
    char *posChar = "L";
    print(posChar, x, y, 0x11);
  }
  // if (mapa_original[y][x] == '.'){
  //   char *posChar = ".";
  //   print(posChar, x, y, 0x22);
  // }
}

//INICIALIZAR LEMMING
void imprimirLemmingInicial(int current){
  if (current == 0)
  {
    print_dec(sched.a.total, 10, 10, 44, 0x5F);
    print("A", 0, 39, 0x5F);
    mapa_actual[39][0] = 'A';
  }

  if (current == 1)
  {
    print_dec(sched.b.total, 10, 60, 44, 0x6F);
    print("B", 79, 0, 0x6F);
    mapa_actual[0][79] = 'B';
  }
}

int lemmingMuerto(team_t* team){
  for (int i = 0; i < 5; i++){
    if (team->lemmings[i][1] == 0)
      return i;
  }
  return -1;
}

void inicializarLemming(team_t* team, int current){

  bool inicioLibre = (current == 0) ? mapa_actual[39][0] == '.' : mapa_actual[0][79] == '.';

  if (inicioLibre && team->next < 6){
    int index = lemmingMuerto(team);
    task_init((team->lemmings[index][0] % 0x78)/8);
    team->lemmings[index][1] = team->next;
    team->next++;
    team->lemmings[index][2] = (current == 0) ? 0 : 79;
    team->lemmings[index][3] = (current == 0) ? 39 : 0;
    team->total++;

    imprimirLemmingInicial(current);
  }
}

//DESALOJAR LEMMING MAS VIEJO
void desalojarMasViejo(team_t* team, int current){
  //breakpoint();
  uint8_t masViejo = 0;

  //BUSCAMOS EL LEMMING MAS VIEJO Y SI UN LEMMING NO ESTA MUERTO LE RESTAMOS 1 A SU EDAD
  for (int i = 0; i < 5; i++){
    if (team->lemmings[i][1] == 1){
      masViejo = i;
      if (team->index == i){
        team->index--;
        team->index = team->index % 5;
      }
      
    }
    if (team->lemmings[i][1] > 0){
      team->lemmings[i][1]--;
    }
  }
  
  //DEJAMOS DE IMPRIMIR EL CLOCK DEL LEMMING
  matarReloj(masViejo, current);

  //PONEMOS EN EL MAPA LO QUE HABIA ANTES DEL LEMMING
  int x = team->lemmings[masViejo][2];
  int y = team->lemmings[masViejo][3];
  print_anterior(x, y);
  mapa_actual[y][x] = mapa_original[y][x];

  //LA EDAD DEL SIGUIENTE LEMMING DECRECE
  team->next--;
  //breakpoint();
  //REVIVIMOS UN LEMMING
  inicializarLemming(team, current);
}

//ACCIONES LEMMING///////////////////////////////////////////
int posicionQueHay(int x, int y)
{
  if (mapa_actual[y][x] == 'P'){
    return 1;
  } else if (mapa_actual[y][x] == 'L'){
    return 2;
  } else if (mapa_actual[y][x] == 'A'){
    return 4;
  } else if (mapa_actual[y][x] == 'B'){
    return 4;
  } else {
    return 0; //HAY PASTO O PUENTE
  }  
}


void desalojarLemmingVictima(team_t *team, int index, int current)
{
  //lo sacamos del mapa
  int x = team->lemmings[index][2];
  int y = team->lemmings[index][3];

  mapa_actual[y][x] = mapa_original[y][x];
  print_anterior(x, y);

  //guardamos la edad del lemming
  int8_t edad = team->lemmings[index][1];

  //sacamos su reloj
  matarReloj(index, current);

  //lo registramos como muerto
  team->lemmings[index][1] = 0;

  //le restamos uno a la edad de los lemmings mayores al actual
  for (int i = 0; i < 5; i++)
  {
    if (team->lemmings[i][1] > edad)
      team->lemmings[i][1]--;
  }

  //le restamos uno a la edad del proximo que hay que revivir
  team->next--;
}

void desalojar()
{
  team_t *team = (sched.current_team == 0) ? &sched.a : &sched.b;
  int index = team->index;
  int current = sched.current_team;

  desalojarLemmingVictima(team, index, current);
}

//MOVER LEMMING
int moverLemming(direction_e dir){

  team_t *team = (sched.current_team == 0) ? &sched.a : &sched.b;

  uint32_t x = team->lemmings[team->index][2];
  uint32_t y = team->lemmings[team->index][3];

  int nueva_x = x;
  int nueva_y = y;

  switch (dir){
  case 2: //DOWN
    nueva_y = y + 1;
    break;
  case 0: //UP
    nueva_y = y - 1;
    break;
  case 1: //RIGHT
    nueva_x = x + 1;
    break;
  case 3: //LEFT
    nueva_x = x - 1;
    break;
  }
   
  int posicion_siguiente = posicionQueHay(nueva_x, nueva_y);

  //caso se pasa del borde

  if (nueva_x == -1 || nueva_x == 80 || nueva_y == -1 || nueva_y == 40){
    posicion_siguiente = 3;
  } else if (posicion_siguiente == 0){
    //IMPRIMO EN MAPA EL NUEVO LEMMING
    char* lemming_string = (sched.current_team == 0) ? "A" : "B";
    int color_lemming = (sched.current_team == 0) ? 0x5F : 0X6F;
    print(lemming_string, nueva_x, nueva_y, color_lemming);
    //CAMBIO DONDE ANTES ESTABA EL LEMMING
    print_anterior(x, y);

    //CAMBIO LAS VARIABLES MAPA
    mapa_actual[y][x] = mapa_original[y][x];
    mapa_actual[nueva_y][nueva_x] = (sched.current_team == 0) ? 'A' : 'B';

    //CAMBIO X E Y DEL LEMMING
    team->lemmings[team->index][2] = nueva_x;
    team->lemmings[team->index][3] = nueva_y;
  }

  if (nueva_x == 0 || nueva_x == 79 || nueva_y == 0 || nueva_y == 39){
    if (gano(team, sched.current_team)){
      ganoJuego(sched.current_team);
    }
  }
  
  //print_dec(posicion_siguiente, 5, 0, 2, 0x0a);
  return posicion_siguiente;
}

//HACER PUENTE DE LEMMING
void lemmingPuente(direction_e dir){
  team_t *team = (sched.current_team == 0) ? &sched.a : &sched.b;

  uint32_t x = team->lemmings[team->index][2];
  uint32_t y = team->lemmings[team->index][3];

  int nueva_x = x;
  int nueva_y = y;

  switch (dir)
  {
  case 2: //DOWN
    nueva_y = y + 1;
    break;
  case 0: //UP
    nueva_y = y - 1;
    break;
  case 1: //RIGHT
    nueva_x = x + 1;
    break;
  case 3: //LEFT
    nueva_x = x - 1;
    break;
  }

  int posicion_siguiente = posicionQueHay(nueva_x, nueva_y);

  if (posicion_siguiente == 2){
    //cambio en pantalla
    print_anterior(x, y);
    print("+", nueva_x, nueva_y, 0x1F);

    //cambio en mapas
    mapa_original[nueva_y][nueva_x] = '+';
    mapa_actual[nueva_y][nueva_x] = '+';
    mapa_actual[y][x] = mapa_original[y][x];
  }

  //desalojo lemming
  desalojarLemmingVictima(team, team->index, sched.current_team);
}

//EXPLOTAR LEMMING
void cambiarDireccion(int x, int y){

  //SI HAY PARED, LA EXPLOTAMOS
  if (mapa_actual[y][x] == 'P'){
    //ponemos pasto donde estaba la pared
    mapa_original[y][x] = '.';
    mapa_actual[y][x] = '.';
    //imprimimos pasto en el mapa
    print(".", x, y, 0x22);
  }
  //SI HAY UN LEMMING A, LO EXPLOTAMOS
  else if (mapa_actual[y][x] == 'A'){

    //buscamos que lemming esta en esa direccion
    for (int i = 0; i < 5; i++){
      if (sched.a.lemmings[i][2] == x && sched.a.lemmings[i][3] == y){
        //desalojamos al lemming que esta ahi
        desalojarLemmingVictima(&sched.a, i, 0);
        break;
      }
    }
    //ponemos en el mapa actual lo que antes habia ahi
    mapa_actual[y][x] = mapa_original[y][x];
    //print_anterior(x, y);
  }
  //SI HAY UN LEMMING B, LO EXPLOTAMOS
  else if (mapa_actual[y][x] == 'B'){

    //buscamos que lemming esta en esa direccioon
    for (int i = 0; i < 5; i++){
      if (sched.b.lemmings[i][2] == x && sched.b.lemmings[i][3] == y){
        //lo desalojamos
        desalojarLemmingVictima(&sched.b, i, 1);
        break;
      }
    }
    //cambiamos mapa actual
    mapa_actual[y][x] = mapa_original[y][x];
    //print_anterior(x, y);
  }
}

void explotarLemming(){

  team_t *team = (sched.current_team == 0) ? &sched.a : &sched.b;
  uint32_t x = team->lemmings[team->index][2];
  uint32_t y = team->lemmings[team->index][3];

  //cambiar mapas
  cambiarDireccion(x + 1, y);
  cambiarDireccion(x - 1, y);
  cambiarDireccion(x, y + 1);
  cambiarDireccion(x, y - 1);

  mapa_actual[x][y] = mapa_original[x][y];
  desalojarLemmingVictima(team, team->index, sched.current_team);
  //breakpoint();
}

//INICIALIZAR SCHEDULER
void sched_init(void){

  inicializarMapaActual();

  //inicializamos offsets equipo a
  sched.a.lemmings[0][0] = LEMMING_0_OFF;
  sched.a.lemmings[1][0] = LEMMING_2_OFF;
  sched.a.lemmings[2][0] = LEMMING_4_OFF;
  sched.a.lemmings[3][0] = LEMMING_6_OFF;
  sched.a.lemmings[4][0] = LEMMING_8_OFF;
  sched.a.total = 0;
  sched.a.next = 1;
  sched.a.index = -1;

  //equipo b
  sched.b.lemmings[0][0] = LEMMING_1_OFF;
  sched.b.lemmings[1][0] = LEMMING_3_OFF;
  sched.b.lemmings[2][0] = LEMMING_5_OFF;
  sched.b.lemmings[3][0] = LEMMING_7_OFF;
  sched.b.lemmings[4][0] = LEMMING_9_OFF;
  sched.b.total = 0;
  sched.b.next = 1;
  sched.b.index = -1;


  //inicializamos edades y posiciones en 0
  for (int i = 0; i < 5; i++)
  {
    sched.a.lemmings[i][1] = 0;
    sched.a.lemmings[i][2] = 0;
    sched.a.lemmings[i][3] = 0;
    sched.b.lemmings[i][1] = 0;
    sched.b.lemmings[i][2] = 0;
    sched.b.lemmings[i][3] = 0;
  }

  //inicializamos los otros atributos de sched
  sched.idle = 0x68;          //offset idle
  sched.current_team = -1;    //0 = a, 1 = b
  sched.quantum_counter = -1; //contador de quantums

}

//NEXT TASK SCHEDULER
uint16_t sched_next_task(void){

  //SUMAMOS 1 AL CONTADOR DE QUANTUMS
  sched.quantum_counter++;

  //SI PASARON 401 CLOCKS REVIVIMOS LEMMING
  if (sched.quantum_counter % 401 == 0){
    inicializarLemming(&sched.a, 0);
    inicializarLemming(&sched.b, 1);
  }

  //SI PASARON 2001 CLOCKS MATAMOS AL MAS VIEJO
  if ((sched.quantum_counter % 2001 == 0) && sched.quantum_counter != 0){
    if (sched.a.next == 6)
      desalojarMasViejo(&sched.a, 0);
    if (sched.b.next == 6)
      desalojarMasViejo(&sched.b, 1);
  }

  //ACTUALIZAMOS EL CURRENT TEAM
  sched.current_team = (sched.current_team + 1) % 2;
  team_t* team = (sched.current_team == 0) ? &sched.a : &sched.b;

  //NOS FIJAMOS QUE HAYA LEMMINGS EN EL TEAM, SI NEXT ES 1 NO HAY LEMMINGS EN EL TEAM Y CAMBIAMOS AL OTRO
  if (team->next == 1){
    sched.current_team = (sched.current_team + 1) % 2;
    team = (sched.current_team == 0) ? &sched.a : &sched.b;
  }

  //NOS FIJAMOS SI EN EL TEAM HAY LEMMINGS
  if (team->next == 1){
    return sched.idle;
  }
  
  //INCREMENTAMOS EL INDEX DEL TEAM ACTUAL
  team->index++;
  team->index %=5;

  if (team->lemmings[team->index][1] == 0){
    while (team->lemmings[team->index][1] == 0){
      team->index++;
      team->index %= 5;
    }    
  }
  
  return team->lemmings[team->index][0];
  
}

int32_t array_a[4096][2];
int32_t array_b[4096][2];

int next_a = 0;
int next_b = 0;

int dirMapped(int32_t dir){

  int next = (sched.current_team == 0) ? next_a : next_b;

  for (int i = 0; i < next; i++){

    int mapped = (sched.current_team == 0) ? array_a[i][0] : array_b[i][0];
    int phy = (sched.current_team == 0) ? array_a [i][1] : array_b[i][1];

    if (dir == mapped)
      return phy;
  }
  return -1;
}

void pageFault(){
  //nos pasan la direccion lineal donde fallo
  int32_t dir = rcr2();

  if (dir >= 0x400000 && dir <= 0x13FFFFF){
  
    int32_t cr3 = rcr3();

    //buscamos si esta mapeada, si lo esta tenemos la fisica, si no lo esta tenemos -1
    int32_t phy = dirMapped(dir >> 12);

    //SI YA ESTA MAPEADA
    if (phy != -1){
      //la mapeamos a esa dir fisica
      mmu_map_page(cr3, dir, phy, 0x7);

    //SI NO ESTA MAPEADA  
    } else {
        //agarramos nueva pag fisica del area de memoria compartida
        phy = mmu_next_free_user_page();

        //ponemos 0 para asegurarnos que no haya basura
        //uint32_t *shared_memory = (uint32_t *)phy;
        //shared_memory[0] = 0x0;

        //la mapeamos a esa phy
        mmu_map_page(cr3, dir, phy, 0x07);

        //actualizamos los datos de dirs ya guardadas
        if (sched.current_team == 0){
          array_a[next_a][0] = dir >> 12;
          array_a[next_a][1] = phy;
          next_a++;
        } else {
          array_b[next_b][0] = dir >> 12;
          array_b[next_b][1] = phy;
          next_b++;
        }
    }
  } else {
    desalojar();
  }

}