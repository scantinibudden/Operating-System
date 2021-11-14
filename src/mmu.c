/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

unsigned int proxima_pagina_libre = 0x100000 - PAGE_SIZE;
unsigned int proxima_pagina_libre_user = 0x400000 - PAGE_SIZE;

void mmu_init(void) {
  proxima_pagina_libre = INICIO_DE_PAGINAS;
}

paddr_t mmu_next_free_kernel_page(void) {
  unsigned int pagina_libre = proxima_pagina_libre;
  proxima_pagina_libre += PAGE_SIZE;
  return pagina_libre;
}

paddr_t mmu_next_free_user_page(void) {
  unsigned int pagina_libre = proxima_pagina_libre_user;
  proxima_pagina_libre_user += PAGE_SIZE;
  return pagina_libre;
}

paddr_t mmu_init_kernel_dir(void){
  page_directory_entry *pd = (page_directory_entry *)KERNEL_PAGE_DIR;
  page_table_entry *pt = (page_table_entry *)KERNEL_PAGE_TABLE_0;

  for (int i = 0; i < 1024; ++i)
  {
    pd[i] = (page_directory_entry){0};
    pt[i] = (page_table_entry){0};
  }

  pd[0].present = MMU_FLAG_PRESENT;
  pd[0].user_supervisor = MMU_FLAG_SUPERVISOR;
  pd[0].read_write = MMU_FLAG_READWRITE;
  pd[0].page_table_base = ((uint32_t)pt >> 12);

  for (int i = 0; i < 1024; ++i)
  {
    pt[i].present = MMU_FLAG_PRESENT;
    pt[i].user_supervisor = MMU_FLAG_SUPERVISOR;
    pt[i].read_write = MMU_FLAG_READWRITE;
    pt[i].physical_adress_base = i;
  }

  return (uint32_t)pd;
}

void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs){
  //virtual desde donde, fisica a donde queremos q virtual apunte
  //dividir la direccion a mapear

  uint32_t directoryIndex = virt >> 22;
  uint32_t tableIndex = (virt >> 12) & 0x3FF;

  //usando cr3 -> puntero a PD, calculamoos la direccion de la pde
  page_directory_entry *PDE = (page_directory_entry *)cr3;

  //si present es 0 entonces pedimos nueva pagina para la page table
  if (PDE[directoryIndex].present != 1){

    page_table_entry *newPT = (page_table_entry *)mmu_next_free_kernel_page();

    for (int i = 0; i < 1024; i++)
      newPT[i] = (page_table_entry){0};

    PDE[directoryIndex].page_table_base = ((uint32_t)newPT >> 12);
  }

  page_table_entry *PT = (page_table_entry *)(PDE[directoryIndex].page_table_base << 12 & ~0xFFF); //~ es negaar
  PT[tableIndex].physical_adress_base = (phy >> 12);

  PDE[directoryIndex].present |= 1;
  PDE[directoryIndex].read_write |= (attrs >> 1) % 2;
  PDE[directoryIndex].user_supervisor |= (attrs>>2);

  PT[tableIndex].present |= 1;
  PT[tableIndex].read_write |= (attrs>>1) % 2;
  PT[tableIndex].user_supervisor |= (attrs>>2);

  //llamamos a la funcion tlbflush(), se encarga de poner en 0 toda la tlb, la cache de traduccion de paginas
  tlbflush();
}

paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {

  uint32_t directoryIndex = virt >> 22;
  uint32_t tableIndex = (virt >> 12) & 0x3FF;

  //usando cr3 -> puntero a PD, calculamoos la direccion de la pde
  page_directory_entry *PDE = (page_directory_entry *)cr3;
  paddr_t phy = 0;

  //si present es 0 entonces pedimos nueva pagina para la page table
  if (PDE[directoryIndex].present == 1){

    page_table_entry *PT = (page_table_entry *)(PDE[directoryIndex].page_table_base << 12);

    phy = PT[tableIndex].physical_adress_base;

    for (int i = 0; i < 1024; i++)
      PT[i] = (page_table_entry){0};

    PDE[directoryIndex].present = 0;
  }

  return phy;
}

paddr_t mmu_init_task_dir(paddr_t phy_start) {

  int32_t cr3 = mmu_next_free_kernel_page();

  for (int32_t i = 0x000000; i < 0x400000; i += PAGE_SIZE){
    mmu_map_page(cr3, i, i, 0x3);
  }

  mmu_map_page(cr3, 0x08000000, phy_start, 0x5);
  mmu_map_page(cr3, 0x08001000, phy_start + PAGE_SIZE, 0x5);

  mmu_map_page(cr3, 0x08002000, mmu_next_free_user_page(), 0x7);

  return cr3;
}
