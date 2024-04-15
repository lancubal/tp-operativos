//
// Created by utnso on 4/5/24.
//

#include "client.h"

int connectToServer(char *ip, char* puerto)
{
    struct addrinfo hints;
    struct addrinfo *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &server_info);

    // Ahora vamos a crear el socket.
    int socket_cliente = socket(server_info->ai_family,
                                server_info->ai_socktype,
                                server_info->ai_protocol);

    // Ahora que tenemos el socket, vamos a conectarlo
    connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

    freeaddrinfo(server_info);

    return socket_cliente;
}

void* conectarA(ConectarArgsT  *args){
    int socket_cliente = connectToServer(args->serverIP, args->puerto);
    if(errno != 0) {
        log_error(logger, "Error al conectar a %s errno %d", args->proceso, errno);
        exit(-1);
    }
    log_info(logger, "Conectado a %s", args->proceso);

    pthread_exit((void*)(intptr_t) socket_cliente);
}

void disconnectClient(int socketCliente)
{
    close(socketCliente);
}

