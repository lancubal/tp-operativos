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
    // TODO CHECKPOINT 3
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
        free(pcb); // Liberar memoria del PCB (Proceso Control Block)
        pcb = queue_pop(ready_queue);
    }
}

void RR_ALGORITHM(t_queue * ready_queue, int quantum) {
    t_PCB * pcb = queue_pop(ready_queue);
    while (pcb != NULL) {
        send_tad(sockets.dispatchSocket, pcb, &pcb->size);
        pcb->State = "EXEC";
        log_info(logger, "Ejecutando proceso %d\n", pcb->PID);
        pcb->Quantum = quantum;
        while (pcb->Quantum > 0) {
            pcb->Quantum--;
            if (pcb->Quantum == 0) {
                // Desalojar de CPU
                // TODO
                send_tad(sockets.interruptSocket, pcb, &pcb->size);
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
    t_queue *block_queue = queue_create();
    while (true) {
        if (pcb == NULL) {
            pcb = queue_pop(block_queue);
            if (pcb == NULL) {
                break;
            }
        }
        // Enviar PCB a CPU
        send_tad(sockets.dispatchSocket, pcb, &pcb->size);
        pcb->State = "EXEC";
        log_info(logger, "Ejecutando proceso %d\n", pcb->PID);
        recv_tad(sockets.dispatchSocket, (void **) &pcb, &pcb->size);
        // Tengo que decidir si lo mando a BLOCK a EXIT
        if (pcb->State == "BLOCK") {
            // Enviar PCB a CPU
            queue_push(block_queue, pcb);
            log_info(logger, "Proceso %d se bloqueo\n", pcb->PID);
        } else if (pcb->State == "EXIT") {
            log_info(logger, "Proceso %d termino\n", pcb->PID);
        }
        free(pcb); // Liberar memoria del PCB (Proceso Control Block)
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