//
// Created by utnso on 4/5/24.
//

#include "server.h"

int startServer(char* ip, char* puerto)
{

    int socket_servidor;

    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

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

    freeaddrinfo(servinfo);

    return socket_servidor;
}

void* waitClient(const int* socketServer)
{
    // Aceptamos un nuevo cliente
    int socketCliente = accept(*socketServer, NULL, NULL);
    if(errno != 0) {
        log_error(logger, "El cliente no se pudo conectar por: errno %d", errno);
        exit(-1);
    }
    log_info(logger, "Se conecto un cliente");

    pthread_exit((void*)(intptr_t) socketCliente); //paso int a intptr_t para que no tire warning
}

int esperar_cliente(t_log* logger, const char* name, int socket_servidor) {
    struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

    log_info(logger, "Cliente conectado (a %s)\n", name);

    return socket_cliente;
}

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

void disconnectServer(int socketServidor)
{
    close(socketServidor);
}

