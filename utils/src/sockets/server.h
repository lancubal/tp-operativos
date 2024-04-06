//
// Created by utnso on 4/5/24.
//

#ifndef TP_2024_1C_GRUPO_SERVER_H
#define TP_2024_1C_GRUPO_SERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>
#include <pthread.h>
#include <errno.h>
#include <utils/logger.h>

#define PUERTO "4444"

typedef enum
{
    MENSAJE,
    PAQUETE
}op_code;

int startServer(char* ip, char* puerto);
int waitClient(int);

void* getBuffer(int*, int);
t_list* getPacket(int);
void getMessage(int);
int getOp(int);
void iniciarServerProceso(char* ip, char* puerto, char* proceso);

#endif //TP_2024_1C_GRUPO_SERVER_H
