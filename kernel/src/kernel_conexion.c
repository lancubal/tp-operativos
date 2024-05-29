//
// Created by utnso on 4/14/24.
//

#include "kernel_conexion.h"


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
 * @brief Inicia las conexiones con los servidores de Memoria, CPU y Kernel.
 *
 * Esta función se encarga de iniciar las conexiones con los servidores de Memoria, CPU y Kernel.
 * Utiliza la configuración del Kernel para obtener las direcciones IP y los puertos necesarios para estas conexiones.
 *
 * @param kernelConfig La configuración del Kernel.
 */
void iniciarConexiones() {
    // Iniciamos el servidor del Kernel
    sockets = malloc(sizeof(socketsT));
    sockets->kernelSocket = iniciarServerProceso(kernel_config->ipKernel, kernel_config->puertoEscucha, "Kernel Server");
    // Nos conectamos al servidor de Memoria
    sockets->memoriaSocket = connectToServer(kernel_config->ipMemoria,kernel_config->puertoMemoria);
    // Nos conectamos al servidor de CPU para despacho
    sockets->dispatchSocket = connectToServer(kernel_config->ipCPU,kernel_config->puertoCPUDispatch);
    // Nos conectamos al servidor de CPU para interrupciones
    sockets->interruptSocket = connectToServer(kernel_config->ipCPU,kernel_config->puertoCPUInterrupt);
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
        // Recibimos el código de operación del mensaje
        t_packet* packet = malloc(sizeof(t_packet));
        if(recv_packet(cliente_socket, packet)) {
            log_info(logger, "Recibido mensaje con código de operación: %d\n", packet->op_code);
        }

        // Procesamos el mensaje de acuerdo a su código de operación
        switch (packet->op_code) {
            case PCB: {
                // Deserializamos el PCB
                deserialize_pcb(packet->payload, packet->payload_size, pcb);
                sem_post(&sem_pcb);
                break;
            }
            // Errores
            case -1:
                log_error(logger, "Cliente desconectado de %s...", server_name);
                break;
            default:
                break;
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
 * @param server_socket El descriptor de archivo del socket del servidor.
 * @return 1 si se estableció una nueva conexión, 0 en caso contrario.
 */
int server_escuchar(char* server_name, int* server_socket) {
    // Esperamos una nueva conexión de un cliente
    int cliente_socket = esperar_cliente(server_name, *server_socket);

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

/**
 * @brief Escucha las conexiones entrantes de los clientes en un bucle.
 *
 * Esta función se encarga de escuchar las conexiones entrantes de los clientes en un bucle.
 * Continúa escuchando hasta que no se establezcan más conexiones.
 *
 * @param server_socket El descriptor de archivo del socket del servidor.
 * @return -1 cuando no se establecen más conexiones.
 */
int phread_server_escuchar(void* server_socket){
    while (server_escuchar("Kernel Server",(int*)server_socket));
    return -1;
}

/**
 * @brief Finaliza todas las conexiones y libera los recursos utilizados.
 *
 * Esta función se encarga de desconectar todos los sockets de cliente y servidor,
 * y liberar los recursos utilizados por el logger.
 */
void fin_conexion(){
    // Desconectamos el socket del servidor del Kernel
    disconnectServer(sockets->kernelSocket);
    // Desconectamos el socket de cliente de Memoria
    disconnectClient(sockets->memoriaSocket);
    // Desconectamos el socket de cliente de CPU para despacho
    disconnectClient(sockets->dispatchSocket);
    // Desconectamos el socket de cliente de CPU para interrupciones
    disconnectClient(sockets->interruptSocket);
    // Registramos un mensaje indicando que el servidor del Kernel ha terminado
    log_info(logger,"Terminado el Servidor Kernel");
    // Liberamos los recursos utilizados por el logger
    log_destroy(logger);
}