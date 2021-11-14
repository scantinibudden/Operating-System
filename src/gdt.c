/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry_t gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x0000,
            .limit_19_16 = 0x0,
            .type = 0x0,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x00,
            .db = 0x0,
            .g = 0x00,
            .l = 0x0,
            .avl = 0x0},

// | base_15_0 | limit_15_0 | base_31_24 | g/db/l/avl | limit_19_16 | p/dpl/s | type | base_23_16 |
// | 0 - 3     | 4 - 7      | 8 - 9      | 10         | 11          | 12      | 13   | 14 - 15    |

        [GDT_IDX_CS_0] = {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x30FF, //? 0x330FF
            .limit_19_16 = 0x3,
            .type = 0xA, //  Read Execute
            .s = 0x01,   // 0-Sistema 1-Data  Codigo
            .dpl = 0x00, // Nivel 0
            .p = 0x01,   //Siempre 1
            .avl = 0x0,  // No nos interesa
            .l = 0x0,    // Compatibilidad 32 bits
            .db = 0x1,   // 1 indica codigo o data 32 bits
            .g = 0x01},   // siempre 1

        [GDT_IDX_DS_0] = {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x30FF, //? 0x330FF
            .limit_19_16 = 0x3,
            .type = 0x02, //  Read write
            .s = 0x01,    // 0-Sistema 1-Data  Codigo
            .dpl = 0x0,  // Nivel 0
            .p = 0x01,    //Siempre 1
            .avl = 0x0,   // No nos interesa
            .l = 0x0,     // Compatibilidad 32 bits
            .db = 0x1,    // 1 indica codigo o data 32 bits
            .g = 0x01},    // siempre 1
    
        [GDT_IDX_CS_3] = {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x30FF, //? 0x330FF
            .limit_19_16 = 0x3,
            .type = 0xA, //  Read Execute
            .s = 0x01,   // 0-Sistema 1-Data  Codigo
            .dpl = 0x03, // Nivel 0
            .p = 0x01,   //Siempre 1
            .avl = 0x0,  // No nos interesa
            .l = 0x0,    // Compatibilidad 32 bits
            .db = 0x1,   // 1 indica codigo o data 32 bits
            .g = 0x01},   // siempre 1

         [GDT_IDX_DS_3] = {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x30FF, //? 0x330FF
            .limit_19_16 = 0x3,
            .type = 0x02, //  Read write
            .s = 0x01,    // 0-Sistema 1-Data  Codigo
            .dpl = 0x03,  // Nivel 0
            .p = 0x01,    //Siempre 1
            .avl = 0x0,   // No nos interesa
            .l = 0x0,     // Compatibilidad 32 bits
            .db = 0x1,    // 1 indica codigo o data 32 bits
            .g = 0x01},    // siempre 1

        [GDT_IDX_VS_0] = {
            .base_15_0 = 0x8000,
            .base_23_16 = 0x0B,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x1F40,       //? 0x1000
            .limit_19_16 = 0x0,
            .type = 0x2,                //  Read Write
            .s = 0x1,                   // 0-Sistema 1-Data  Codigo
            .dpl = 0x0,                 // Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x1,                  // 1 indica codigo o data 32 bits
            .g = 0x00},                 // 

        [TSS_IDLE] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0},                  // siempre 1

        [TSS_INIT] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0},                 

        [LEMMING_0] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 3
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,                  
        },

        [LEMMING_1] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 3
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,                 
        },

        [LEMMING_2] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,                  
        },
        
        [LEMMING_3] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,                  
        },
        
        [LEMMING_4] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,                
        },
        
        [LEMMING_5] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,                
        },

        [LEMMING_6] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,                  
        },

        [LEMMING_7] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,                 
        },

        [LEMMING_8] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,              
        },
        
        [LEMMING_9] = {
            .base_15_0 = 0x0000,        //nose
            .base_23_16 = 0x00,         //nose
            .base_31_24 = 0x00,         //nose
            .limit_15_0 = 0x67,         //0x67 = size of tss
            .limit_19_16 = 0x0,
            .type = 0x9,                //type tss, 1001
            .s = 0x0,                   //0 - Sistema
            .dpl = 0x0,                 //Nivel 0
            .p = 0x01,                  //Siempre 1
            .avl = 0x0,                 // No nos interesa
            .l = 0x0,                   // Compatibilidad 32 bits
            .db = 0x0,                  // 1 indica codigo o data 32 bits
            .g = 0x0,                 
        }
};

gdt_descriptor_t GDT_DESC = {
  sizeof(gdt) - 1, 
  (uint32_t) &gdt
  };