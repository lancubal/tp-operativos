//
// Created by utnso on 4/14/24.
//

#include "kernel_conexion.h"


typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

void iniciarConexiones(kernel_config_t* kernelConfig) {
    //Iniciar servidor de Kernel
    //Se crea el tad para el hilo de Memoria server
    pthread_t kernelServer;
    //Se inicia el servidor de Kernel
    int socketKernel = iniciarServerProceso(kernelConfig->ipKernel, kernelConfig->puertoEscucha, "Kernel");
    //Se crea el hilo de Kernel server
    pthread_create(&kernelServer, NULL, (void*) waitClient, (void*) &socketKernel);

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



void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

    op_code_NUESTRO cop;
    while (cliente_socket != -1) {

        if (recv(cliente_socket, &cop, sizeof(op_code_NUESTRO), 0) != sizeof(op_code_NUESTRO)) {
            log_info(logger, "DISCONNECT!");
            return;
        }

        switch (cop) {
            case DEBUG_CODE:
                log_info(logger, "debug");
                break;

            case MIRAR_NETFLIX:
            {
                char* peli;
                uint8_t pochoclos;

                if (!recv_mirar_netflix(cliente_socket, &peli, &pochoclos)) {
                    log_error(logger, "Fallo recibiendo MIRAR_NETFLIX");
                    break;
                }

                log_info(logger, "Mirando %s con %" PRIu8 " pochoclos.", peli, pochoclos);

                free(peli);
                break;
            }

            // Errores
            case -1:
                log_error(logger, "Cliente desconectado de %s...", server_name);
                return;
            default:
                log_error(logger, "Algo anduvo mal en el server de %s", server_name);
                log_info(logger, "Cop: %d", cop);
                return;
        }
    }

    log_warning(logger, "El cliente se desconecto de %s server", server_name);
    return;
}



int server_escuchar(t_log* logger, char* server_name, int* server_socket) {
    int cliente_socket = esperar_cliente(logger, server_name, *server_socket);
    

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}

int phread_server_escuchar(void* server_socket){

    while (server_escuchar(logger,"NAME",(int*)server_socket));
    return -1;
}