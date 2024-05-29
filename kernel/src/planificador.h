//
// Created by utnso on 5/20/24.
//

#ifndef TP_2024_1C_GRUPO_PLANIFICADOR_H
#define TP_2024_1C_GRUPO_PLANIFICADOR_H

#include <stddef.h>
#include <commons/collections//queue.h>
#include <utils/logger.h>
#include <utils/module_tads.h>
#include "sockets/protocol.h"
#include "sockets/client.h"
#include "semaphore.h"
#include <kernel_config.h>

typedef void (*ALGORITHM)(t_queue *, int);

extern sem_t sem_pcb;
extern t_PCB * pcb;
extern t_queue * ready_queue;

void SHORT_TERM_SCHEDULER();
//void LONG_TERM_SCHEDULER(t_queue * ready_queue, t_queue * new_queue);
void VRR_ALGORITHM(t_queue * ready_queue, int quantum);
void RR_ALGORITHM(t_queue * ready_queue, int quantum);
void FIFO_ALGORITHM(t_queue * ready_queue, int quantum);

#endif //TP_2024_1C_GRUPO_PLANIFICADOR_H
