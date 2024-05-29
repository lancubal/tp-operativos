//
// Created by utnso on 5/20/24.
//


#include "planificador.h"


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
        // TODO: Implementar el paquete
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

void FIFO_ALGORITHM(t_queue * ready_queue, int quantum) {
    t_PCB * pcb = malloc(sizeof(t_PCB));
    pcb = queue_pop(ready_queue);
    t_queue *block_queue = queue_create();
    while (true) {
        if (pcb == NULL) {
            if (queue_size(block_queue) == 0) {
                break;
            }
            pcb = queue_pop(block_queue); // TODO no se deberia desbloquear si no llega una interrupcion.
            if (pcb == NULL) {
                break;
            }
            queue_push(ready_queue, pcb);
        }
        // Enviar PCB a CPU
        OP_CODES cop = PCB;
        // TODO: Implementar el paquete
        send_data(sockets.dispatchSocket, &cop, sizeof(OP_CODES));
        send_tad(sockets.dispatchSocket, pcb, pcb->size);
        pcb->State = "EXEC";
        log_info(logger, "Ejecutando proceso %d\n", pcb->PID);

        // Recibir contexto actualizado del CPU
        if (recv_tad(sockets.dispatchSocket, (void **) &pcb)) {
            log_info(logger, "Proceso %d se ejecuto\n", pcb->PID);
        }

        // Tengo que decidir si lo mando a BLOCK o a EXIT
        if (strcmp(pcb->State, "BLOCK") == 0) {
            // Enviar PCB a CPU
            queue_push(block_queue, pcb);
            log_info(logger, "Proceso %d se bloqueo\n", pcb->PID);
        } else if (strcmp(pcb->State, "EXIT") == 0) {
            log_info(logger, "Proceso %d termino\n", pcb->PID);
        }
        if(queue_size(ready_queue) != 0) // La funcion queue_pop debería de implementar este checkeo
            pcb = queue_pop(ready_queue);
        else
            pcb = NULL;
    }
    free(pcb);
}

void SHORT_TERM_SCHEDULER(t_queue * ready_queue, const char* algo, int quantum) {
    ALGORITHM algorithm;
    if (strcmp(algo, "VRR") == 0) {
        algorithm = VRR_ALGORITHM;
    } else if (strcmp(algo, "RR") == 0) {
        algorithm = RR_ALGORITHM;
    } else if (strcmp(algo, "FIFO") == 0) {
        algorithm = FIFO_ALGORITHM;
    }
    while (queue_size(ready_queue) > 0) {
        algorithm(ready_queue, quantum);
    }
}

/*void LONG_TERM_SCHEDULER() {
    // Code
}*/