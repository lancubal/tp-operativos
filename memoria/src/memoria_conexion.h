//
// Created by utnso on 4/13/24.
//

#ifndef TP_2024_1C_GRUPO_MEMORIA_CONEXION_H
#define TP_2024_1C_GRUPO_MEMORIA_CONEXION_H
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <commons/log.h>
#include <pthread.h>
#include <sockets/server.h>
#include <memoria_config.h>
#include "sockets/protocol.h"
#include "sockets/client.h"
#include <semaphore.h>
#include "utils/module_tads.h"
#include "memoria_management.h"

extern char* instrucciones[];

void iniciarConexiones();
int server_escuchar(char* server_name);
void procesar_conexion(void* void_args);
void fin_conexion();

#endif //TP_2024_1C_GRUPO_MEMORIA_CONEXION_H
