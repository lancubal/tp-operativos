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

extern char* instrucciones[];

void iniciarConexiones(memoria_config_t* memoriaConfig);
int server_escuchar(char* server_name);
void procesar_conexion(void* void_args);
void fin_conexion();

#endif //TP_2024_1C_GRUPO_MEMORIA_CONEXION_H
