//
// Created by utnso on 4/5/24.
//

#ifndef TP_2024_1C_GRUPO_SERVER_H
#define TP_2024_1C_GRUPO_SERVER_H

#include<unistd.h>
#include<netdb.h>
#include <pthread.h>
#include <errno.h>
#include <utils/logger.h>

#define PUERTO "4444"

int startServer(char* ip, char*);
int esperar_cliente(const char* , int );
int iniciarServerProceso(char*, char*, char*);
void disconnectServer(int);

#endif //TP_2024_1C_GRUPO_SERVER_H
