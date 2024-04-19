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
#include "sockets/conexiontad.h"
#include "sockets/protocol.h"





void iniciarConexiones(memoria_config_t* memoriaConfig, socketsT * sockets);
int server_escuchar(t_log* logger, char* server_name, socketsT * server_socket);
void procesar_conexion(void* void_args);
void fin_conexion(t_log* logger, socketsT * sockets);
#endif //TP_2024_1C_GRUPO_MEMORIA_CONEXION_H
