//
// Created by utnso on 4/5/24.
//

#include "server.h"

/**
 * @file server.c
 * @brief Funciones del servidor para iniciar el servidor.
 */

/**
 * @brief Inicia el servidor en la dirección IP y puerto especificados.
 *
 * Esta función inicia un servidor en la dirección IP y puerto especificados.
 * Configura las opciones del socket, crea el socket, y lo asocia a la dirección IP y puerto.
 * Luego, pone el servidor en modo de escucha para conexiones entrantes.
 * Si ocurre un error en cualquiera de estos pasos, la función registra el error y retorna -1.
 *
 * @param ip La dirección IP en la que se iniciará el servidor.
 * @param puerto El puerto en el que se iniciará el servidor.
 * @return El descriptor de socket del servidor iniciado, o -1 si hubo un error.
 */
int startServer(char* ip, char* puerto)
{
    int socket_servidor;

    struct addrinfo hints, *servinfo;

    // Configuramos las opciones para el socket
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Obtenemos la información de la dirección
    getaddrinfo(ip, puerto, &hints, &servinfo);
    if(errno != 0)
    {
        log_error(logger, "Error en getaddrinfo: %d\n", errno);
        return -1;
    }

    // Creamos el socket de escucha del servidor
    socket_servidor = socket(servinfo->ai_family,
                             servinfo->ai_socktype,
                             servinfo->ai_protocol);
    if(socket_servidor == -1)
    {
        log_error(logger, "Error al crear el socket: %d\n", errno);
        return -1;
    }

    // Asociamos el socket a un puerto
    if(bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
    {
        log_error(logger, "Error al bindear el socket, errno %d\n", errno);
        close(socket_servidor);
        return -1;
    }

    // Escuchamos las conexiones entrantes
    if(listen(socket_servidor, SOMAXCONN) != 0)
    {
        log_error(logger, "Error al escuchar\n");
        close(socket_servidor);
        return -1;
    }

    // Liberamos la información de la dirección
    freeaddrinfo(servinfo);

    // Retornamos el descriptor del socket del servidor
    return socket_servidor;
}

/**
 * @file server.c
 * @brief Funciones del servidor para manejar las conexiones de los clientes.
 */

/**
 * @brief Espera a que un cliente se conecte al servidor.
 *
 * Esta función espera a que un cliente se conecte al servidor y registra la conexión.
 * Si un cliente no puede conectarse, la función registra el error y termina el programa.
 *
 * @param name El nombre del servidor.
 * @param socket_servidor El descriptor de socket del servidor.
 * @return El descriptor de socket del cliente conectado.
 */
int esperar_cliente(const char* name, int socket_servidor) {
    // Estructura para almacenar la información de la dirección del cliente
    struct sockaddr_in dir_cliente;
    // Tamaño de la estructura de dirección del cliente
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    // Aceptar una conexión de cliente
    int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);
    // Si hubo un error al aceptar la conexión del cliente
    if(errno != 0) {
        // Registrar el error y terminar el programa
        log_error(logger, "El cliente no se pudo conectar por: errno %d", errno);
        exit(-1);
    }

    // Registrar la conexión exitosa del cliente
    log_info(logger, "Cliente conectado (a %s)\n", name);

    // Devolver el descriptor de socket del cliente conectado
    return socket_cliente;
}

/**
 * @file server.c
 * @brief Funciones del servidor para iniciar y desconectar el servidor.
 */

/**
 * @brief Inicia el servidor para un proceso específico.
 *
 * Esta función inicia un servidor para un proceso específico en la dirección IP y puerto especificados.
 * Si ocurre un error al iniciar el servidor, la función registra el error y termina el programa.
 *
 * @param ip La dirección IP en la que se iniciará el servidor.
 * @param puerto El puerto en el que se iniciará el servidor.
 * @param proceso El nombre del proceso para el que se está iniciando el servidor.
 * @return El descriptor de socket del servidor iniciado.
 */
int iniciarServerProceso(char* ip, char* puerto, char* proceso) {
    //Iniciar servidor
    int socketServidor = startServer(ip, puerto);
    if(errno != 0) {
        log_error(logger, "Error al iniciar servidor de %s", proceso);
        exit(-1);
    }
    log_info(logger, "Servidor de %s iniciado en: %s:%s", proceso, ip, puerto);

    return socketServidor;
}

/**
 * @brief Desconecta el servidor.
 *
 * Esta función cierra el socket del servidor, efectivamente desconectando el servidor.
 *
 * @param socketServidor El descriptor de socket del servidor a desconectar.
 */
void disconnectServer(int socketServidor)
{
    close(socketServidor);
}

