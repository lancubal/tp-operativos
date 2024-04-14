//
// Created by utnso on 4/14/24.
//

#include "kernel_conexion.h"

void iniciarConexiones(kernel_config_t* kernelConfig) {
    //Iniciar servidor de Kernel
    //Se crea el tad para el hilo de Memoria server
    pthread_t kernelServer;
    //Se inicia el servidor de Kernel
    int socketKernel = iniciarServerProceso(kernelConfig->ipKernel, kernelConfig->puertoEscucha, "Kernel");
    //Se crea el hilo de Kernel server
    pthread_create(&kernelServer, NULL, (void*) iniciarServerProceso, (void*) &socketKernel);

    //Se crea el tad para el hilo de conexion a Memoria
    pthread_t conectarAMemoria;
    //Se asignan los argumentos para el hilo de Memoria
    ConectarArgsT* memoriaServerArgs = createConectarArgs(kernelConfig->ipMemoria, kernelConfig->puertoMemoria, "Memoria");
    //Se crea el hilo de Memoria
    pthread_create(&conectarAMemoria, NULL, (void*) conectarA, (void*) memoriaServerArgs);

    //Se crea el tad para el hilo de conexion a CPU Dispatch
    pthread_t conectarACPUDispatch;
    //Se asignan los argumentos para el hilo de CPU Dispatch
    ConectarArgsT* cpuDispatchServerArgs = createConectarArgs(kernelConfig->ipCPU, kernelConfig->puertoCPUDispatch, "CPU Dispatch");
    //Se crea el hilo de CPU Dispatch
    pthread_create(&conectarACPUDispatch, NULL, (void*) conectarA, (void*) cpuDispatchServerArgs);

    //Se crea el tad para el hilo de conexion a CPU Interrupt
    pthread_t conectarACPUInterrupt;
    //Se asignan los argumentos para el hilo de CPU Interrupt
    ConectarArgsT* cpuInterruptServerArgs = createConectarArgs(kernelConfig->ipCPU, kernelConfig->puertoCPUInterrupt, "CPU Interrupt");
    //Se crea el hilo de CPU Interrupt
    pthread_create(&conectarACPUInterrupt, NULL, (void*) conectarA, (void*) cpuInterruptServerArgs);

    //Se inician los hilos de las conexiones
    int socketKernelCliente;
    pthread_join(kernelServer, (void*) &socketKernelCliente);
    int socketMemoriaServer;
    pthread_join(conectarAMemoria, (void*) &socketMemoriaServer);
    int socketCPUDispatchServer;
    pthread_join(conectarACPUDispatch, (void*) &socketCPUDispatchServer);
    int socketCPUInterruptServer;
    pthread_join(conectarACPUInterrupt, (void*) &socketCPUInterruptServer);
}