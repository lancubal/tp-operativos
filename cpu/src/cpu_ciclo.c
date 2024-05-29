//
// Created by utnso on 4/14/24.
//

#include "cpu_ciclo.h"

char* fetch() {
    log_warning(logger, "Esperando semaforo...");
    sem_wait(&sem_pcb);
    log_warning(logger, "Fetching...");
    // Enviar el opcode FETCH a memoria
    // TODO: Implementar el paquete
    //OP_CODES opcode = FETCH;
    //send_data(sockets.memoriaSocket, &opcode, sizeof(FETCH));
    // Envio PC
    //send_data(sockets.memoriaSocket, &CPU_Registers.PC, sizeof(CPU_Registers.PC));
    // Recibo la instruccion de memoria
    char* instruccion;
    instruccion = "instruccion";
    //recv_tad(sockets.memoriaSocket, (void*) &instruccion);
    CPU_Registers.PC++;
    sem_post(&sem_fetch);
    return instruccion;
}

void decode(char* instruccion) {
    // TODO: Decodificar la instruccion
}

void execute() {
    // TODO: Ejecutar la instruccion
}

void cpu_ciclo() {
    // TODO: Implementar el ciclo de la CPU
    char* instruccion = malloc(100);
    while (true) {
        instruccion = fetch();
        //decode(instruccion);
        //execute();
    }
}