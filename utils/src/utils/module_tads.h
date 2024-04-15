//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_MODULE_TADS_H
#define TP_2024_1C_GRUPO_MODULE_TADS_H

#include <stdint.h>

typedef struct registroCPU {
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
} registroCPU;

struct PCB {
    int PID;
    int PC;
    int Quantum;
    registroCPU* registrosCPU;
};

#endif //TP_2024_1C_GRUPO_MODULE_TADS_H
