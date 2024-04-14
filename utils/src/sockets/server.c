//
// Created by utnso on 4/5/24.
//

#include "server.h"

int getOp(int socket_cliente)
{
    int cod_op;
    if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
        return cod_op;
    else
    {
        close(socket_cliente);
        return -1;
    }
}

void* getBuffer(int* size, int socket_cliente)
{
    void * buffer;

    recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
    buffer = malloc(*size);
    recv(socket_cliente, buffer, *size, MSG_WAITALL);

    return buffer;
}

void getMessage(int socket_cliente)
{
    int size;
    char* buffer = getBuffer(&size, socket_cliente);
    log_info(logger, "Me llego el mensaje %s", buffer);
    free(buffer);
}

t_list* getPacket(int socket_cliente)
{
    int size;
    int desplazamiento = 0;
    void * buffer;
    t_list* valores = list_create();
    int tamanio;

    buffer = getBuffer(&size, socket_cliente);
    while(desplazamiento < size)
    {
        memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
        desplazamiento+=sizeof(int);
        char* valor = malloc(tamanio);
        memcpy(valor, buffer+desplazamiento, tamanio);
        desplazamiento+=tamanio;
        list_add(valores, valor);
    }
    free(buffer);
    return valores;
}

int startServer(char* ip, char* puerto)
{

    int socket_servidor;

    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &servinfo);
    if(getaddrinfo(ip, puerto, &hints, &servinfo) != 0)
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

