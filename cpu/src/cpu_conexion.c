//
// Created by utnso on 4/14/24.
//

#include "cpu_conexion.h"

typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;


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