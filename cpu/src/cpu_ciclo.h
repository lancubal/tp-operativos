//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_CPU_CICLO_H
#define TP_2024_1C_GRUPO_CPU_CICLO_H

#include <stdint.h>
#include <pthread.h>
#include <utils/module_tads.h>
#include <sockets/protocol.h>
#include "utils/logger.h"
#include <semaphore.h>
#include "sockets/client.h"

typedef enum {
    SET, SUM, SUB, MOV, MOV_IN, MOV_OUT, RESIZE, JNZ, COPY_STRING,
    IO_GEN_SLEEP, IO_STDIN_READ, IO_STDOUT_WRITE, IO_FS_CREATE,
    IO_FS_DELETE, IO_FS_TRUNCATE, IO_FS_WRITE, IO_FS_READ,
    WAIT, SIGNAL, EXIT
} instruction_type;

typedef struct {
    instruction_type type;
    char operands[3][32]; // Tres operandos como máximo, tamaño máximo de cada operando: 32 caracteres
} instruction_decoded_t;

extern T_CPU_REGISTERS CPU_Registers;
extern sem_t sem_pcb;
extern sem_t sem_cycle;
extern char* instruccion;
extern sem_t sem_instruccion;
extern instruction_decoded_t* decoded_instruction;

t_PCB* pcb_recv(int socketMemoria);
void cpu_ciclo();


#endif //TP_2024_1C_GRUPO_CPU_CICLO_H
