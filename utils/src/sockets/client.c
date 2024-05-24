//
// Created by utnso on 4/5/24.
//

#include "client.h"

/**
 * @file client.c
 * @brief Funciones del cliente para manejar las conexiones con el servidor.
 */

/**
 * @brief Conecta al servidor en la dirección IP y puerto especificados.
 *
 * Esta función crea un socket y lo conecta a la dirección IP y puerto especificados.
 * Si ocurre un error durante la conexión, la función registra el error y termina el programa.
 *
 * @param ip La dirección IP del servidor.
 * @param puerto El puerto del servidor.
 * @return El descriptor de socket del cliente conectado.
 */
int connectToServer(char *ip, char* puerto)
{
    // Define las hints para el socket
    struct addrinfo hints;
    struct addrinfo *server_info;

    // Inicializa las hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Permite IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM; // Socket de flujo
    hints.ai_flags = AI_PASSIVE; // Para la dirección IP comodín

    // Obtiene la información del servidor
    getaddrinfo(ip, puerto, &hints, &server_info);

    // Crea el socket
    int socket_cliente = socket(server_info->ai_family,
                                server_info->ai_socktype,
                                server_info->ai_protocol);

    // Conecta el socket
    connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
    // Si hay un error, lo registra y sale
    if(errno != 0) {
        log_error(logger, "Error al conectar a %s:%s errno: %s", ip, puerto, strerror(errno));
        exit(-1);
    }
    // Registra la conexión exitosa
    log_info(logger, "Conectado a %s:%s", ip, puerto);

    // Libera la información del servidor
    freeaddrinfo(server_info);

    // Devuelve el socket del cliente
    return socket_cliente;
}

/**
 * @brief Desconecta el cliente.
 *
 * Esta función cierra el socket del cliente, efectivamente desconectando el cliente del servidor.
 *
 * @param socketCliente El descriptor de socket del cliente a desconectar.
 */
void disconnectClient(int socketCliente)
{
    close(socketCliente);
}

