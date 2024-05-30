//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_MODULE_TADS_H
#define TP_2024_1C_GRUPO_MODULE_TADS_H

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>

// Estructura para una entrada de la tabla de páginas
typedef struct {
    uint32_t frame_number; // Número de marco de la página
    int valid; // Bit de validez
} t_page_table_entry;

// Estructura para una tabla de páginas
typedef struct {
    t_page_table_entry* entries;
    size_t num_entries;
} t_page_table;

typedef struct {
    uint32_t PC;
    uint8_t AX;
    uint8_t BX;
    uint8_t CX;
    uint8_t DX;
    uint32_t EAX;
    uint32_t EBX;
    uint32_t EDX;
    uint32_t ECX;
    uint32_t SI;
    uint32_t DI;
    size_t size;
} T_CPU_REGISTERS;

// Definición de la estructura para el PCB
typedef struct {
    int PID; // Identificador del proceso
    int Quantum; // Unidad de tiempo utilizada por el algoritmo de planificación VRR
    char* State; // Estado del proceso (NEW, READY, EXEC, BLOCK, EXIT)
    T_CPU_REGISTERS CPU_REGISTERS; // Registros de la CPU
    uint32_t memory_size;
    t_page_table page_table;
    size_t size;
} t_PCB;

/* RECORDATORIO: El kernel inicializa un PCB.
 * Luego la memoria se fija si tiene espacio para asignarle y si es asi le asigna la pagina correspondiente.
 * En la pagina guardo las instrucciones*/

// Función para inicializar un t_PCB
t_PCB* init_PCB(int PID, int Quantum, char* State, T_CPU_REGISTERS* CPU_REGISTERS, uint32_t memory_size);

// Función para inicializar un T_CPU_REGISTERS
T_CPU_REGISTERS* init_CPU_REGISTERS(uint32_t, uint8_t, uint8_t, uint8_t, uint8_t,
                                   uint32_t, uint32_t, uint32_t, uint32_t,
                                   uint32_t, uint32_t);

// Funcion para cargar el contexto de ejecucion
void load_context(T_CPU_REGISTERS* old_registers, T_CPU_REGISTERS* new_registers);


#endif //TP_2024_1C_GRUPO_MODULE_TADS_H
