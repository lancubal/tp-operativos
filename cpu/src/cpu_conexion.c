//
// Created by utnso on 4/14/24.
//

#include "cpu_conexion.h"

typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

void iniciarConexiones(cpu_config_t* cpuConfig, socketsT * sockets) {
    sockets->dispatchSocket = iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaDispatch, "CPU Dispatch");
    sockets->interruptSocket = iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaInterrupt, "CPU Interrupt");
    sockets->memoriaSocket = connectToServer(cpuConfig->ipMemoria,cpuConfig->puertoMemoria);
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

            case TEST:
            {
                char* cadena;
                uint8_t cant;

                if (!recv_test(cliente_socket, &cadena, &cant)) {
                    log_error(logger, "Fallo recibiendo TEST");
                    break;
                }

                log_info(logger, "Mirando %s con %" PRIu8 " cant.", cadena, cant);

                free(cadena);
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



int server_escuchar(char* server_name, const int* server_socket) {
    int cliente_socket = esperar_cliente(server_name, *server_socket);

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

    while (server_escuchar("CPU",(int*)server_socket));
    return -1;
}


void fin_conexion(socketsT * sockets){
    disconnectServer(sockets->dispatchSocket);
    disconnectServer(sockets->interruptSocket);
    disconnectClient(sockets->memoriaSocket);
    log_info(logger,"Terminado el Servidor CPU");
    log_destroy(logger);

}