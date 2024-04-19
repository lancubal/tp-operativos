//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_CPU_CONEXION_H
#define TP_2024_1C_GRUPO_CPU_CONEXION_H


#include <cpu_config.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <commons/log.h>
#include <pthread.h>
#include <sockets/server.h>
#include "sockets/conexiontad.h"
#include "sockets/protocol.h"


int server_escuchar(t_log* logger, char* server_name, int*  server_socket);
int phread_server_escuchar(void* server_socket);
void procesar_conexion(void* void_args);

#endif //TP_2024_1C_GRUPO_CPU_CONEXION_H
