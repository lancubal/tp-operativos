//
// Created by utnso on 4/13/24.
//

#include "memoria_conexion.h"

void iniciarConexiones(memoria_config_t* memoriaConfig) {


    //Iniciar servidor de Memoria server
    int socketMemoria = iniciarServerProceso(memoriaConfig->ipMemoria, memoriaConfig->puertoEscucha, "Memoria");

    //Se crean los tad para recibir al CPU
    pthread_t cpuClientTH;
    //Se crea el hilo para recibir al CPU
    pthread_create(&cpuClientTH, NULL, (void*) waitClient, (void*) socketMemoria);

    //Se crea el tad para recibir al Kernel
    pthread_t kernelClientTH;
    //Se crea el hilo para recibir al Kernel
    pthread_create(&kernelClientTH, NULL, (void*) waitClient, (void*) socketMemoria);

    //Se inicia el hilo para recibir al CPU
    int socketCPU;
    int socketKernel;
    log_info(logger, "Esperando al cliente: CPU");
    log_info(logger, "Esperando al cliente: Kernel");
    pthread_join(cpuClientTH, (void*) &socketCPU);
    pthread_join(kernelClientTH, (void*) &socketKernel);
}