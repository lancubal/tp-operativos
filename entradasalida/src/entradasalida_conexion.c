//
// Created by utnso on 4/14/24.
//

#include "entradasalida_conexion.h"
#include "pthread.h"

void iniciarConexiones(entradasalida_config_t* entradasalidaConfig) {
    //Se crea el tad para el hilo de conexion a kernel
    pthread_t conectarAKernel;
    //Se asignan los argumentos para el hilo de kernel
    ConectarArgsT* kernelServerArgs = createConectarArgs(entradasalidaConfig->ipKernel, entradasalidaConfig->puertoKernel, "Kernel");
    //Se crea el hilo de kernel
    pthread_create(&conectarAKernel, NULL, (void*) conectarA, (void*) kernelServerArgs);

    //Se crea el tad para el hilo de conexion a Memoria
    pthread_t conectarAMemoria;
    //Se asignan los argumentos para el hilo de Memoria
    ConectarArgsT* memoriaServerArgs = createConectarArgs(entradasalidaConfig->ipMemoria, entradasalidaConfig->puertoMemoria, "Memoria");
    //Se crea el hilo de Memoria
    pthread_create(&conectarAMemoria, NULL, (void*) conectarA, (void*) memoriaServerArgs);

    //Se inician los hilos de las conexiones
    pthread_join(conectarAKernel, NULL);
    pthread_join(conectarAMemoria, NULL);
}