//
// Created by utnso on 4/5/24.
//

#ifndef TP_2024_1C_GRUPO_CLIENT_H
#define TP_2024_1C_GRUPO_CLIENT_H

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include <commons/log.h>
#include "server.h"
#include "utils/logger.h"
#include <errno.h>


/*typedef enum
{
    MENSAJE,
    PAQUETE
}op_code;*/

typedef struct
{
    int size;
    void* stream;
} t_buffer;

typedef struct
{
    op_code codigo_operacion;
    t_buffer* buffer;
} t_paquete;



int connectToServer(char* ip, char* puerto);
void disconnect(int socket_cliente);

void sendMessage(char* mensaje, int socket_cliente);

t_paquete* packetCreate(void);
void addToPacket(t_paquete* paquete, void* valor, int tamanio);
void sendPacket(t_paquete* paquete, int socket_cliente);
void freePacket(t_paquete* paquete);
int conectarA(char* ip, char* puerto, char* nombreProceso);

#endif //TP_2024_1C_GRUPO_CLIENT_H
