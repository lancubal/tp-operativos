//
// Created by utnso on 4/15/24.
//

#ifndef TP_2024_1C_GRUPO_NETWORKING_H
#define TP_2024_1C_GRUPO_NETWORKING_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <commons/collections/list.h>
#include "utils/logger.h"
#include "pthread.h"

typedef struct
{
    int size;
    void* stream;
} t_buffer;

typedef enum
{
    MENSAJE,
    PAQUETE
} op_code;

typedef struct
{
    op_code codigo_operacion;
    t_buffer* buffer;
} t_paquete;

t_paquete* packetCreate(void);
void addToPacket(t_paquete*, void*, int);
void sendPacket(t_paquete*, int);
void freePacket(t_paquete*);
void sendMessage(char*, int);
void* serializar_paquete(t_paquete*, int);

void* getBuffer(int*, int);
t_list* getPacket(int);
void* getMessage(int);
int getOp(int);

#endif //TP_2024_1C_GRUPO_NETWORKING_H
