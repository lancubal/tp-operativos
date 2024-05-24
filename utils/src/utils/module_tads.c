//
// Created by utnso on 4/14/24.
//

#include "module_tads.h"

// Función para inicializar un t_PCB
t_PCB init_PCB(int PID, int PC, int Quantum) {
    t_PCB pcb;
    pcb.PID = PID;
    pcb.PC = PC;
    pcb.Quantum = Quantum;
    pcb.CPU_REGISTERS = init_CPU_REGISTERS(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    pcb.State = "NEW";
    pcb.size = sizeof(t_PCB) + pcb.CPU_REGISTERS.size;
    return pcb;
}

// Función para inicializar un T_CPU_REGISTERS
T_CPU_REGISTERS init_CPU_REGISTERS(uint8_t AX, uint8_t BX, uint8_t CX, uint8_t DX,

                                   uint32_t EAX, uint32_t EBX, uint32_t EDX, uint32_t ECX,
                                   uint32_t SI, uint32_t DI) {
    T_CPU_REGISTERS registers;
    registers.AX = 0;
    registers.BX = 0;
    registers.CX = 0;
    registers.DX = 0;
    registers.EAX = 0;
    registers.EBX = 0;
    registers.EDX = 0;
    registers.ECX = 0;
    registers.SI = 0;
    registers.DI = 0;
    registers.size = sizeof(T_CPU_REGISTERS);
    return registers;
}