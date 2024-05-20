//
// Created by utnso on 5/20/24.
//


#include "planificador.h"

enum ALGORITHM {
    FIFO,
    RR,
    VRR
};

void VRR_ALGORITHM(t_queue * ready_queue, int quantum) {
    // TODO
    t_PCB * pcb = queue_pop(ready_queue);
    while (pcb != NULL) {
        pcb->State = "EXEC";
        log_info(logger, "Ejecutando proceso %d\n", pcb->PID);
        pcb->Quantum = quantum;
        while (pcb->Quantum > 0) {
            pcb->Quantum--;
            if (pcb->Quantum == 0) {
                log_info(logger, "Proceso %d se quedo sin quantum\n", pcb->PID);
                break;
            }
        }
        free(pcb); // Liberar memoria del PCB (Proceso Control Block
        pcb = queue_pop(ready_queue);
    }
}

void RR_ALGORITHM(t_queue * ready_queue, int quantum) {
    t_PCB * pcb = queue_pop(ready_queue);
    while (pcb != NULL) {
        pcb->State = "EXEC";
        log_info(logger, "Ejecutando proceso %d\n", pcb->PID);
        pcb->Quantum = quantum;
        while (pcb->Quantum > 0) {
            pcb->Quantum--;
            if (pcb->Quantum == 0) {
                // Desalojar de CPU
                pcb->State = "READY";
                log_info(logger, "Proceso %d se quedo sin quantum\n", pcb->PID);
                break;
            }
        }
        free(pcb); // Liberar memoria del PCB (Proceso Control Block
        pcb = queue_pop(ready_queue);
    }
}

void FIFO_ALGORITHM(t_queue * ready_queue) {
    t_PCB * pcb = queue_pop(ready_queue);
    while (pcb != NULL) {
        pcb->State = "EXEC";
        log_info(logger, "Ejecutando proceso %d\n", pcb->PID);
        // Enviar PCB a CPU

        free(pcb); // Liberar memoria del PCB (Proceso Control Block
        pcb = queue_pop(ready_queue);
    }
}

void SHORT_TERM_SCHEDULER(t_queue * ready_queue, enum ALGORITHM algorithm, int quantum) {
    while (queue_size(ready_queue) > 0) {
        if (algorithm == FIFO) {
            FIFO_ALGORITHM(ready_queue);
            continue;
        }
        if (algorithm == RR) {
            RR_ALGORITHM(ready_queue, quantum);
            continue;
        }
        if (algorithm == VRR) {
            VRR_ALGORITHM(ready_queue, quantum);
            continue;
        }
    }
}



void LONG_TERM_SCHEDULER() {
    // Code
}