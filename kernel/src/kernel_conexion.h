//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_KERNEL_CONEXION_H
#define TP_2024_1C_GRUPO_KERNEL_CONEXION_H

#include <sockets/server.h>
#include <sockets/client.h>
#include <kernel_config.h>
#include <pthread.h>

void iniciarConexiones(kernel_config_t* kernelConfig);

#endif //TP_2024_1C_GRUPO_KERNEL_CONEXION_H
