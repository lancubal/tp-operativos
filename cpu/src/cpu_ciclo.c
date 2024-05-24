//
// Created by utnso on 4/14/24.
//

#include "cpu_ciclo.h"

t_PCB* pcb_recv(int socketDispatch) { // recibir un pcb
    t_PCB* pcb = malloc(sizeof (t_PCB));
    if(recv_tad(socketDispatch, (void**) pcb)) {
        log_info(logger, "Recibido PCB con PID: %d\n", pcb->PID);
        return pcb;
    }
    return NULL;
}

/*char* fetch(uint32_t PC, int socketMemoria) { // TODO: Implementar
    sendMessage("next ins", socketMemoria);

    //Hilo para getMessasge y retornar el mensaje
    pthread_t getMessageTH;
    pthread_create(&getMessageTH, NULL, (void*) getMessage, (void*) &socketMemoria);
    char* instruccion = malloc(sizeof (char) * 100);
    pthread_join(getMessageTH, (void*) instruccion);


    return instruccion;
}*/