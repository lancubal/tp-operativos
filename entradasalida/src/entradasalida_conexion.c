//
// Created by utnso on 4/14/24.
//

#include "entradasalida_conexion.h"

/**
 * @brief Inicia las conexiones con los servidores de Memoria y Kernel.
 *
 * Esta función se encarga de iniciar las conexiones con los servidores de Memoria y Kernel.
 * Utiliza la configuración de Entrada/Salida para obtener las direcciones IP y los puertos necesarios para estas conexiones.
 *
 * @param entradasalidaConfig La configuración de Entrada/Salida.
 */
void iniciarConexiones(){
    sockets = malloc(sizeof(socketsT));
    // Nos conectamos al servidor de Memoria utilizando la IP y el puerto especificados en la configuración de Entrada/Salida.
    sockets->memoriaSocket = connectToServer(entradasalida_config->ipMemoria,entradasalida_config->puertoMemoria);
    // Nos conectamos al servidor de Kernel utilizando la IP y el puerto especificados en la configuración de Entrada/Salida.
    sockets->kernelSocket = connectToServer(entradasalida_config->ipKernel,entradasalida_config->puertoKernel);
}

/**
 * @brief Finaliza todas las conexiones y libera los recursos utilizados.
 *
 * Esta función se encarga de desconectar todos los sockets de cliente,
 * y liberar los recursos utilizados por el logger.
 *
 */
void fin_conexion(){
    // Desconectamos el socket de cliente de Memoria
    disconnectClient(sockets->memoriaSocket);
    // Desconectamos el socket de cliente de Kernel
    disconnectClient(sockets->kernelSocket);
    // Registramos un mensaje indicando que el servidor de Entrada/Salida ha terminado
    log_info(logger,"Terminado I/O");
    // Liberamos los recursos utilizados por el logger
    log_destroy(logger);
}

void procesar_conexion(void* void_args) {
    // Convertimos los argumentos a su tipo correcto
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

    // Entramos en un bucle donde recibimos y procesamos los mensajes de los clientes
    while (cliente_socket != -1) {
        // Recibimos el código de operación del mensaje
        t_packet* packet = malloc(sizeof(t_packet));
        if(recv_packet(cliente_socket, packet)) {
            log_info(logger, "Recibido mensaje con código de operación: %d\n", packet->op_code);
        }
        switch (packet->op_code){
            case IO_GEN_SLEEP: {
                if (interface->type != GENERIC) {
                    log_warning(logger, "No se puede realizar la operación IO_GEN_SLEEP en la interfaz %s", interface->name);
                    return;
                }
                int sleep_time;
                memcpy(&sleep_time, packet->payload, packet->payload_size);
                sleep(sleep_time);

                // Le avisamos al kernel que termino
                send_packet(cliente_socket, create_packet(IO_END, sizeof(IO_END), NULL, NULL));
                break;
            }
            case IO_STDIN_READ: {
                if (interface->type != STDIN) {
                    log_warning(logger, "No se puede realizar la operación IO_STDIN_READ en la interfaz %s", interface->name);
                    return;
                }
                uint32_t physical_address;
                memcpy(&physical_address, packet->payload, packet->payload_size);
                char* text_from_console = readline(">");
                send_packet(sockets->memoriaSocket, create_packet(WRITE_FROM_PHYSICAL, sizeof(WRITE_FROM_PHYSICAL), text_from_console, NULL));
                recv_packet(sockets->memoriaSocket, packet);
                if(packet->op_code != WRITE_FROM_PHYSICAL) {
                    log_error(logger, "Error al escribir en memoria");
                    return;
                }

                // Le avisamos al kernel que termino
                send_packet(cliente_socket, create_packet(IO_END, sizeof(IO_END), NULL, NULL));
                break;
            }
            case IO_STDOUT_WRITE: {
                if (interface->type != STDOUT) {
                    log_warning(logger, "No se puede realizar la operación IO_STDOUT_WRITE en la interfaz %s", interface->name);
                    return;
                }
                uint32_t physical_address;
                memcpy(&physical_address, packet->payload, packet->payload_size);
                send_packet(sockets->memoriaSocket, create_packet(READ_FROM_PHYSICAL, sizeof(READ_FROM_PHYSICAL), &physical_address, NULL));
                recv_packet(sockets->memoriaSocket, packet);
                if(packet->op_code != READ_FROM_PHYSICAL) {
                    log_error(logger, "Error al leer de memoria");
                    return;
                }
                char* text_from_memory;
                memcpy(text_from_memory, packet->payload, packet->payload_size);
                // Le avisamos al kernel que termino
                send_packet(cliente_socket, create_packet(IO_END, sizeof(IO_END), NULL, NULL));
                break;
            }
            case IO_FS_CREATE: {
                if (interface->type != DIALFS) {
                    log_warning(logger, "No se puede realizar la operación IO_FS_CREATE en la interfaz %s", interface->name);
                    return;
                }

                // Le avisamos al kernel que termino
                send_packet(cliente_socket, create_packet(IO_END, sizeof(IO_END), NULL, NULL));
                break;
            }
            case IO_FS_DELETE: {
                if (interface->type != DIALFS) {
                    log_warning(logger, "No se puede realizar la operación IO_FS_DELETE en la interfaz %s", interface->name);
                    return;
                }

                // Le avisamos al kernel que termino
                send_packet(cliente_socket, create_packet(IO_END, sizeof(IO_END), NULL, NULL));
                break;
            }
            case IO_FS_TRUNCATE: {
                if (interface->type != DIALFS) {
                    log_warning(logger, "No se puede realizar la operación IO_FS_TRUNCATE en la interfaz %s", interface->name);
                    return;
                }

                // Le avisamos al kernel que termino
                send_packet(cliente_socket, create_packet(IO_END, sizeof(IO_END), NULL, NULL));
                break;
            }
            case IO_FS_WRITE: {
                if (interface->type != DIALFS) {
                    log_warning(logger, "No se puede realizar la operación IO_FS_WRITE en la interfaz %s", interface->name);
                    return;
                }

                // Le avisamos al kernel que termino
                send_packet(cliente_socket, create_packet(IO_END, sizeof(IO_END), NULL, NULL));
                break;
            }
            case IO_FS_READ: {
                if (interface->type != DIALFS) {
                    log_warning(logger, "No se puede realizar la operación IO_FS_READ en la interfaz %s", interface->name);
                    return;
                }

                // Le avisamos al kernel que termino
                send_packet(cliente_socket, create_packet(IO_END, sizeof(IO_END), NULL, NULL));
                break;
            }
        }
    }
}

// Escuchar al servidor conectado
void cliente_escuchar(int* client_socket) {

    // debo usar procesar_conexion y correrlo.
    pthread_t hilo;
    t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
    args->fd = *client_socket;
    args->server_name = "test";
    pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
    pthread_detach(hilo);
}