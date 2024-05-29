//
// Created by utnso on 4/14/24.
//


#include "module_tads.h"

// Función para inicializar un t_PCB
t_PCB* init_PCB(int PID, int Quantum, char* State, T_CPU_REGISTERS* CPU_REGISTERS) {
    t_PCB* pcb = malloc(sizeof(t_PCB));
    pcb->PID = PID;
    pcb->Quantum = Quantum;
    pcb->State = State;
    load_context(&pcb->CPU_REGISTERS, CPU_REGISTERS);
    pcb->size = sizeof(t_PCB) + CPU_REGISTERS->size;
    return pcb;
}

// Función para inicializar un T_CPU_REGISTERS
T_CPU_REGISTERS* init_CPU_REGISTERS(uint32_t PC, uint8_t AX, uint8_t BX, uint8_t CX, uint8_t DX,

                                   uint32_t EAX, uint32_t EBX, uint32_t EDX, uint32_t ECX,
                                   uint32_t SI, uint32_t DI) {
    T_CPU_REGISTERS* registers = malloc(sizeof(T_CPU_REGISTERS));
    registers->PC = PC;
    registers->AX = AX;
    registers->BX = BX;
    registers->CX = CX;
    registers->DX = DX;
    registers->EAX = EAX;
    registers->EBX = EBX;
    registers->EDX = EDX;
    registers->ECX = ECX;
    registers->SI = SI;
    registers->DI = DI;

    registers->size = sizeof(T_CPU_REGISTERS);
    return registers;
}

// Funcion para cargar el contexto de ejecucion
void load_context(T_CPU_REGISTERS* old_registers, T_CPU_REGISTERS* new_registers) {
    old_registers->AX = new_registers->AX;
    old_registers->BX = new_registers->BX;
    old_registers->CX = new_registers->CX;
    old_registers->DX = new_registers->DX;
    old_registers->EAX = new_registers->EAX;
    old_registers->EBX = new_registers->EBX;
    old_registers->EDX = new_registers->EDX;
    old_registers->ECX = new_registers->ECX;
    old_registers->SI = new_registers->SI;
    old_registers->DI = new_registers->DI;
    old_registers->size = new_registers->size;
}
