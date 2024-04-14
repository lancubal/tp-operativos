//
// Created by utnso on 4/14/24.
//

#include "cpu_conexion.h"

void iniciarConexiones(cpu_config_t* cpuConfig) {
    //Se crea el tad para el hilo de Dispatch
    pthread_t dispatchServer;
    //Se crea el servidor de Dispatch
    int socketDispatchServer = iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaDispatch, "Dispatch");
    //Se crea el hilo de Dispatch
    pthread_create(&dispatchServer, NULL, (void*) waitClient, (void*) &socketDispatchServer);

    //Se crea el tad para el hilo de Interrupt
    pthread_t interruptServer;
    //Se crea el servidor de Interrupt
    int socketInterruptServer = iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaInterrupt, "Interrupt");
    //Se crea el hilo de Interrupt
    pthread_create(&interruptServer, NULL, (void*) waitClient, (void*) &socketInterruptServer);

    //Se crea el tad para el hilo de conexion a Memoria
    pthread_t conectarAMemoria;
    //Se asignan los argumentos para el hilo de Memoria
    ConectarArgsT* memoriaServerArgs = createConectarArgs(cpuConfig->ipMemoria, cpuConfig->puertoMemoria, "Memoria");
    //Se crea el hilo de Memoria
    pthread_create(&conectarAMemoria, NULL, (void*) conectarA, (void*) memoriaServerArgs);

    //Se inician los hilos de las conexiones
    int socketInterruptCliente;
    pthread_join(interruptServer, (void*) &socketInterruptCliente);
    int socketDispatchCliente;
    pthread_join(dispatchServer, (void*) &socketDispatchCliente);
    pthread_join(conectarAMemoria, NULL);
}
