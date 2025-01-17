//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_ENTRADASALIDA_CONEXION_H
#define TP_2024_1C_GRUPO_ENTRADASALIDA_CONEXION_H

#include <entradasalida_config.h>
#include <sockets/client.h>
#include <sockets/protocol.h>
#include "entradasalida_interfaces.h"

typedef struct {
    int fd;
    char* server_name;
}t_procesar_conexion_args;

void iniciarConexiones();
void fin_conexion();
void cliente_escuchar(int* client_socket);

#endif //TP_2024_1C_GRUPO_ENTRADASALIDA_CONEXION_H
