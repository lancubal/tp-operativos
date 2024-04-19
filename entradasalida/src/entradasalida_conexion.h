//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_ENTRADASALIDA_CONEXION_H
#define TP_2024_1C_GRUPO_ENTRADASALIDA_CONEXION_H

#include <entradasalida_config.h>
#include <sockets/client.h>

void iniciarConexiones(entradasalida_config_t * entradasalidaConfig, socketsT * sockets);
void fin_conexion(t_log* logger, socketsT * sockets);
#endif //TP_2024_1C_GRUPO_ENTRADASALIDA_CONEXION_H
