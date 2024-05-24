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

t_PCB* pcb_recv(int socketMemoria);
char* fetch(uint32_t PC, int socketMemoria);


#endif //TP_2024_1C_GRUPO_CPU_CICLO_H
