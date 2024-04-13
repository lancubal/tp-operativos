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
#include "sockets/conexiontad.h"


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

void sendMessage(char*, int);

t_paquete* packetCreate(void);
void addToPacket(t_paquete*, void*, int);
void sendPacket(t_paquete*, int);
void freePacket(t_paquete*);
int connectToServer(char*, char*);
void* conectarA(ConectarArgsT*);
void disconnectClient(int);

#endif //TP_2024_1C_GRUPO_CLIENT_H
