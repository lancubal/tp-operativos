//
// Created by utnso on 4/14/24.
//

#include "cpu_ciclo.h"

char* fetch(uint32_t PC, int socketMemoria) { // TODO: Implementar
    sendMessage("next ins", socketMemoria);

    //Hilo para getMessasge y retornar el mensaje
    pthread_t getMessageTH;
    pthread_create(&getMessageTH, NULL, (void*) getMessage, (void*) &socketMemoria);
    char* instruccion = malloc(sizeof (char) * 100);
    pthread_join(getMessageTH, (void*) instruccion);


    return instruccion;
}