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

extern T_CPU_REGISTERS CPU_Registers;
extern sem_t sem_pcb;
extern sem_t sem_cycle;
extern char* instruccion;
extern sem_t sem_instruccion;

t_PCB* pcb_recv(int socketMemoria);
void cpu_ciclo();


#endif //TP_2024_1C_GRUPO_CPU_CICLO_H
