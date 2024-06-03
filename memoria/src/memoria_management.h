//
// Created by utnso on 5/30/24.
//

#ifndef TP_2024_V2_MEMORIA_MANAGEMENT_H
#define TP_2024_V2_MEMORIA_MANAGEMENT_H

#include "stdint.h"
#include "stdlib.h"
#include "memoria_config.h"
#include "utils/module_tads.h"


// Memoria contigua
extern void* memory;
// Tabla de páginas del proceso actual (simulación simple con una única tabla)
extern t_page_table page_table;

// Funciones para inicializar y liberar la memoria
void init_memory();
void free_memory();

// Funciones para acceder a la memoria
uint32_t translate_address(uint32_t logical_address);
void write_memory_from_physical(uint32_t physical_address, uint8_t value);
void write_memory_from_logical(uint32_t logical_address, uint8_t value);
uint8_t read_memory_from_logical(uint32_t logical_address);
void allocate_page(uint32_t page_number, uint32_t frame_number);
// Funcion para asignarle una pagina a un proceso
void assign_page_to_process(t_PCB* pcb);



#endif //TP_2024_V2_MEMORIA_MANAGEMENT_H
