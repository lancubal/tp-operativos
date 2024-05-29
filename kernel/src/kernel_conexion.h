//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_KERNEL_CONEXION_H
#define TP_2024_1C_GRUPO_KERNEL_CONEXION_H

#include <sockets/server.h>
#include <sockets/client.h>
#include <kernel_config.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <commons/log.h>
#include <pthread.h>
#include <sockets/server.h>
#include "sockets/protocol.h"
#include "semaphore.h"
#include "kernel_config.h"

extern t_PCB * pcb;
extern sem_t sem_pcb;


void iniciarConexiones();
int server_escuchar(char* server_name, int*  server_socket);
int phread_server_escuchar(void* server_socket);
void procesar_conexion(void* void_args);
void fin_conexion();
void cliente_escuchar(int* client_socket);

#endif //TP_2024_1C_GRUPO_KERNEL_CONEXION_H
