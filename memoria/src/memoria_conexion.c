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
void iniciarConexiones(memoria_config_t* memoriaConfig) {
    // Iniciamos el servidor de Memoria utilizando la IP y el puerto especificados en la configuración de la Memoria.
    sockets.memoriaSocket = iniciarServerProceso(memoriaConfig->ipMemoria, memoriaConfig->puertoEscucha, "Memoria");
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

    OP_CODES cop;
    // Entramos en un bucle donde recibimos y procesamos los mensajes de los clientes
    while (cliente_socket != -1) {
        // Recibimos el código de operación del mensaje
        if (recv(cliente_socket, &cop, sizeof(OP_CODES), 0) != sizeof(OP_CODES)) {
            log_info(logger, "DISCONNECT!");
            return;
        }

        // Procesamos el mensaje de acuerdo a su código de operación
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
            case ERROR_OP:
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
    int cliente_socket = esperar_cliente(server_name, sockets.memoriaSocket);


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
    disconnectServer(sockets.memoriaSocket);
    // Registramos un mensaje indicando que el servidor de la Memoria ha terminado
    log_info(logger,"Terminado el Servidor Memoria");
    // Liberamos los recursos utilizados por el logger
    log_destroy(logger);
}