//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_MODULE_TADS_H
#define TP_2024_1C_GRUPO_MODULE_TADS_H

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>

// Definición de la estructura para el PCB


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

typedef struct {
    int PID; // Identificador del proceso
    int Quantum; // Unidad de tiempo utilizada por el algoritmo de planificación VRR
    char* State; // Estado del proceso (NEW, READY, EXEC, BLOCK, EXIT)
    T_CPU_REGISTERS CPU_REGISTERS; // Registros de la CPU
    size_t size;
} t_PCB;

// Función para inicializar un t_PCB
t_PCB* init_PCB(int PID, int Quantum, char* State, T_CPU_REGISTERS* CPU_REGISTERS);

// Función para inicializar un T_CPU_REGISTERS
T_CPU_REGISTERS* init_CPU_REGISTERS(uint32_t, uint8_t, uint8_t, uint8_t, uint8_t,
                                   uint32_t, uint32_t, uint32_t, uint32_t,
                                   uint32_t, uint32_t);

// Funcion para cargar el contexto de ejecucion
void load_context(T_CPU_REGISTERS* old_registers, T_CPU_REGISTERS* new_registers);


#endif //TP_2024_1C_GRUPO_MODULE_TADS_H
