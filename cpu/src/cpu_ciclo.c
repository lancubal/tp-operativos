//
// Created by utnso on 4/14/24.
//

#include "cpu_ciclo.h"

void fetch() {
    log_warning(logger, "Fetching...");
    // Enviar el PC con el opcode FETCH
    send_packet(sockets->memoriaSocket, create_packet(FETCH, sizeof(CPU_Registers.PC), &CPU_Registers.PC, NULL));
    // Semaforo para esperar la instruccion de memoria
    sem_wait(&sem_instruccion);
    // semaforo
    CPU_Registers.PC++;
}

void decode() {
    // TODO: Decodificar la instruccion
}

void execute() {
    // TODO: Ejecutar la instruccion
}

void cpu_ciclo() {
    // TODO: Implementar el ciclo de la CPU
    while(true) {
        sem_wait(&sem_pcb);
        while (true) {
            fetch();
            decode();
            execute();
            if (1 == 0) {
                sem_post(&sem_cycle);
                break;
            }
        }
    }
}