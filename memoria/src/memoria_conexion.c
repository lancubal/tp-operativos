//
// Created by utnso on 4/13/24.
//

#include "memoria_conexion.h"

/**
 * @brief Estructura para almacenar los argumentos de la función procesar_conexion.
 *
 * Esta estructura se utiliza para pasar los argumentos a la función procesar_conexion cuando se crea un nuevo hilo.
 */
typedef struct {
    int fd;             // El descriptor de archivo del socket del cliente.
    char* server_name;  // El nombre del servidor.
} t_procesar_conexion_args;

/**
 * @brief Inicia las conexiones con el servidor de Memoria.
 *
 * Esta función se encarga de iniciar la conexión con el servidor de Memoria.
 * Utiliza la configuración de la Memoria para obtener la dirección IP y el puerto necesarios para esta conexión.
 *
 * @param memoriaConfig La configuración de la Memoria.
 */
void iniciarConexiones() {
    // Iniciamos el servidor de Memoria utilizando la IP y el puerto especificados en la configuración de la Memoria.
    sockets = malloc(sizeof(socketsT));
    sockets->memoriaSocket = iniciarServerProceso(memoria_config->ipMemoria, memoria_config->puertoEscucha, "Memoria");
}

/**
 * @brief Procesa las conexiones entrantes de los clientes.
 *
 * Esta función se encarga de procesar las conexiones entrantes de los clientes.
 * Recibe los mensajes de los clientes y los procesa de acuerdo a su código de operación.
 *
 * @param void_args Los argumentos de la función, que deben ser de tipo t_procesar_conexion_args.
 */
void procesar_conexion(void* void_args) {
    // Convertimos los argumentos a su tipo correcto
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

    // Entramos en un bucle donde recibimos y procesamos los mensajes de los clientes
    while (cliente_socket != -1) {
        // Recibir un paquete
        t_packet* packet = malloc(sizeof(t_packet));
        if(recv_packet(cliente_socket, packet)) {
            log_info(logger, "Recibido mensaje con código de operación: %d\n", packet->op_code);
        }
        // Procesamos el mensaje de acuerdo a su código de operación
        switch (packet->op_code) {
            case FETCH: {
                // Recibir un PC
                uint32_t pc;
                memcpy(&pc, packet->payload, packet->payload_size);
                log_info(logger, "Recibido PC: %d", pc);
                char* instruccion = instrucciones[pc];
                send_packet(cliente_socket, create_packet(FETCH, strlen(instruccion), instruccion, NULL));
                break;
            }
            case NEW_PROCESS: {
                // Recibir un PCB y le asigno una pagina si hay espacio.
                t_PCB* pcb = malloc(sizeof(t_PCB));
                deserialize_pcb(packet->payload, packet->payload_size, pcb);
                log_info(logger, "Recibido PCB con PID: %d", pcb->PID);
                assign_page_to_process(pcb);
                send_packet(cliente_socket, create_packet(PCB, pcb->size, pcb, serialize_pcb));
                break;
            }
            case END_PROCESS: {
                // Recibir un PCB y lo finaliza

            }
            // Errores
            case ERROR_OP:
                log_error(logger, "Cliente desconectado de %s...", server_name);
                return;
            default:
                log_error(logger, "Algo anduvo mal en el server de %s", server_name);
                log_info(logger, "Cop: %d", packet->op_code);
                return;
        }
    }

    log_warning(logger, "El cliente se desconecto de %s server", server_name);
}

/**
 * @brief Escucha las conexiones entrantes de los clientes.
 *
 * Esta función se encarga de escuchar las conexiones entrantes de los clientes.
 * Cuando se establece una nueva conexión, crea un nuevo hilo para procesarla.
 *
 * @param server_name El nombre del servidor.
 * @return 1 si se estableció una nueva conexión, 0 en caso contrario.
 */
int server_escuchar(char* server_name) {
    // Esperamos una nueva conexión de un cliente
    int cliente_socket = esperar_cliente(server_name, sockets->memoriaSocket);


    // Si se estableció una nueva conexión, creamos un nuevo hilo para procesarla
    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}

/**
 * @brief Finaliza todas las conexiones y libera los recursos utilizados.
 *
 * Esta función se encarga de desconectar el socket de servidor de la Memoria
 * y liberar los recursos utilizados por el logger.
 */
void fin_conexion(){
    // Desconectamos el socket del servidor de la Memoria
    disconnectServer(sockets->memoriaSocket);
    // Registramos un mensaje indicando que el servidor de la Memoria ha terminado
    log_info(logger,"Terminado el Servidor Memoria");
    // Liberamos los recursos utilizados por el logger
    log_destroy(logger);
}